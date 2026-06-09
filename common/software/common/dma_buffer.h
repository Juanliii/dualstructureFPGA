#ifndef STEREO_IR_DMA_BUFFER_H
#define STEREO_IR_DMA_BUFFER_H

#include <cstddef>
#include <cstdint>

namespace stereo_ir {

struct DmaBufferView {
    void *virtual_address = nullptr;
    std::uintptr_t physical_address = 0;
    std::size_t size_bytes = 0;
};

}  // namespace stereo_ir

#endif

