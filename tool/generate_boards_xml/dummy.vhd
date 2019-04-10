-- Dummy component for creating a temporal design
-- Javier Sevilla

library ieee;
use ieee.std_logic_1164.all;

entity dummy is
port (
    clk     : IN  STD_LOGIC;
    output  : OUT STD_LOGIC);
end;

architecture structural_dummy of dummy is
begin
  dummyPrcss: PROCESS (clk)
  BEGIN
    IF clk'EVENT AND clk = '1' THEN
        output <= '0';
    END IF;
  END PROCESS dummyPrcss;
end structural_dummy;

