#include <xbasic_types.h>
#define id_set_hdmi 0x4B48633B
#define id_set_camera 0x683EB001
#define id_set_uart 0x4B489514
#define id_set_camerasize 0x10BD1675
#define id_set_mipicamera 0x656D6163
#define id_write_mipicamerareg 0x4FD29D30
#define id_read_hdmiiic 0x63696969
#define id_read_cameraiic 0x204A3169
#define id_read_hdmireg 0x67657269
#define id_read_camerareg 0x204A34C9
#define id_read_vdma1test 0x65740161
#define id_read_vdma2test 0x65740261
#define mipi_reset_n 0
#define mipi_cs_n 4
#define camera_pwdn_n 8
void img_init();
int camera_exposer(u16 exposer_value);
u16 img(u16 read_address);
void camerawrite(u16 value);
void cameraread();
void master_menu();
void cmd_menu();
void cameramenu();
void camera_set_registers();
int img_write_register(u8 address, u16 value);
int img_read_register(u8 address, u16 *value);
void mipicamerawritereg(u16 reg,u16 value);
void mipiread();
void mipicameraread();
u32 read_in(u8 offset);
void write_in(u8 offset,u32 data);