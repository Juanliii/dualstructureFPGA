#include "../../../common/software/common/axi_regs.h"
#include "../../../common/software/common/dma_buffer.h"

#include <iostream>

int main()
{
    std::cout << "V2 GigE PS HLS control placeholder\n";
    std::cout << "Camera: MV-CS013-60GN, "
              << stereo_ir::kImageWidth << "x" << stereo_ir::kImageHeight
              << " Mono8 @ 30 fps\n";
    std::cout << "Next: configure GenICam trigger, acquire left/right frames, DMA into stereo_pipeline.\n";
    return 0;
}

