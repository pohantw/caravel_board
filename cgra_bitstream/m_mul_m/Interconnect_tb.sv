`timescale 1ns/1ns
module Interconnect_tb;
    integer cnt, i;
    reg [31:0] sram_addr;
    reg [31:0] sram_data;

    reg clk;
    reg reset;
    reg [3:0] stall ;
    wire [31:0] read_config_data ;
    reg [31:0] config_config_addr ;
    reg [31:0] config_config_data ;
    reg [0:0] config_read ;
    reg [0:0] config_write ;
    reg [0:0] glb2io_1_X00_Y00 ;
    wire [0:0] io2glb_1_X00_Y00 ;
    reg [15:0] glb2io_16_X00_Y00 ;
    wire [15:0] io2glb_16_X00_Y00 ;
    reg [0:0] glb2io_1_X01_Y00 ;
    wire [0:0] io2glb_1_X01_Y00 ;
    reg [15:0] glb2io_16_X01_Y00 ;
    wire [15:0] io2glb_16_X01_Y00 ;
    reg [0:0] glb2io_1_X02_Y00 ;
    wire [0:0] io2glb_1_X02_Y00 ;
    reg [15:0] glb2io_16_X02_Y00 ;
    wire [15:0] io2glb_16_X02_Y00 ;
    reg [0:0] glb2io_1_X03_Y00 ;
    wire [0:0] io2glb_1_X03_Y00 ;
    reg [15:0] glb2io_16_X03_Y00 ;
    wire [15:0] io2glb_16_X03_Y00 ;

    integer file_bs, scan_file;
    reg [31:0] scan_addr, scan_data;

    integer gold_cnt;
    integer chk_cnt;
    reg [15:0] rtl_mem1 [0:511];
    reg [15:0] rtl, gold;
    reg [31:0] mem_data;
    integer error = 0;


    localparam PERIOD = 10;
    reg [15:0] flush_xy;
    reg [15:0] mem0_xy;
    reg [15:0] mem1_xy;

    initial begin
        clk = 0;
        while(1) begin
            #(PERIOD/2) clk = ~clk;
        end
    end

    Interconnect #(
        
    ) dut (
        .clk(clk),
        .read_config_data(read_config_data),
        .reset(reset),
        .stall(stall),
        .config_0_config_addr(config_config_addr),
        .config_0_config_data(config_config_data),
        .config_0_read(config_read),
        .config_0_write(config_write),
        .config_1_config_addr(config_config_addr),
        .config_1_config_data(config_config_data),
        .config_1_read(config_read),
        .config_1_write(config_write),
        .config_2_config_addr(config_config_addr),
        .config_2_config_data(config_config_data),
        .config_2_read(config_read),
        .config_2_write(config_write),
        .config_3_config_addr(config_config_addr),
        .config_3_config_data(config_config_data),
        .config_3_read(config_read),
        .config_3_write(config_write),
        .glb2io_1_X00_Y00(glb2io_1_X00_Y00),
        .io2glb_1_X00_Y00(io2glb_1_X00_Y00),
        .glb2io_16_X00_Y00(glb2io_16_X00_Y00),
        .io2glb_16_X00_Y00(io2glb_16_X00_Y00),
        .glb2io_1_X01_Y00(glb2io_1_X01_Y00),
        .io2glb_1_X01_Y00(io2glb_1_X01_Y00),
        .glb2io_16_X01_Y00(glb2io_16_X01_Y00),
        .io2glb_16_X01_Y00(io2glb_16_X01_Y00),
        .glb2io_1_X02_Y00(glb2io_1_X02_Y00),
        .io2glb_1_X02_Y00(io2glb_1_X02_Y00),
        .glb2io_16_X02_Y00(glb2io_16_X02_Y00),
        .io2glb_16_X02_Y00(io2glb_16_X02_Y00),
        .glb2io_1_X03_Y00(glb2io_1_X03_Y00),
        .io2glb_1_X03_Y00(io2glb_1_X03_Y00),
        .glb2io_16_X03_Y00(glb2io_16_X03_Y00),
        .io2glb_16_X03_Y00(io2glb_16_X03_Y00)
    );

    initial begin
        // set the x, y coordinate from design.place
        flush_xy = 16'h03_02;
        mem0_xy = 16'h02_01;
        mem1_xy = 16'h01_01;

        reset <= 1'b0;
        stall <= 4'd0;
        config_config_addr <= 32'd0;
        config_config_data <= 32'd0;
        config_read <= 1'd0;
        config_write <= 1'd0;
        glb2io_1_X00_Y00 <= 1'd0;
        glb2io_16_X00_Y00 <= 16'd0;
        glb2io_1_X01_Y00 <= 1'd0;
        glb2io_16_X01_Y00 <= 16'd0;
        glb2io_1_X02_Y00 <= 1'd0;
        glb2io_16_X02_Y00 <= 16'd0;
        glb2io_1_X03_Y00 <= 1'd0;
        glb2io_16_X03_Y00 <= 16'd0;
        reset <= 1'b1;
        #(PERIOD*3);
        reset <= 1'b0;

        // stall the cgra to start configuration
        $display("stall the cgra to start configuration");
        stall <= 4'hf;

        // write the config
        file_bs = $fopen("/aha/Halide-to-Hardware/apps/hardware_benchmarks/tests/simple_mem/bin/simple_mem.bs", "r");
        if (file_bs == 0) begin
            $display("Bitstream file not found!");
            $finish;
        end
        $display("start writing bitstream...");
        while (!$feof(file_bs)) begin
            scan_file = $fscanf(file_bs, "%h %h\n", scan_addr, scan_data);
            // $display("addr = %h, data = %h", scan_addr, scan_data);
            @(negedge clk);
            config_config_addr <= scan_addr;
            config_config_data <= scan_data;
            config_read <= 1'd0;
            config_write <= 1'd1;
            #(PERIOD);
            config_write <= 1'd0;
        end
        $fclose(file_bs);
        $display("finish writing bitstream");

        // read the config to check
        file_bs = $fopen("/aha/Halide-to-Hardware/apps/hardware_benchmarks/tests/simple_mem/bin/simple_mem.bs", "r");
        $display("start bitstream readback check...");
        while (!$feof(file_bs)) begin
            scan_file = $fscanf(file_bs, "%h %h\n", scan_addr, scan_data);
            @(negedge clk);
            config_config_addr <= scan_addr;
            config_read <= 1'd1;
            config_write <= 1'd0;
            #(PERIOD);
            if (read_config_data != scan_data) begin
                $display("config data readback mismatch @ addr=%h, rtl=%h, gold=%h", scan_addr, read_config_data, scan_data);
                $finish;
            end
            config_read <= 1'd0;
        end
        $fclose(file_bs);
        $display("bitstream readback check passed");

        // de-assert flush (otherwise cannot prefill)
        $display("de-assert flush (otherwise cannot prefill)");
        @(negedge clk);
        config_config_addr <= {16'h0000, flush_xy};
        config_config_data <= 32'h001C_0000;
        config_read <= 1'd0;
        config_write <= 1'd1;
        #(PERIOD);
        config_write <= 1'd0;

        // pre-fill the memory
        $display("pre-fill the memory");
        cnt = 0;
        while(cnt<256) begin
            for (i=0; i<2; i=i+1) begin
                @(negedge clk);
                sram_addr = {cnt[7:0], 8'h01, mem0_xy};
                // sram_data = {24'd0, cnt[6:0], 1'b0} + i + 256;
                sram_data = {16'h0000, 16'h0099};
                config_config_addr <= sram_addr;
                config_config_data <= sram_data;
                config_write <= 1'd1;
                #(PERIOD);
                config_write <= 1'd0;
            end
            cnt = cnt + 1;
        end
        config_write <= 1'd0;

        // unstall the CGRA to start application
        $display("unstall the CGRA to start application");
        stall <= 4'd0;

        // create flush pulse
        $display("create flush pulse");
        @(negedge clk);
        config_config_addr <= {16'h0000, flush_xy};
        config_config_data <= 32'h001C_7E00;
        config_write <= 1'd1;
        #(PERIOD*2); 
        config_write <= 1'd0;
        @(negedge clk);
        config_config_addr <= {16'h0000, flush_xy};
        config_config_data <= 32'h001C_0000;
        config_write <= 1'd1;
        #(PERIOD);
        config_write <= 1'd0;

        // wait for application done inside
        $display("wait for application done inside...");
        #(PERIOD*2000);

        // stall the cgra to read from sram
        $display("stall the cgra to read from sram");
        stall <= 4'hf;
        gold_cnt = 0;
        chk_cnt = 0;

        // read back the sram via config
        $display("read back the sram via config");
        while(gold_cnt <= 512) begin
            @(negedge clk);
            config_config_addr <= {gold_cnt[8:1], 8'h01, mem1_xy};
            config_read <= 1'd1;
            #(PERIOD*5);
            if (gold_cnt > 0) begin
                if (chk_cnt[0]) begin
                    rtl_mem1[chk_cnt-1] = read_config_data[15:0];
                end
                else begin
                    rtl_mem1[chk_cnt+1] = read_config_data[15:0];
                end
                chk_cnt = chk_cnt + 1;
            end
            gold_cnt = gold_cnt + 1;
        end

        // check the contents
        $display("comparing...");
        for (gold_cnt=0; gold_cnt<512; gold_cnt=gold_cnt+1) begin
            gold = 32'h0000_0099 * 3;
            rtl = rtl_mem1[gold_cnt];
            if (rtl !== gold) begin
                $display("fail at gold[%0d] = %h, rtl = %h", gold_cnt, gold, rtl);
                error = error + 1;
            end
        end
        if (error > 0) $display("\nxxx Comparison Fail, total error = %0d xxx\n", error);
        else           $display("\n=== Comparison Pass ===\n");

        $finish;
    end

endmodule
