#ifndef STEREO_IR_AXI_REGS_H
#define STEREO_IR_AXI_REGS_H

#include <cstdint>

namespace stereo_ir {

static constexpr std::uintptr_t kStereoPipelineBase = 0xA0000000;

static constexpr std::uint32_t kRegControl = 0x00;
static constexpr std::uint32_t kRegGie = 0x04;
static constexpr std::uint32_t kRegIer = 0x08;
static constexpr std::uint32_t kRegIsr = 0x0C;
static constexpr std::uint32_t kRegWidth = 0x10;
static constexpr std::uint32_t kRegHeight = 0x18;
static constexpr std::uint32_t kRegMinDisp = 0x20;
static constexpr std::uint32_t kRegMaxDisp = 0x28;
static constexpr std::uint32_t kRegFx = 0x30;
static constexpr std::uint32_t kRegFy = 0x38;
static constexpr std::uint32_t kRegCx = 0x40;
static constexpr std::uint32_t kRegCy = 0x48;
static constexpr std::uint32_t kRegBaseline = 0x50;

static constexpr std::uint32_t kImageWidth = 1224;
static constexpr std::uint32_t kImageHeight = 1024;
static constexpr std::uint32_t kBytesPerMono8Frame = kImageWidth * kImageHeight;

}  // namespace stereo_ir

#endif

