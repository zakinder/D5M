#include <stdio.h>
#include <xaxivdma.h>
#include <xbasic_types.h>
#include <xil_io.h>
#include <xil_printf.h>
#include <xparameters.h>
#include <xstatus.h>
#include "../PLATFORM/platform.h"
#include "../SYSTEM_CONFIG_HEADER/system_config_header.h"
// 0x100000
// 0x200000
unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR  + 0x100000);
//void print(char *str);
void pattern()
{
    int Status;
    XAxiVdma InstancePtr;
    /* Calling the API to configure and start VDMA without frame counter interrupt */
    run_triple_frame_buffer(&InstancePtr, 0, SCREEN_WIDTH_HORIZONTAL, SCREEN_HEIGHT_VERTICAL,srcBuffer, 3, 1);
    u32 vdma_stat;
    vdma_stat = Xil_In32(0x43000000);
    printf("MM2S vdma cntrl %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000004);
    printf("MM2S vdma stat %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000030);
    printf("S2MM vdma cntrl %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000034);
    printf("S2MM vdma stat %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000028);
    printf("Pointers %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x4300005c);
    printf("MM2S addr 1 %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000060);
    printf("MM2S addr 2 %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000064);
    printf("MM2S addr 3 %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x4300005c);
    printf("S2MM addr 1 %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000060);
    printf("S2MM addr 2 %x\n\r",(unsigned) vdma_stat);
    vdma_stat = Xil_In32(0x43000064);
    printf("S2MM addr 3 %x\n\r",(unsigned) vdma_stat);
    //readout first three lines from store 0
    // MM2S vdma cntrl 101508b
    // MM2S vdma stat 10000
    // S2MM vdma cntrl 101508b
    // S2MM vdma stat 16100
    // Pointers 0
    // MM2S addr 1 200000
    // MM2S addr 2 9e9000
    // MM2S addr 3 11d2000
    // S2MM addr 1 200000
    // S2MM addr 2 9e9000
    // S2MM addr 3 11d2000
    u32 address1 = 0x200000;
    u32 address2 = 0x9e9000;
    u32 address3 = 0x11d2000;
    printf("---------------------%x\n",address1);
    run_teast(address1);
    printf("---------------------%x\n",address2);
    run_teast(address2);
    printf("---------------------%x\n",address3);
    run_teast(address3);
    cleanup_platform();
}
void run_teast(u32 address) {
    int i;
    //u32 address = 0x200000;
    u8  pixel1, pixel2, pixel3;
    u32 pixel;
    for(i=0;i<1082;i++){
        pixel1 = Xil_In8(address);
        pixel2 = Xil_In8(address+1);
        pixel3 = Xil_In8(address+2);
        pixel = (pixel3<<16)|(pixel2<<8)|(pixel1);
        printf("%i- %x\n",i,(unsigned) pixel);
        address = address + 3;
        cleanup_platform();
    }
}
