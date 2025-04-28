library verilog;
use verilog.vl_types.all;
entity FILTRE_vlg_check_tst is
    port(
        hall_filtre     : in     vl_logic_vector(2 downto 0);
        valid_hall      : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end FILTRE_vlg_check_tst;
