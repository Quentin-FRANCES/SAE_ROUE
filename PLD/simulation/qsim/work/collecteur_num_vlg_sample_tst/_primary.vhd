library verilog;
use verilog.vl_types.all;
entity collecteur_num_vlg_sample_tst is
    port(
        DIR             : in     vl_logic;
        H_10KHZ         : in     vl_logic;
        HALL_FILTRE     : in     vl_logic_vector(2 downto 0);
        PWM_IN          : in     vl_logic;
        VALID_HALL      : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end collecteur_num_vlg_sample_tst;
