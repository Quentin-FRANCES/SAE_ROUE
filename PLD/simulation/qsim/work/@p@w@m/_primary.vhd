library verilog;
use verilog.vl_types.all;
entity PWM is
    port(
        H_2_5_MHZ       : in     vl_logic;
        VALID_PWM       : in     vl_logic;
        REF_PWM         : in     vl_logic_vector(7 downto 0);
        H_10KHZ         : out    vl_logic;
        PWM_OUT         : out    vl_logic
    );
end PWM;
