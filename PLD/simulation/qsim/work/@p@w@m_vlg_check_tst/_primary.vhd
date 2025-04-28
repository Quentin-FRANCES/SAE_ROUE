library verilog;
use verilog.vl_types.all;
entity PWM_vlg_check_tst is
    port(
        H_10KHZ         : in     vl_logic;
        PWM_OUT         : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end PWM_vlg_check_tst;
