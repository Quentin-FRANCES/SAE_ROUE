library verilog;
use verilog.vl_types.all;
entity FILTRE is
    port(
        valid_hall      : out    vl_logic;
        hall_C          : in     vl_logic;
        hall_B          : in     vl_logic;
        hall_A          : in     vl_logic;
        H_10khz         : in     vl_logic;
        hall_filtre     : out    vl_logic_vector(2 downto 0)
    );
end FILTRE;
