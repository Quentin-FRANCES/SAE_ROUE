library verilog;
use verilog.vl_types.all;
entity CLK_10KHZ_vlg_check_tst is
    port(
        H_10KHZ         : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end CLK_10KHZ_vlg_check_tst;
