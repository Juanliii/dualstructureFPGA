# 双目红外 FPGA 三版本工程

本目录包含基于 AXU5EVB-E Zynq UltraScale+ MPSoC 开发板的三个双目红外深度计算 FPGA 工程骨架。

## 工程列表

| 工程目录 | 输入方式 | 采集位置 | PL/HLS 职责 |
| --- | --- | --- | --- |
| `projects/v1_usb_ps_hls` | USB 双目/深度参考相机 | PS Linux | 通过 DMA 输入共享 HLS 双目流水线 |
| `projects/v2_gige_ps_hls` | 两台 MV-CS013-60GN 千兆网口红外相机 | PS Linux | PL 产生相机/激光同步触发，并通过 DMA 输入共享 HLS 双目流水线 |
| `projects/v3_gige_pl_hls` | 两台 MV-CS013-60GN 千兆网口红外相机 | PL Ethernet/GVSP | PL 完成采集、相机/激光同步触发，并直接输出 AXI4-Stream 到共享 HLS 双目流水线 |

三个版本默认输入规格为：

```text
1224 x 1024
Mono8
30 fps
left + right 双目输入
```

## 三个工作流 Scope

### 工作流一 Scope：HLS 算法与 PS-DMA 验证流

对应工程：`projects/v1_usb_ps_hls`

包含内容：

- USB 双目/红外参考输入，或从文件/缓存模拟 left/right Mono8 输入；
- PS Linux 侧图像采集、缓存管理和 DMA buffer 准备；
- PS 通过 AXI DMA/AXI4-Stream 将左右图像送入 PL；
- PL 中运行共享 `stereo_pipeline` HLS IP；
- HLS IP 输出 `depth stream` 和 `pointcloud stream`；
- PS 读取结果、保存结果，并与 CPU 软件实现做延迟和输出一致性对比。

不包含内容：

- 不要求接入真实双 GigE 工业相机；
- 不要求 PL 解析以太网/GVSP 数据包；
- 不要求完成最终真实双目匹配算法，第一阶段可使用占位 depth/pointcloud。

### 工作流二 Scope：真实双 GigE 相机 PS 采集流

对应工程：`projects/v2_gige_ps_hls`

包含内容：

- 两台 MV-CS013-60GN 千兆网口红外相机；
- PS Linux 侧 GigE Vision/GenICam 相机发现、配置和采集；
- 左右帧配对、帧号/时间戳检查、丢帧统计；
- PL 产生左右相机硬触发、曝光窗口、激光/散斑投影器使能时序；
- PS 通过 AXI-Lite 配置 PL 触发参数并读取触发状态；
- PS 将真实左右图像通过 DMA 送入共享 HLS `stereo_pipeline`；
- 输出深度图和点云，并评估真实相机链路下的同步、延迟和稳定性。

不包含内容：

- 不要求 PL 直接接收 GigE 数据包；
- 不要求 PL 解析 IPv4/UDP/GVSP；
- 不改变 HLS 算法 IP 的主体结构，优先复用工作流一的 `stereo_pipeline`。

### 工作流三 Scope：PL 纯采集低延迟流

对应工程：`projects/v3_gige_pl_hls`

包含内容：

- 两台 MV-CS013-60GN 千兆网口红外相机；
- PL RGMII / Ethernet MAC 接收链路；
- PL 解析 Ethernet、IPv4、UDP、GVSP，并完成 packet-to-pixel stream；
- PL 检查 packet id、frame id、丢包、乱序和帧完整性；
- PL 统一产生左右相机触发、曝光窗口、激光/散斑投影器时序；
- PL 将重组后的 left/right Mono8 像素流直接送入共享 HLS `stereo_pipeline`；
- PS 仅负责参数配置、状态监控、错误计数读取和结果管理。

不包含内容：

- 不以 PS Linux 网络采集作为主数据路径；
- 不把双目匹配、深度转换、点云生成改写成大规模手写 RTL；
- 不跳过 V1/V2 的验证结果，V3 应在已有 HLS IP 和同步策略稳定后推进。

## 硬件与工具链假设

- 开发板：AXU5EVB-E。
- FPGA/MPSoC：XCZU5EV-2SFVC784I。
- 工具链：Vivado / Vitis / Vitis HLS 2021.2。
- PS DDR4：4 GB。
- PL DDR4：1 GB。
- 板载可用接口：USB3.0、PS 千兆以太网、PL 千兆以太网、DisplayPort、UART、SD 卡。
- V2/V3 相机：MV-CS013-60GN，1224 x 1024 Mono8，GigE Vision 2.0 / GenICam。
- V1 可使用奥比中光 Gemini 335 或其他 USB 双目/红外设备作为采集参考，但进入 HLS 前需要在 PS 软件侧转换为统一输入格式。

## 目录结构

```text
stereo_ir_fpga/
├── README.md
├── README.zh-CN.md
├── common/
│   ├── hls/                  # 三个版本复用的 Vitis HLS stereo_pipeline IP
│   └── software/             # Linux 侧通用寄存器和 DMA 头文件
├── docs/
│   └── references.md         # 本地参考文档和 PDF 约束摘要
└── projects/
    ├── v1_usb_ps_hls/        # V1：USB 输入，PS 采集，PL/HLS 计算
    ├── v2_gige_ps_hls/       # V2：双 GigE 相机，PS 采集，PL/HLS 计算
    └── v3_gige_pl_hls/       # V3：双 GigE 相机，PL 采集，PL/HLS 计算
```

## 当前 HLS 状态

当前已创建共享 HLS 内核：

```text
common/hls/kernels/stereo_pipeline/
├── stereo_pipeline.cpp
├── stereo_pipeline.h
├── tb_stereo_pipeline.cpp
└── run_hls.tcl
```

第一版 `stereo_pipeline` 是最小可运行占位版本：

- 消费 left/right 两路 Mono8 AXI4-Stream 输入；
- 每个像素输出固定 `1000 mm` 深度；
- 按 `X=(u-cx)*Z/fx`、`Y=(v-cy)*Z/fy`、`Z=depth` 输出 Q16.16 点云；
- `TLAST` 当前按每行末尾输出；
- 保留完整 AXI4-Stream 和 AXI-Lite 参数接口，后续可逐步替换真实双目匹配算法。

## HLS 构建与验证

在已安装 Vitis HLS 2021.2 的环境中运行：

```bash
cd stereo_ir_fpga/common/hls
vitis_hls -f scripts/run_all_csims.tcl
```

导出 HLS IP：

```bash
cd stereo_ir_fpga/common/hls
vitis_hls -f scripts/export_all_ips.tcl
```

当前已经验证通过：

```text
CSim done with 0 errors
生成 IP: common/hls/ip_repo/stereo_pipeline/stereo_pipeline.zip
```

## 三条工作流

本工程不是三个互不相关的项目，而是三条逐步递进的工作流：先验证算法和 DMA 通路，再接入真实双 GigE 相机系统，最后把采集和同步时序尽量下沉到 PL，形成低延迟方案。

| 工作流 | 对应工程 | 核心目标 | 主要完成的工作 |
| --- | --- | --- | --- |
| 工作流一：HLS 算法与 PS-DMA 验证流 | `v1_usb_ps_hls` | 用 USB 输入快速打通 PS 采集、DMA、HLS IP、结果回读 | 建立共享 `stereo_pipeline` HLS IP；验证 AXI4-Stream 输入输出；在 PS Linux 中准备 USB/V4L2 采集入口；通过 DMA 把 left/right 图像送入 PL；读取 depth 和 pointcloud 输出 |
| 工作流二：真实双 GigE 相机 PS 采集流 | `v2_gige_ps_hls` | 接入两台 MV-CS013-60GN，并验证真实相机、硬触发、散斑/激光同步和 HLS 计算 | 在 PS Linux 中完成 GigE Vision/GenICam 相机配置和采集；PL 产生左右相机触发和激光/投影器使能；PS 配置触发参数并做左右帧配对；继续复用 V1 的 HLS 深度计算 IP |
| 工作流三：PL 纯采集低延迟流 | `v3_gige_pl_hls` | 将 GigE 接收、GVSP 解析、帧重组、同步触发尽量放入 PL，PS 只负责配置和监控 | 在 PL 中实现 RGMII/MAC/IPv4/UDP/GVSP 解析；将相机数据直接转为像素 AXI4-Stream；PL 统一管理相机触发、曝光窗口、激光/投影器时序和 frame_id；HLS IP 直接消费 PL 图像流并输出深度图和点云 |

三条工作流的关系：

```text
工作流一：证明 HLS IP 和 PS-DMA 数据通路可用
    -> 工作流二：换成真实双 GigE 相机，并加入 FPGA 同步触发
        -> 工作流三：把采集链路也搬到 PL，降低延迟和 PS 负载
```

每条工作流都复用同一个核心目标：让 `stereo_pipeline` 最终输出 `depth stream` 和 `pointcloud stream`。区别在于输入数据从哪里来、采集由谁负责、同步时序由谁控制。

### 每条工作流可测评内容

| 工作流 | 可测评对象 | 典型测评指标 | 主要价值 |
| --- | --- | --- | --- |
| 工作流一：HLS 算法与 PS-DMA 验证流 | FPGA 与 CPU 在红外图像转深度图、点云生成上的性能差异 | 单帧处理延迟、端到端延迟、吞吐帧率、CPU 占用、PL 资源占用、DMA 搬运开销、depth/pointcloud 输出一致性 | 证明把深度计算搬到 FPGA 是否值得；量化 CPU 软件算法和 HLS/PL 加速之间的延迟差距 |
| 工作流二：真实双 GigE 相机 PS 采集流 | 真实双相机系统的采集稳定性、同步效果和 FPGA 触发控制效果 | 左右帧时间差、触发抖动、丢帧率、GigE 采集延迟、PS 网络负载、DMA 排队延迟、激光/散斑开启时序与曝光窗口匹配程度 | 验证真实硬件链路是否能稳定给 HLS IP 喂数据；评估 FPGA 触发相比 Linux 软件触发的同步优势 |
| 工作流三：PL 纯采集低延迟流 | PS 采集方案与 PL 采集方案的低延迟差异，以及完整 PL 数据链路可靠性 | 以太网包到像素流延迟、GVSP 解析延迟、帧重组延迟、端到端深度输出延迟、丢包/乱序检测能力、PS 占用下降幅度、系统最大稳定帧率 | 评估最终低延迟架构是否优于 PS 采集架构；验证 PL 直采、PL 同步和 HLS 深度计算能否组成完整实时链路 |

建议在三个工作流中统一记录以下基础数据，便于横向比较：

```text
输入规格：1224 x 1024 Mono8 x 2
目标帧率：30 fps
输出：uint16 depth + Q16.16 XYZ pointcloud
统计项：平均延迟、P95/P99 延迟、最大延迟、丢帧率、CPU 占用、PL 资源、功耗
```

## 三个版本的开发重点

### V1：USB 输入 + PS 采集 + HLS

目标是验证最基础的数据通路：

```text
USB 相机
-> PS Linux 采集
-> PS DDR
-> AXI DMA
-> HLS stereo_pipeline
-> depth stream + pointcloud stream
-> PS 读取结果
```

优先完成 USB/V4L2 采集、DMA buffer、HLS 寄存器配置和结果保存。

### V2：双 GigE 相机 + PS 采集 + HLS

目标是接入两台 MV-CS013-60GN：

```text
双 GigE 相机
-> 千兆交换机/PS 网口
-> PS Linux 采集
-> PS DDR
-> AXI DMA
-> HLS stereo_pipeline
```

优先完成 GenICam/GigE Vision 采集、硬触发配置、散斑投影同步和左右帧配对。

V2 中图像采集仍由 PS Linux 完成，但相机同步和红外散斑/激光器控制建议交给 FPGA PL：

```text
PS Linux 写 AXI-Lite 参数
-> PL trigger_controller
-> left_camera_trigger
-> right_camera_trigger
-> laser/projector_enable
```

PL 侧负责产生稳定时序，例如 30 fps 帧周期、相机触发脉宽、曝光窗口、激光提前量、激光开启时间和左右相机相位。PS 只负责配置、启动/停止和读取状态。

建议 V2 RTL 触发模块：

```text
rtl/trigger/
├── sync_timing_ctrl.sv
├── camera_trigger_gen.sv
├── projector_trigger_gen.sv
└── trigger_regs.sv
```

### V3：双 GigE 相机 + PL 纯采集 + HLS

目标是最终低延迟版本：

```text
PL RGMII / Ethernet MAC
-> IPv4 / UDP / GVSP Parser
-> Packet to Pixel Stream
-> HLS stereo_pipeline
-> depth stream + pointcloud stream
```

RTL 侧主要负责以太网接收、UDP/GVSP 解析、丢包/乱序检测和帧重组；双目深度算法仍优先放在 HLS 中实现。

V3 中相机同步和红外散斑/激光器控制也应放在 PL，与 PL 纯采集时序统一：

```text
PL trigger_controller
├── left_camera_trigger
├── right_camera_trigger
├── laser/projector_enable
├── exposure_window
├── frame_id_counter
└── status/counter registers
```

典型时序：

```text
t0: 打开激光/散斑投影器
t0 + laser_lead_us: 同时触发左右相机
t0 + laser_lead_us + exposure_us: 曝光窗口结束
t0 + laser_on_us: 关闭激光/散斑投影器
t0 + frame_period_us: 下一帧
```

注意：FPGA 管脚通常不应直接驱动相机 Line 输入或激光器，需要根据电压、电流、极性和安全联锁加入电平转换、光耦、MOSFET 或专用驱动电路。

## 后续建议

1. 补齐 AXU5EVB-E 官方 XDC 和 PS 配置。
2. 在 Vivado 中完成 V1 的最小 Block Design：PS、AXI DMA、HLS IP、AXI-Lite 控制。
3. 在 Linux 用户态完成 V1 的 DMA/HLS 控制程序。
4. 在 V2/V3 中完成 PL 触发控制器：帧周期、相机触发、曝光窗口、激光提前/延后、状态计数器。
5. 将占位深度替换为 `disparity_to_depth` 和 `depth_to_pointcloud` 独立 HLS kernel。
6. 再逐步加入 Census、代价计算、WTA、滤波和校正 LUT。
