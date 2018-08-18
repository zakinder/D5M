#include <xil_types.h>
#define quit 0x1
#define QUIT_VALUE 999
#define VGA_FRAME_ADDRESS             0x00200000
#define VGA_MEMORY_ATTRIBUTE         0x00010c06
// CMDW [WRITE]
#define cmdw_vgadraw 0x777617264
#define cmdw_d5mreg 0x7EFE83F9
#define cmdw_d5mcsize 0x7A697363
// CMDR [READ]
#define cmdr_plread 0x7C25A8B2
#define cmdr_d5mreg 0x7C039379
#define cmdr_d5mviewmenu 0x77656976
// CMDS [SET]
// VDMA FUNCTION TESTS
#define cmds_exposer 0x7265736F
#define cmds_vdfxa 0xD94700A9
#define cmds_vdfxb 0xD94700AA
#define cmds_vdfxc 0xD94700AB
// VDMA TEST
#define cmds_vdtesta 0x61747365
#define cmds_vdtestb 0x62747365
#define cmds_vdtestc 0x63747365
// VDMA API
#define cmds_vdapia 0x7CC5F0A3
#define cmds_vdapib 0x7CC5F0A4
#define cmds_vdapic 0x7CC5F0A5
// VGA API
#define cmds_vgaapia 0x61697061
#define cmds_vgaapib 0x62697061
#define cmds_vgaapic 0x63697061
// CMDS QUIT
#define cmds_quit 0x95BA400A
#define cmds_space 0x20
// CMDS PHERI
#define cmds_uart 0x95BA4834
#define cmds_hdmitx 0x7CB0C08C
#define cmds_videoconfig 0x6F636F65
// CMDS PATTERN
#define cmds_patternv1 0x40CECE35
#define cmds_patternv2 0x40CECE36
#define cmds_patternv3 0x40CECE37
u32 uartcmd(u32 argA,u32 argB);
void menu_calls();