library verilog;
use verilog.vl_types.all;
entity CLK_10KHZ_vlg_sample_tst is
    port(
        H_2_5_MHZ       : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end CLK_10KHZ_vlg_sample_tst;
