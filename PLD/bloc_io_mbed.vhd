library IEEE;
use IEEE.std_logic_1164.all;

entity bloc_io_mbed is
	port (AD:inout std_logic_vector (7 downto 0);
		RD_WRn,PCS6: in std_logic; DATA_OUT: out std_logic_vector (7 downto 0);
		DATA_IN:in std_logic_vector (7 downto 0));
end bloc_io_mbed;

architecture bloc_ioa of bloc_io_mbed is
signal WR_INT,RD_INT: std_logic; 
begin
	RD_INT<=PCS6 or not(RD_WRn);
	WR_INT<=PCS6 or RD_WRn;	
	AD<=DATA_IN when (RD_INT='0') else "ZZZZZZZZ"; 
		
P1:process (WR_INT)
begin	
	if (WR_INT'event and WR_INT='1' ) then
		DATA_OUT<=AD;
	end if;
end process P1;	 	
end bloc_ioa;
