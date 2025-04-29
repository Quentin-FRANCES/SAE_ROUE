library verilog;
use verilog.vl_types.all;
entity CLK_10KHZ is
    port(
        H_2_5_MHZ       : in     vl_logic;
        H_10KHZ         : out    vl_logic
    );
end CLK_10KHZ;
