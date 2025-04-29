library IEEE;
use IEEE.std_logic_1164.all;

ENTITY filtre_hall IS
PORT(
	hall_A, hall_B, hall_C, H_10khz : IN STD_LOGIC;
	valid_hall : OUT STD_LOGIC;
	hall_filtre : OUT STD_LOGIC_VECTOR (2 downto 0)
	);
END filtre_hall;

ARCHITECTURE archi OF filtre_hall IS
SIGNAL N_3, N_2, N : STD_LOGIC_VECTOR (2 downto 0);
BEGIN
	PROCESS(H_10khz)
	BEGIN
		IF(H_10khz'EVENT AND H_10khz = '1') THEN
			N_3 <= N_2;
			N_2 <= N;
			N <= hall_C & hall_B & hall_A;
			IF((N_3 = N_2) AND (N_3 = N) AND (N_2 = N)) THEN
				valid_hall <= '1';
				hall_filtre <= N;
			ELSE
				valid_hall <= '0';
			END IF;
		END IF;
	END PROCESS;			
		
END archi;
