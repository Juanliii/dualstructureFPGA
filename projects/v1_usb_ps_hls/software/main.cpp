#include "../../../common/software/common/axi_regs.h"
#include "../../../common/software/common/dma_buffer.h"

#include <iostream>

int main()
{
    std::cout << "V1 USB PS HLS control placeholder\n";
    std::cout << "Input contract: "
              << stereo_ir::kImageWidth << "x" << stereo_ir::kImageHeight
              << " Mono8 left/right\n";
    std::cout << "Next: open USB camera, allocate DMA buffers, configure stereo_pipeline registers.\n";
    return 0;
}

