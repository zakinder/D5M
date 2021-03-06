#include <sleep.h>
#include <stdio.h>
#include <xiic_l.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xparameters.h>
#include <xstatus.h>
#include "../UART/uartio.h"
#define IIC_BASE_ADDRESS XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR//0x41610000
#define SENSOR_ADDRESS 0x5d // 0xBA - write mode, 0xBB read mode
#define IMG_OUTPUT_CONTROL 7
#define IMG_GLOBAL_GAIN 0x35
#define IMG_PIXEL_CLOCK_CONTROL    10
#define IMG_PIXEL_CLOCK_INVERT 0x8000
#define IMG_RESET 13
#define IIC_BASE_ADDRESS XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR//0x41610000
#define SENSOR_ADDRESS 0x5d // 0xBA - write mode, 0xBB read mode
const static char message[] = "Usage: img <r|w> <address> [value|size] (register operation)\n";
struct registerEntry {
    u8 index;
    char name[20];
};
struct registerEntry registerEntries[] = {
    {0, "Chip Version"}, {1, "Row Start"}, {2, "Column Start"}, {3, "Row Size"}, {4, "Column Size"},
    {5, "Horizontal Blank"}, {6, "Vertical Blank"}, {7, "Output Control"}, {8, "Shutter Width Upper"},
    {9, "Shutter Width Lower"}, {10, "Pixel Clock Control"}, {11, "Restart"}, {12, "Shutter Delay"},
    {13, "Reset"}, {16, "PLL Control"}, {17,"PLL Config 1"}, {18, "PLL Config 2"},
    {30, "Read Mode 1"}, {32, "Read Mode 2"}, {34, "Row Address Mode"}, {35, "Column address mode"},
    {43, "Green1 Gain"}, {44, "Blue Gain"}, {45, "Red Gain"}, {46, "Green2 Gain"},
    {53, "Global Gain"}, {75, "Row black offset"}, {160, "TestPatternControl"}, {161, "Test Pattern Green"},
    {162, "Test Pattern Red"}, {163, "Test Pattern Blue"}, {164, "TestPatBarWidth"}, {255, "Chip Version Alt"}
};
s16 find_register(struct registerEntry entries[], u8 index) { // find register starting at index
    u16 i = 0;
    for (i = 0; i < sizeof(registerEntries)/sizeof(struct registerEntry); i++) {
        if (entries[i].index == index) {
            return i;
        }
    }
    return -1;
}
int img_read_register(u8 address, u16 *value) {
    u8 buffer[2];
    if(XIic_Send(IIC_BASE_ADDRESS, SENSOR_ADDRESS, &address, 1, XIIC_STOP) != 1) {
        return XST_FAILURE;
    }
    if (XIic_Recv(IIC_BASE_ADDRESS, SENSOR_ADDRESS, buffer, 2, XIIC_STOP) != 2) {
        return XST_FAILURE;
    }
    *value = ((buffer[0] << 8) & 0xFF00) + buffer[1];
    return XST_SUCCESS;
}
int img_write_register(u8 address, u16 value) {
    u8 buffer[3] = {SENSOR_ADDRESS, 0, 0};
    buffer[0] = address;
    buffer[1] = (value & 0xFF00) >> 8;
    buffer[2] = (value & 0xFF);
    if (XIic_Send(XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR, SENSOR_ADDRESS, buffer, 3, XIIC_STOP) != 3){
        return XST_FAILURE;
    } else {
        return XST_SUCCESS;
    }
}
int camera_exposer(u16 exposer_value) {
    u8 buffer[3] = {SENSOR_ADDRESS, 0, 0};
    buffer[0] = 9;
    buffer[1] = (exposer_value & 0xFF00) >> 8;
    buffer[2] = (exposer_value & 0xFF);
    if (XIic_Send(XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR, SENSOR_ADDRESS, buffer, 3, XIIC_STOP) != 3){
        return XST_FAILURE;
    } else {
        return XST_SUCCESS;
    }
}
void img_init() {
    int ret = 0;
    u16 register_value = 0;
    ret = img_write_register(IMG_RESET, 1); // software-reset of sensor
    printf("\n ret %x\n\n",ret);
    ret |= img_write_register(IMG_RESET, 0);
    printf("\n ret %x\n\n",ret);
    ret |= img_read_register(IMG_PIXEL_CLOCK_CONTROL, &register_value); //invert pixel clock
    printf("\n ret %x\n\n",ret);
    register_value |= IMG_PIXEL_CLOCK_INVERT;
    ret |= img_write_register(IMG_PIXEL_CLOCK_CONTROL, register_value);
    ret |= img_write_register(IMG_OUTPUT_CONTROL, 0x1202); // set data output slew rate to 4 (half strength)
    ret |= img_write_register(IMG_GLOBAL_GAIN,16); // Sets the analog gain to 2
    if (ret != XST_SUCCESS)    print("Could not initialize\n");
    else print("Image sensor initialized\n");
}
void camera_set_registersv1() {
    int ret = 0;
    int err=0;
    u16 value;
    u16 R_01_value = 0x0036; //    set start row
    u16 R_02_value = 0x0010; //    set start column
    u16 R_03_value = 0x0438; //    set row size 1920[780] 600[258]
    u16 R_04_value = 0x0780; //    set column size 1080[438] 400[190]
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0019; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1807; //    PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0002; //    PLL_p1_Divider
    usleep(100);
    u16 R_1X_value = 0x0053; //    set USE PLL
    usleep(100);
    u16 R_20_value = 0xc000; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x000f; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x000f; // 44[15] Blue Gain
    u16 R_2D_value = 0x000f; // 45[15] Red Gain
    u16 R_2E_value = 0x000f; // 46[15] Green 2 Gain
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x0000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x01A8; // 73[424] row black target
    ret |=  img_write_register(0x001,R_01_value);
    printf("Row Start: H: %02x , D: %d\n",R_01_value);
    ret |=  img_write_register(0x002,R_02_value);
    printf("Column Start: H: %02x , D: %d\n",R_02_value);
    ret |=  img_write_register(0x003,R_03_value);
    printf("Row Size: H: %02x , D: %d\n",R_03_value);
    ret |=  img_write_register(0x004,R_04_value);
    printf("Column Size: H: %02x , D: %d\n",R_04_value);
    ret |=  img_write_register(0x005,R_05_value);
    printf("Horizontal Blank: H: %02x , D: %d\n",R_05_value);
    ret |=  img_write_register(0x006,R_06_value);
    printf("Vertical Blank: H: %02x , D: %d\n",R_06_value);
    ret |=  img_write_register(0x00A,R_0A_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n",R_0A_value);
    ret |=  img_write_register(0x010,R_10_value);
    printf("PLL Control: H: %02x , D: %d\n",R_10_value);
    ret |=  img_write_register(0x011,R_11_value);
    printf("PLL Config 1: H: %02x , D: %d\n",R_11_value);
    ret |=  img_write_register(0x012,R_12_value);
    printf("PLL Config 2: H: %02x , D: %d\n",R_12_value);
    ret |=  img_write_register(0x010,R_1X_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n",R_1X_value);
    ret |=  img_write_register(0x020,R_20_value);
    printf("Read Mode 2: H: %02x , D: %d\n",R_20_value);
    ret |=  img_write_register(0x022,R_22_value);
    printf("Row Address Mode: H: %02x , D: %d\n",R_22_value);
    ret |=  img_write_register(0x023,R_23_value);
    printf("Column address mode: H: %02x , D: %d\n",R_23_value);
    ret |=  img_write_register(0x02B,R_2B_value);
    printf("Green1 Gain: H: %02x , D: %d\n",R_2B_value);
    ret |=  img_write_register(0x02C,R_2C_value);
    printf("Blue Gain: H: %02x , D: %d\n",R_2C_value);
    ret |=  img_write_register(0x02D,R_2D_value);
    printf("Red Gain: H: %02x , D: %d\n",R_2D_value);
    ret |=  img_write_register(0x02E,R_2E_value);
    printf("Green2 Gain: H: %02x , D: %d\n",R_2E_value);
    ret |=  img_write_register(0x049,R_49_value);
    printf("TestPatBarWidth: H: %02x , D: %d\n",R_49_value);
    ret |=  img_write_register(0x098,R_98_value);
    printf("Test Pattern Blue: H: %02x , D: %d\n",R_98_value);
    ret |=  img_write_register(0x0A0,R_A0_value);
    printf("TestPatternControl: H: %02x , D: %d\n",R_A0_value);
    ret |=  img_write_register(0x0A1,R_A1_value);
    printf("Test Pattern Green: H: %02x , D: %d\n",R_A1_value);
    ret |=  img_write_register(0x0A2,R_A2_value);
    printf("Test Pattern Red: H: %02x , D: %d\n",R_A2_value);
}
void camera_set_registers() {
    int ret = 0;
    int err=0;
    u16 value;
    u16 R_01_value = 0x0036; //    set start row
    u16 R_02_value = 0x0010; //    set start column
    u16 R_03_value = 0x0438; //    set row size 1920[780] 600[258]
    u16 R_04_value = 0x0780; //    set column size 1080[438] 400[190]
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0019; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    //PLL Programming Example
    //The following is an example of register settings
    //for input fXCLKIN = 24 MHz and output fPIXCLK = 96MHz
        //REG=0, 0x10, 0x0051 // PLL_CONTROL; POWER UP PLL
        //REG=0,0x11, 0x4805 // PLL_CONFIG_1; CONFIG_1; PLL_n_Divider=5,
        //PLL_m_Factor=72 (N=6, M=72)
        //REG=0,0x12, 0x0002 // PLL_CONFIG_2; PLL_p1_Divider=2, (P1=3)
        //Delay=1, // Wait 1ms for VCO to lock
        //REG=0, 0x10, 0x0053 // PLL_CONTROL; USE PLL
        //DELAY=1, // Wait 1ms
        //In this example,
    	//PIXCLK = ( fXCLKIN � M ) / ( N � P1 )
    	//= 24 MHz x 72 / 6 x 3 = 96 MHz
        //In this example,
    	//PIXCLK = ( fXCLKIN � M ) / ( N � P1 )
    	//= 25 MHz x 36 / 6 x 3 = 50 MHz
        //In this example,
    	//PIXCLK = ( fXCLKIN � M ) / ( N � P1 )
    	//= 25 MHz x 18 / 6 x 3 = 50 MHz
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1807; //    PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0002; //    PLL_p1_Divider
    usleep(100);
    u16 R_1X_value = 0x0053; //    set USE PLL
    usleep(100);
    u16 R_20_value = 0xc000; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x000f; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x000f; // 44[15] Blue Gain
    u16 R_2D_value = 0x000f; // 45[15] Red Gain
    u16 R_2E_value = 0x000f; // 46[15] Green 2 Gain
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x0000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x01A8; // 73[424] row black target
    // 1 Row Start
    printf("--------------Start Write-----------------\n");
    ret |=  img_write_register(0x001,R_01_value);
    printf("Row Start: H: %02x , D: %d\n",R_01_value);
    // 2 Column Start
    ret |=  img_write_register(0x002,R_02_value);
    printf("Column Start: H: %02x , D: %d\n",R_02_value);
    // 3 Row Size
    ret |=  img_write_register(0x003,R_03_value);
    printf("Row Size: H: %02x , D: %d\n",R_03_value);
    // 4 Column Size
    ret |=  img_write_register(0x004,R_04_value);
    printf("Column Size: H: %02x , D: %d\n",R_04_value);
    // 5 Horizontal Blank
    ret |=  img_write_register(0x005,R_05_value);
    printf("Horizontal Blank: H: %02x , D: %d\n",R_05_value);
    // 6 Vertical Blank
    ret |=  img_write_register(0x006,R_06_value);
    printf("Vertical Blank: H: %02x , D: %d\n",R_06_value);
    // 7 Output Control
    //ret |=  img_write_register(0x007,value);
    //printf("Output Control: %d\n",value);
    // 8 Shutter Width Upper
    //ret |=  img_write_register(0x008,value);
    //printf("Shutter Width Upper: %d\n",value);
    // 9 Shutter Width Lower
    //ret |=  img_write_register(0x009,value);
    //printf("Shutter Width Lower: %d\n",value);
    // 10|Pixel Clock Control
    ret |=  img_write_register(0x00A,R_0A_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n",R_0A_value);
    // 12|Shutter Delay
    //ret |=  img_write_register(0x00C,value);
    //printf("Shutter Delay: %d\n",value);
    //16|PLL Control
    ret |=  img_write_register(0x010,R_10_value);
    printf("PLL Control: H: %02x , D: %d\n",R_10_value);
    //17|PLL Config 1
    ret |=  img_write_register(0x011,R_11_value);
    printf("PLL Config 1: H: %02x , D: %d\n",R_11_value);
    //18|PLL Config 2
    ret |=  img_write_register(0x012,R_12_value);
    printf("PLL Config 2: H: %02x , D: %d\n",R_12_value);
    //10|PLL Control
    ret |=  img_write_register(0x010,R_1X_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n",R_1X_value);
    // 30|Read Mode 1
    //ret |=  img_write_register(0x01E,value);
    //printf("Read Mode 1: %d\n",value);
    // 32|Read Mode 2
    ret |=  img_write_register(0x020,R_20_value);
    printf("Read Mode 2: H: %02x , D: %d\n",R_20_value);
    // 34|Row Address Mode
    ret |=  img_write_register(0x022,R_22_value);
    printf("Row Address Mode: H: %02x , D: %d\n",R_22_value);
    // 35|Column address mode
    ret |=  img_write_register(0x023,R_23_value);
    printf("Column address mode: H: %02x , D: %d\n",R_23_value);
    // 43|Green1 Gain
    ret |=  img_write_register(0x02B,R_2B_value);
    printf("Green1 Gain: H: %02x , D: %d\n",R_2B_value);
    // 44|Blue Gain
    ret |=  img_write_register(0x02C,R_2C_value);
    printf("Blue Gain: H: %02x , D: %d\n",R_2C_value);
    // 45|Red Gain
    ret |=  img_write_register(0x02D,R_2D_value);
    printf("Red Gain: H: %02x , D: %d\n",R_2D_value);
    // 46|Green2 Gain
    ret |=  img_write_register(0x02E,R_2E_value);
    printf("Green2 Gain: H: %02x , D: %d\n",R_2E_value);
    // 53|Global Gain
    //ret |=  img_write_register(0x035,value);
    //printf("Global Gain: %d\n",value);
    // 73|row black target
    ret |=  img_write_register(0x049,R_49_value);
    printf("TestPatBarWidth: H: %02x , D: %d\n",R_49_value);
    // 75|Row black offse
    //ret |=  img_write_register(0x04B,value);
    //printf("Row black offse: %d\n",value);
    // 152|disble calibration
    ret |=  img_write_register(0x098,R_98_value);
    printf("Test Pattern Blue: H: %02x , D: %d\n",R_98_value);
    // 160|TestPatternControl
    ret |=  img_write_register(0x0A0,R_A0_value);
    printf("TestPatternControl: H: %02x , D: %d\n",R_A0_value);
    // 161|Test Pattern Green
    ret |=  img_write_register(0x0A1,R_A1_value);
    printf("Test Pattern Green: H: %02x , D: %d\n",R_A1_value);
    // 162|Test Pattern Red
    ret |=  img_write_register(0x0A2,R_A2_value);
    printf("Test Pattern Red: H: %02x , D: %d\n",R_A2_value);
    // 163|Test Pattern Blue
    //ret |=  img_write_register(0x0A3,value);
    //printf("Test Pattern Blue: %d\n",value);
    // 164|TestPatBarWidth
    //ret |=  img_write_register(0x0A4,value);
    //printf("TestPatBarWidth: %d\n",value);
    printf("--------------End Write-----------------\n");
}
void camerawrite(u16 value) {
    int ret = 0;
    int err=0;
    u16 columnsize;
    err = img_read_register(0x004,&columnsize);
    printf("Column Size Before @ 0x004 : %d\n",columnsize);
    printf("Column Size to be Set @ 0x004 : %d\n",value);
    ret |= img_write_register(0x004,value);
    if (ret != XST_SUCCESS)    print("Could not initialize Column Size\n");
    else printf("Column Size Updated\n");
    err = img_read_register(0x004,&columnsize);
    printf("Column Size After @ 0x004 : %d\n",columnsize);
}
void cameraread() {
    u16 chipversion;
    u16 columnsize;
    u16 columnStart;
    u16 rowsize;
    u16 rowstart;
    u16 columnstart;
    int err=0;
    u16 vb;
    printf("READ CAMERA REGISTERS\n");
	printf("CHIPVERSION\n");
    err = img_read_register(0x000,&chipversion);
    printf("D[%d]-H[%02x]\n",chipversion);
	printf("Row Start\n");
    err = img_read_register(0x001,&rowstart);
    printf("D[%d]-H[%02x]\n",rowstart);
	printf("Column Start\n");
    err = img_read_register(0x002,&columnstart);
    printf("D[%d]-H[%02x]\n",columnstart);
	printf("Row Size\n");
    err = img_read_register(0x003,&rowsize);
    printf("D[%d]-H[%02x]\n",rowsize);
	printf("Column Start\n");
    err = img_read_register(0x004,&columnStart);
    printf("D[%d]-H[%02x]\n",columnStart);
	printf("REG 5 Horizontal Blank\n");
    err = img_read_register(0x005,&columnStart);
    printf("D[%d]-H[%02x]\n",columnsize);
	printf("REG 6 Vertical Blank\n");
    err = img_read_register(0x006,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 7 Output Control\n");
    err = img_read_register(0x007,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 8 Shutter Width Upper\n");
    err = img_read_register(0x008,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 9 Shutter Width Lower\n");
    err = img_read_register(0x009,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 10|Pixel Clock Control\n");
    err = img_read_register(0x00A,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 12|Shutter Delay\n");
    err = img_read_register(0x00C,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 16|PLL Control\n");
    err = img_read_register(0x010,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 17|PLL Config 1\n");
    err = img_read_register(0x011,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 18|PLL Config 2\n");
    err = img_read_register(0x012,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("30|Read Mode 1\n");
    err = img_read_register(0x01E,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("32|Read Mode 2\n");
    err = img_read_register(0x020,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 34|Row Address Mode\n");
    err = img_read_register(0x022,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("35|Column address mode\n");
    err = img_read_register(0x023,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("43|Green1 Gain\n");
    err = img_read_register(0x02B,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("44|Blue Gain\n");
    err = img_read_register(0x02C,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("45|Red Gain\n");
    err = img_read_register(0x02D,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("46|Green2 Gain\n");
    err = img_read_register(0x02E,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 53|Global Gain\n");
    err = img_read_register(0x035,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 75|Row black offse\n");
    err = img_read_register(0x04B,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 160|TestPatternControl\n");
    err = img_read_register(0x0A0,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("REG 161|TEST PATTERN GREEN\n");
    err = img_read_register(0x0A1,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("162|TEST PATTERN RED\n");
    err = img_read_register(0x0A2,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("163|TEST PATTERN BLUE\n");
    err = img_read_register(0x0A3,&vb);
    printf("D[%d]-H[%02x]\n",vb);
	printf("164|TESTPATBARWIDTH\n");
    err = img_read_register(0x0A4,&vb);
    printf("D[%d]-H[%02x]\n",vb);
    printf("--------------End Read\n");
}
u16 img(u16 read_address) {
    printf("contentC:");
    u16 contentC;
    contentC = uart_prompt_io();
    printf("\nread_address %x\n\n",read_address);
    printf("\ncontentC %x\n\n",contentC);
    s16 tableindex = -1;
    u32 i;
    u16 content;
    u8 address;
    u16 value = 1;
    u16 contentA;
    u16 contentB;
    printf("find_register 0x1 0x2 0x3 0x4 \n");
    switch(read_address) {
        case 0x0001:
            printf("contentA:");
            contentA = uart_prompt_io();
            printf("contentB:");
            contentB = uart_prompt_io();
            tableindex = find_register(contentA, contentB);
            if (tableindex == -1) {
                print("Register not found, probably reserved");
                return;
            }
            if (XIic_Send(IIC_BASE_ADDRESS, SENSOR_ADDRESS, &contentB, 1, XIIC_STOP) != 1) {
                print("Error: could not send address\n");
                //return;
            }
            for (i = tableindex; (i < tableindex+value) && (i < sizeof(registerEntries)/sizeof(struct registerEntry)); i++) {
                if (img_read_register(registerEntries[i].index, &content) != XST_SUCCESS) {
                    print("Read failed\n");
                    return;
                }
                printf("R%i: %d (0x%04x) %s\n", registerEntries[i].index, content, content ,  registerEntries[i].name );
            }
            break;
        case 0x0002:
            if (img_write_register(contentB, value) != XST_SUCCESS) {
                print("Write failed\n");
                return;
            }
            break;
        case 0x3:
            img_init();
            break;
        case 0x4: // test command for test patterns
            // disable BLC
            img_write_register(32, 0); // clears row BLC bit
            img_write_register(75, 0); // default BLC offset, clear
            img_write_register(160, ((address & 0x0F) << 3) | 1 ); // set test pattern as given
            break;
        default:
            print((char*)message);
            break;
    }
    print("\n");
}
//u32 read_in(u8 offset)
//{
//    return(Xil_In32((XPAR_PS_VIDEO_VIDEO_TEST_S_CONF_VIDEO_TEST_BASEADDR) + offset));
//}
//void write_in(u8 offset,u32 data)
//{
//    Xil_Out32((XPAR_PS_VIDEO_VIDEO_TEST_S_CONF_VIDEO_TEST_BASEADDR + offset),data);
//}
