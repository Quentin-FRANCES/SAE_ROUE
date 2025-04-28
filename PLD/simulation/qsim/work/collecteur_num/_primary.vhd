library verilog;
use verilog.vl_types.all;
entity collecteur_num is
    port(
        H_10KHZ         : in     vl_logic;
        VALID_HALL      : in     vl_logic;
        DIR             : in     vl_logic;
        PWM_IN          : in     vl_logic;
        HALL_FILTRE     : in     vl_logic_vector(2 downto 0);
        PWM_AH          : out    vl_logic;
        PWM_AL          : out    vl_logic;
        PWM_BH          : out    vl_logic;
        PWM_BL          : out    vl_logic;
        PWM_CH          : out    vl_logic;
        PWM_CL          : out    vl_logic
    );
end collecteur_num;
