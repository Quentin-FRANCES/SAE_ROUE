library verilog;
use verilog.vl_types.all;
entity PWM_vlg_sample_tst is
    port(
        H_2_5_MHZ       : in     vl_logic;
        REF_PWM         : in     vl_logic_vector(8 downto 0);
        VALID_PWM       : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end PWM_vlg_sample_tst;
