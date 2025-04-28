library verilog;
use verilog.vl_types.all;
entity FILTRE_vlg_sample_tst is
    port(
        H_10khz         : in     vl_logic;
        hall_A          : in     vl_logic;
        hall_B          : in     vl_logic;
        hall_C          : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end FILTRE_vlg_sample_tst;
