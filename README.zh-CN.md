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
