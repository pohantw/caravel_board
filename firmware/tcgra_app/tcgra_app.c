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

// flush hack
#define flush_pe_addr 0x00000302 // change me according to design.place
#define flush_true 0x001C7E00
#define flush_false 0x001C0000

// source SRAM (pre-program)
// {8b sram address}{8'd1: feature address for sram}{x addr}{y addr}
#define src_sram_addr_base 0x00010201 // change me according to design.place
#define dst_sram_addr_base 0x00010101 // change me according to design.place

int CGRA_load_bs(int rb_chk);
void led_signal_pass(uint32_t error, uint32_t last_rtl);
void led_signal_fail(uint32_t error, uint32_t last_rtl);
void led_signal_fail_cfg();
void uart_message(int status);
void flush_set(uint32_t value);

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
    // Enable UART transmission
    reg_mprj_io_6 = 0x7ff;
    reg_uart_clkdiv = 1042;
    reg_uart_enable = 1;

    // perform the transfer to configure all I/Os
    reg_mprj_datal = 0x00000000; // set mprj_io[31:0] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_datah = 0x00000000; // set mprj_io[37:32] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    // Enable GPIO (all output, ena = 0)
	reg_gpio_ena = 0x0;
	reg_gpio_pu = 0x0;
	reg_gpio_pd = 0x0;
	reg_gpio_data = 0x1;

    // [CGRA] stall the CGRA for configuration
    reg_mprj_stall = 0xF;

    // [CGRA] load bitstream
    int status = 0;
    status = CGRA_load_bs(1);
    if (status == 1){
        led_signal_fail_cfg();
    }

    // de-assert flush
    flush_set(flush_false);

    // pre-program sram
    uint32_t i;
    uint32_t ii;
    for (i=0; i<512; i++) {
        ii = i>>1; // same address write twice
        ii = ii<<24; // shift the address to [31:24]
        ii = src_sram_addr_base | ii;
        reg_mprj_cfg_addr = ii;
        reg_mprj_cfg_wdata = 0x00000099; // use same data
        reg_mprj_cfg_write = 1;
    }

    // unstall the cgra
    reg_mprj_stall = 0x0;

    // create flush
    flush_set(flush_true);
    flush_set(flush_false);

    // wait some time
    for(i=0; i<3000; i++);

    // stall cgra
    reg_mprj_stall = 0xF;

    // check the result SRAM
    uint32_t rtl, k;
    uint32_t error=0;
    for (i=0;i<256;i++){
        ii = i<<24;
        ii = dst_sram_addr_base | ii;
        reg_mprj_cfg_addr = ii;
        reg_mprj_cfg_read = 1;
        for(k=0; k<50; k++);
        rtl = reg_mprj_cfg_rdata;
        reg_mprj_cfg_read = 0;
        // if (rtl != (0x00000099)){
        // if (rtl != (0x00000099+1)){
        // if (rtl != (0x00000099*3)){
        // if (rtl != (0x00000099>>3)){
        if (rtl != (0x00000099^0x00000013)){
            status = 1;
            error++;
        }
    }

    // Use LED blinking pattern to show CGRA program status
    if (status==0) {
        led_signal_pass(error, rtl);
    }
    else {
        // led_signal_fail(error);
        led_signal_fail(error, rtl);
    }

}

void led_signal_pass(uint32_t error, uint32_t last_rtl)
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
        print_hex(error, 4);
        print_hex(last_rtl, 4);

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

void led_signal_fail(uint32_t error, uint32_t last_rtl)
{
    int i, j;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(error, 4);
        print_hex(last_rtl, 4);
        // continuous fast blink
        for (i=0; i<15; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 2000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 2000; j++);
        }
	}
}

void led_signal_fail_cfg()
{
    int i, j;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(0x9999, 4);
        // continuous fast blink
        for (i=0; i<15; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 500; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 500; j++);
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

void flush_set(uint32_t value)
{
    reg_mprj_cfg_addr = flush_pe_addr;
    reg_mprj_cfg_wdata = value;
    reg_mprj_cfg_write = 1;
}

int CGRA_load_bs(int rb_chk)
{
    // This is the bitstream file of : simple_mem
    // Auto generated at 11/29/2022 02:03:58
    uint32_t rtl;
    int fail = 0;

    // write the bitstream
    reg_mprj_cfg_addr = 0x01090301;
    reg_mprj_cfg_wdata = 0x00028008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_wdata = 0x00001010;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090201;
    reg_mprj_cfg_wdata = 0x08040000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_wdata = 0x00014000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_wdata = 0x00050000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060302;
    reg_mprj_cfg_wdata = 0x0000C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020301;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_wdata = 0x00000081;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_wdata = 0x00018000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_wdata = 0x0000000B;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_wdata = 0x00003000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0201;
    reg_mprj_cfg_wdata = 0x00200000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00040202;
    reg_mprj_cfg_wdata = 0x00000008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_wdata = 0x0000C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050202;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_wdata = 0xC8000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4F000201;
    reg_mprj_cfg_wdata = 0x00100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x38000201;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2D000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x30000201;
    reg_mprj_cfg_wdata = 0x000FE100;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x37000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x49000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x46000201;
    reg_mprj_cfg_wdata = 0x00000110;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x40000201;
    reg_mprj_cfg_wdata = 0x000000FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x48000201;
    reg_mprj_cfg_wdata = 0x00010002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x3F000201;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x47000201;
    reg_mprj_cfg_wdata = 0x01000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000201;
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
    reg_mprj_cfg_addr = 0x05000301;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000301;
    reg_mprj_cfg_wdata = 0x0000003C;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x04000301;
    reg_mprj_cfg_wdata = 0x00010080;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000301;
    reg_mprj_cfg_wdata = 0x0000F880;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x03000301;
    reg_mprj_cfg_wdata = 0x00010000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000301;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x00800014;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000202;
    reg_mprj_cfg_wdata = 0x48000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000202;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000203;
    reg_mprj_cfg_wdata = 0x48004C00;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000203;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000302;
    reg_mprj_cfg_wdata = 0x001C7E00;
    reg_mprj_cfg_write = 1;

    // readback check
    reg_mprj_cfg_addr = 0x01090301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00028008) fail = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00001010) fail = 1;
    reg_mprj_cfg_addr = 0x00090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x08040000) fail = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00014000) fail = 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00050000) fail = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) fail = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) fail = 1;
    reg_mprj_cfg_addr = 0x00060302;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000C000) fail = 1;
    reg_mprj_cfg_addr = 0x00020301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) fail = 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000081) fail = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00018000) fail = 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000000B) fail = 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00003000) fail = 1;
    reg_mprj_cfg_addr = 0x000A0201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00200000) fail = 1;
    reg_mprj_cfg_addr = 0x00040202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000008) fail = 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000C000) fail = 1;
    reg_mprj_cfg_addr = 0x00050202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xC8000000) fail = 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x4F000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00100000) fail = 1;
    reg_mprj_cfg_addr = 0x38000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x2D000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x30000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000FE100) fail = 1;
    reg_mprj_cfg_addr = 0x37000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x49000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x46000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000110) fail = 1;
    reg_mprj_cfg_addr = 0x40000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000000FE) fail = 1;
    reg_mprj_cfg_addr = 0x48000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010002) fail = 1;
    reg_mprj_cfg_addr = 0x3F000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x47000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01000000) fail = 1;
    reg_mprj_cfg_addr = 0x51000201;
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
    reg_mprj_cfg_addr = 0x05000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) fail = 1;
    reg_mprj_cfg_addr = 0x01000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000003C) fail = 1;
    reg_mprj_cfg_addr = 0x04000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010080) fail = 1;
    reg_mprj_cfg_addr = 0x00000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000F880) fail = 1;
    reg_mprj_cfg_addr = 0x03000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010000) fail = 1;
    reg_mprj_cfg_addr = 0x51000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) fail = 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00800014) fail = 1;
    reg_mprj_cfg_addr = 0x01000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000200) fail = 1;
    reg_mprj_cfg_addr = 0x02000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x01000203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48004C00) fail = 1;
    reg_mprj_cfg_addr = 0x02000203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) fail = 1;
    reg_mprj_cfg_addr = 0x00000302;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x001C7E00) fail = 1;

    reg_mprj_cfg_read = 0;
    return fail;

}
