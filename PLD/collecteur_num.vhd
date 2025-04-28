	library IEEE;
use IEEE.std_logic_1164.all;

ENTITY collecteur_num IS
PORT(
	H_10KHZ, VALID_HALL, DIR, PWM_IN : IN STD_LOGIC ;
	HALL_FILTRE : IN INTEGER range 0 to 7;
	PWM_AH, PWM_AL, PWM_BH, PWM_BL, PWM_CH, PWM_CL : OUT STD_LOGIC
	);
END collecteur_num ;

ARCHITECTURE archi OF collecteur_num IS
	SIGNAL Command : STD_LOGIC_VECTOR (5 downto 0);
BEGIN
	PROCESS(H_10KHZ)
	BEGIN
		IF(H_10KHZ'EVENT AND H_10KHZ='1') THEN
			IF(VALID_HALL='1')THEN
				CASE HALL_FILTRE IS
					WHEN 0 => Command <= "000000";
					WHEN 1 => Command <= "100001"; --AH et CL
					WHEN 2 => Command <= "011000"; --AL et BH
					WHEN 3 => Command <= "001001"; --CL et BH
					WHEN 4 => Command <= "000110"; --BL et CH
					WHEN 5 => Command <= "100100"; --AH et BL
					WHEN 6 => Command <= "010010"; --AL et CH
					WHEN 7 => Command <= "000000";
				END CASE;
			END IF;
		END IF;
	END PROCESS;
	
	PWM_CL <= Command(0);
	PWM_CH <= Command(1) AND PWM_IN;
	PWM_BL <= Command(2);
	PWM_BH <= Command(3) AND PWM_IN;
	PWM_AL <= Command(4);
	PWM_AH <= Command(5) AND PWM_IN;

END archi;