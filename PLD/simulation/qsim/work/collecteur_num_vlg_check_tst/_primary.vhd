library verilog;
use verilog.vl_types.all;
entity collecteur_num_vlg_check_tst is
    port(
        PWM_AH          : in     vl_logic;
        PWM_AL          : in     vl_logic;
        PWM_BH          : in     vl_logic;
        PWM_BL          : in     vl_logic;
        PWM_CH          : in     vl_logic;
        PWM_CL          : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end collecteur_num_vlg_check_tst;
