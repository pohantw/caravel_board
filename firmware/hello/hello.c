#include "../defs_mpw-two-mfix.h"
#include "../print_io.h"

// CGRA wishbone control register address
#define reg_mprj_cfg_addr  (*(volatile uint32_t*)0x30000000)
#define reg_mprj_cfg_wdata (*(volatile uint32_t*)0x30000004)
#define reg_mprj_cfg_rdata (*(volatile uint32_t*)0x30000008)
#define reg_mprj_cfg_write (*(volatile uint32_t*)0x3000000C)
#define reg_mprj_cfg_read  (*(volatile uint32_t*)0x30000010)
#define reg_mprj_stall     (*(volatile uint32_t*)0x30000014)
#define reg_mprj_message   (*(volatile uint32_t*)0x30000018)

int CGRA_load_bs(int rb_chk);
void led_signal_pass(uint32_t data);
void led_signal_fail(uint32_t error);
void uart_message(int status);

// --------------------------------------------------------
// Firmware routines
// --------------------------------------------------------
void set_gpio(int pin)
{
    (volatile uint32_t) ((reg_mprj_datal) |= pin);
}

void clear_gpio(int pin)
{
    (volatile uint32_t) ((reg_mprj_datal) &= ~(pin));
}

void main()
{
	// Important note!  GPIOs 1 to 4 have a reversed bit for
    // management disable that requires swapping the values
    // for "USER" and "MGMT" for these four pins.  Normally, use
    // of these pins for user project purposes is discouraged, so
    // it should not be a major issue.
    // --------------------------------------------------
    // #define GPIO_MODE_USER_STD_INPUT_NOPULL    0x0403
    // #define GPIO_MODE_USER_STD_INPUT_PULLDOWN  0x0803
    // #define GPIO_MODE_USER_STD_INPUT_PULLUP    0x0c03
    // #define GPIO_MODE_USER_STD_OUTPUT          0x1809
    // #define GPIO_MODE_USER_STD_BIDIRECTIONAL   0x1803
    // #define GPIO_MODE_USER_STD_OUT_MONITORED   0x1803
    // #define GPIO_MODE_USER_STD_ANALOG          0x000b
    // --------------------------------------------------
    // #define GPIO_MODE_MGMT_STD_INPUT_NOPULL    0x0402
    // #define GPIO_MODE_MGMT_STD_INPUT_PULLDOWN  0x0802
    // #define GPIO_MODE_MGMT_STD_INPUT_PULLUP    0x0c02
    // #define GPIO_MODE_MGMT_STD_OUTPUT          0x1808
    // #define GPIO_MODE_MGMT_STD_BIDIRECTIONAL   0x1802
    // #define GPIO_MODE_MGMT_STD_OUT_MONITORED   0x1802
    // #define GPIO_MODE_MGMT_STD_ANALOG          0x000a
    // --------------------------------------------------
    //reg_mprj_io_0  = 0x1c03; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[0]
    //reg_mprj_io_1  = 0x1e01; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[1]
    //reg_mprj_io_2  = 0x0f00; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[2]
    //reg_mprj_io_3  = 0x0780; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[3]
    //reg_mprj_io_4  = 0x03c0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[4]
    //reg_mprj_io_5  = 0x01e0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[5]
    //reg_mprj_io_6  = 0x00f0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[6]
    //reg_mprj_io_7  = 0x0078; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[7]
    //reg_mprj_io_8  = 0x003c; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[8]
    //reg_mprj_io_9  = 0x001e; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[9]
    //reg_mprj_io_10 = 0x100f; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[10]
    //reg_mprj_io_11 = 0x1807; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[11]
    //reg_mprj_io_12 = 0x1c03; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[12]
    //reg_mprj_io_13 = 0x1e01; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[13]
    //reg_mprj_io_14 = 0x0f00; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[14]
    //reg_mprj_io_15 = 0x1380; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[15]
    //reg_mprj_io_16 = 0x0980; // [slippage fix-14USER] Input - glb2io_1_X00_Y00
    //reg_mprj_io_17 = 0x00c0; // [slippage fix-14USER] Output - io2glb_16_X01_Y00[0]
    //reg_mprj_io_18 = 0x0000; // [slippage fix-14USER] Output - io2glb_16_X01_Y00[1]

    //reg_mprj_io_0  = 0x0403; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[0]
    //reg_mprj_io_1  = 0x1201; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[1]
    //reg_mprj_io_2  = 0x0900; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[2]
    //reg_mprj_io_3  = 0x0480; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[3]
    //reg_mprj_io_4  = 0x03c0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[4]
    //reg_mprj_io_5  = 0x01e0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[5]
    //reg_mprj_io_6  = 0x00f0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[6]
    //reg_mprj_io_7  = 0x0078; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[7]
    //reg_mprj_io_8  = 0x003c; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[8]
    //reg_mprj_io_9  = 0x001e; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[9]
    //reg_mprj_io_10 = 0x100f; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[10]
    //reg_mprj_io_11 = 0x1807; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[11]
    //reg_mprj_io_12 = 0x1c03; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[12]
    //reg_mprj_io_13 = 0x1e01; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[13]
    //reg_mprj_io_14 = 0x0f00; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[14]
    //reg_mprj_io_15 = 0x1380; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[15]
    //reg_mprj_io_16 = 0x0980; // [slippage fix-14MGMT] Input - glb2io_1_X00_Y00
    //reg_mprj_io_17 = 0x00c0; // [slippage fix-14MGMT] Output - io2glb_16_X01_Y00[0]
    //reg_mprj_io_18 = 0x0000; // [slippage fix-14MGMT] Output - io2glb_16_X01_Y00[1]
    //reg_mprj_io_19 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[2]
    //reg_mprj_io_20 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[3]
    //reg_mprj_io_21 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[4]
    //reg_mprj_io_22 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[5]
    //reg_mprj_io_23 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[6]
    //reg_mprj_io_24 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[7]
    //reg_mprj_io_25 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[8]
    //reg_mprj_io_26 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[9]
    //reg_mprj_io_27 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[10]
    //reg_mprj_io_28 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[11]
    //reg_mprj_io_29 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[12]
    //reg_mprj_io_30 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[13]
    //reg_mprj_io_31 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[14]
    //reg_mprj_io_32 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[15]
    //reg_mprj_io_33 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_1_X01_Y00
    //reg_mprj_io_34 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_clk
    //reg_mprj_io_35 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_reset
    reg_mprj_io_36 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[0]
    reg_mprj_io_37 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[1]
    
    // Enable UART transmission
    reg_mprj_io_6 = 0x7ff;
    reg_uart_clkdiv = 1042;
    reg_uart_enable = 1;

    // perform the transfer to configure all I/Os
    reg_mprj_datal = 0x00000000; // set mprj_io[31:0] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_datah = 0x00000000; // set mprj_io[37:32] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    // Configure LA probes as outputs from the cpu
    // reg_la0_oenb = reg_la0_iena = 0xFFFFFFFF;    // [31:0]
    // reg_la1_oenb = reg_la1_iena = 0xFFFFFFFF;    // [63:32]
    // reg_la2_oenb = reg_la2_iena = 0xFFFFFFFF;    // [95:64]
    // reg_la3_oenb = reg_la3_iena = 0xFFFFFFFF;    // [127:96]

    // configure the IO direction through LA probe
    // 0=output, 1=input
    // reg_la0_data = 0x0001FFFF; // 0000_0000_0000_0001_1111_1111_1111_1111
    // reg_la1_data = 0x0000000C; // 0000_0000_0000_0000_0000_0000_0000_1100
    // reg_la2_data = 0x00000000;
    // reg_la3_data = 0x00000000; // note: la_data[96] is the clk/reset select signal

    // Enable GPIO (all output, ena = 0)
	reg_gpio_ena = 0x0;
	reg_gpio_pu = 0x0;
	reg_gpio_pd = 0x0;
	reg_gpio_data = 0x1;

    // configure again 
    // reg_mprj_io_0  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[0]
    // reg_mprj_io_1  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[1] swap USER to MGMT
    // reg_mprj_io_2  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[2] swap USER to MGMT
    // reg_mprj_io_3  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[3] swap USER to MGMT
    // reg_mprj_io_4  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[4] swap USER to MGMT
    // reg_mprj_io_5  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[5]
    // reg_mprj_io_6  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[6]
    // reg_mprj_io_7  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[7]
    // reg_mprj_io_8  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[8]
    // reg_mprj_io_9  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[9]
    // reg_mprj_io_10 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[10]
    // reg_mprj_io_11 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[11]
    // reg_mprj_io_12 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[12]
    // reg_mprj_io_13 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[13]
    // reg_mprj_io_14 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[14]
    // reg_mprj_io_15 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[15]
    // reg_mprj_io_16 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_1_X00_Y00
    // reg_mprj_io_17 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[0]
    // reg_mprj_io_18 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[1]
    // reg_mprj_io_19 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[2]
    // reg_mprj_io_20 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[3]
    // reg_mprj_io_21 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[4]
    // reg_mprj_io_22 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[5]
    // reg_mprj_io_23 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[6]
    // reg_mprj_io_24 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[7]
    // reg_mprj_io_25 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[8]
    // reg_mprj_io_26 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[9]
    // reg_mprj_io_27 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[10]
    // reg_mprj_io_28 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[11]
    // reg_mprj_io_29 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[12]
    // reg_mprj_io_30 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[13]
    // reg_mprj_io_31 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[14]
    // reg_mprj_io_32 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[15]
    // reg_mprj_io_33 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_1_X01_Y00
    // reg_mprj_io_34 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_clk
    // reg_mprj_io_35 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_reset
    // reg_mprj_io_36 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[0]
    // reg_mprj_io_37 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[1]

    // [CGRA] stall the CGRA for configuration
    reg_mprj_stall = 0xF;

    // [CGRA] set message
    // reg_mprj_message = 2;

    // [CGRA] load bitstream
    // input
    //     0: no readback check (always success)
    //     1: with readback check
    // output
    //     0: success
    //     1: fail
    int status = 0;
    status = CGRA_load_bs(1);

    // de-assert flush
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x001C0000;
    reg_mprj_cfg_write = 1;

    // pre-program sram
    uint32_t i;
    uint32_t ii;
    for (i=0; i<512; i++) {
        ii = i>>1;
        ii = ii<<24;
        ii = 0x00010001 | ii;
        reg_mprj_cfg_addr = ii;
        reg_mprj_cfg_wdata = 0x00000099;
        reg_mprj_cfg_write = 1;
    }

    // unstall the cgra
    reg_mprj_stall = 0x0;

    // assert flush
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x001C7E00;
    reg_mprj_cfg_write = 1;
    // de-assert flush (CGRA app start)
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x001C0000;
    reg_mprj_cfg_write = 1;

    // wait some time
    for(i=0; i<3000; i++);

    // stall cgra
    reg_mprj_stall = 0xF;

    // check the result SRAM
    uint32_t rtl, k;
    uint32_t error=0;
    for (i=0;i<256;i++){
        ii = i<<24;
        ii = ii | 0x00010101;
        reg_mprj_cfg_addr = ii;
        reg_mprj_cfg_read = 1;
        for(k=0; k<50; k++);
        rtl = reg_mprj_cfg_rdata;
        reg_mprj_cfg_read = 0;
        if (rtl != 0x00000099){
            status = 1;
            error++;
        }
    }

    // Use LED blinking pattern to show CGRA program status
    if (status==0) {
        led_signal_pass(rtl);
    }
    else {
        led_signal_fail(error);
    }

}

void led_signal_pass(uint32_t data)
{
    char *msg[] = {
        " [CGRA] Success!!!   ",
        " [CGRA] Fail...      "
    };
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
    int i, j;
    int N = 5;

	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(data, 4);

        for (j = 0; j < 30000; j++);
        // a slow bink
        reg_gpio_data = 0x0;
        for (j = 0; j < 10000; j++);
        reg_gpio_data = 0x1;
        for (j = 0; j < 10000; j++);

        // followed by N fast blink
        for (i=0; i<N; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 1000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 1000; j++);
        }
	}
}

void led_signal_fail(uint32_t error)
{
    int i, j;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(error, 4);
        // continuous fast blink
        for (i=0; i<15; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 2000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 2000; j++);
        }
	}
}

void uart_message(int status)
{
    char *msg[] = {
        " [CGRA] Success!!!   ",
        " [CGRA] Fail...      "
    };
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
    print("|"); putchar(0x2d); // clear screen
    print(msg[status]);
}

int CGRA_load_bs(int rb_chk)
{
    // write the bitstream
    reg_mprj_cfg_addr = 0x01090101;
    reg_mprj_cfg_wdata = 0x00000010;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_wdata = 0x00050000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020001;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_wdata = 0x00005008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060202;
    reg_mprj_cfg_wdata = 0x0000C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_wdata = 0x00014000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0001;
    reg_mprj_cfg_wdata = 0x03240000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_wdata = 0x00003000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00040002;
    reg_mprj_cfg_wdata = 0x00000008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_wdata = 0x00000007;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070102;
    reg_mprj_cfg_wdata = 0x4014C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_wdata = 0x000D0000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070002;
    reg_mprj_cfg_wdata = 0x04340000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050102;
    reg_mprj_cfg_wdata = 0x00000007;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090001;
    reg_mprj_cfg_wdata = 0x00040000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_wdata = 0x00000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x05000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000201;
    reg_mprj_cfg_wdata = 0x0000003C;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x04000201;
    reg_mprj_cfg_wdata = 0x00010080;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000201;
    reg_mprj_cfg_wdata = 0x0000F880;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x03000201;
    reg_mprj_cfg_wdata = 0x00010000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4F000001;
    reg_mprj_cfg_wdata = 0x00100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x38000001;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2D000001;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x30000001;
    reg_mprj_cfg_wdata = 0x000FE100;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x37000001;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x49000001;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x46000001;
    reg_mprj_cfg_wdata = 0x00000110;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x40000001;
    reg_mprj_cfg_wdata = 0x000000FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x48000001;
    reg_mprj_cfg_wdata = 0x00010002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x3F000001;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x47000001;
    reg_mprj_cfg_wdata = 0x01000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000001;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1B000101;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x29000101;
    reg_mprj_cfg_wdata = 0x00000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x22000101;
    reg_mprj_cfg_wdata = 0x00000FE1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x07000101;
    reg_mprj_cfg_wdata = 0x00100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1A000101;
    reg_mprj_cfg_wdata = 0x00000009;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0C000101;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x09000101;
    reg_mprj_cfg_wdata = 0x00011000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x14000101;
    reg_mprj_cfg_wdata = 0x000000FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x13000101;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0B000101;
    reg_mprj_cfg_wdata = 0x00000005;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000002;
    reg_mprj_cfg_wdata = 0x00800000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000002;
    reg_mprj_cfg_wdata = 0x48000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000002;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000102;
    reg_mprj_cfg_wdata = 0x00800000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000102;
    reg_mprj_cfg_wdata = 0x48000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000102;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x001C7E00;
    reg_mprj_cfg_write = 1;

    // readback check
    int fail = 0;
    int rtl;
    reg_mprj_cfg_addr = 0x01090101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000010) fail = 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00050000) fail = 1;
    reg_mprj_cfg_addr = 0x00020001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) fail = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00005008) fail = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) fail = 1;
    reg_mprj_cfg_addr = 0x00060202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000C000) fail = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00014000) fail = 1;
    reg_mprj_cfg_addr = 0x000A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x03240000) fail = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00003000) fail = 1;
    reg_mprj_cfg_addr = 0x00040002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000008) fail = 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000007) fail = 1;
    reg_mprj_cfg_addr = 0x00070102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x4014C000) fail = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000D0000) fail = 1;
    reg_mprj_cfg_addr = 0x00070002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x04340000) fail = 1;
    reg_mprj_cfg_addr = 0x00050102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000007) fail = 1;
    reg_mprj_cfg_addr = 0x00090001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00040000) fail = 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000200) fail = 1;
    reg_mprj_cfg_addr = 0x05000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x01000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000003C) fail = 1;
    reg_mprj_cfg_addr = 0x04000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010080) fail = 1;
    reg_mprj_cfg_addr = 0x00000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000F880) fail = 1;
    reg_mprj_cfg_addr = 0x03000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010000) fail = 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) fail = 1;
    reg_mprj_cfg_addr = 0x4F000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00100000) fail = 1;
    reg_mprj_cfg_addr = 0x38000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x2D000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x30000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000FE100) fail = 1;
    reg_mprj_cfg_addr = 0x37000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x49000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x46000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000110) fail = 1;
    reg_mprj_cfg_addr = 0x40000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000000FE) fail = 1;
    reg_mprj_cfg_addr = 0x48000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010002) fail = 1;
    reg_mprj_cfg_addr = 0x3F000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x47000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01000000) fail = 1;
    reg_mprj_cfg_addr = 0x51000001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) fail = 1;
    reg_mprj_cfg_addr = 0x1B000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x29000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000200) fail = 1;
    reg_mprj_cfg_addr = 0x22000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000FE1) fail = 1;
    reg_mprj_cfg_addr = 0x07000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00100000) fail = 1;
    reg_mprj_cfg_addr = 0x1A000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000009) fail = 1;
    reg_mprj_cfg_addr = 0x0C000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) fail = 1;
    reg_mprj_cfg_addr = 0x09000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00011000) fail = 1;
    reg_mprj_cfg_addr = 0x14000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000000FE) fail = 1;
    reg_mprj_cfg_addr = 0x13000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x0B000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000005) fail = 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) fail = 1;
    reg_mprj_cfg_addr = 0x00000002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00800000) fail = 1;
    reg_mprj_cfg_addr = 0x01000002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000200) fail = 1;
    reg_mprj_cfg_addr = 0x02000002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x00000102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00800000) fail = 1;
    reg_mprj_cfg_addr = 0x01000102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000200) fail = 1;
    reg_mprj_cfg_addr = 0x02000102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x001C7E00) fail = 1;
    return fail;
}
