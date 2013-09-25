--Authors: 	Jorden Hetherington
--				Sarah Holdijk
--				Gorden Larson
--				Steven Erenst
--
--Purpose: Top Level File for EECE 381 Project

LIBRARY ieee;
USE ieee.std_logic_1164.all; 
USE ieee.std_logic_arith.all; 
USE ieee.std_logic_unsigned.all; 

entity LaserSharknado is
port (
    SW : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
    KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    CLOCK_50 : IN STD_LOGIC;
    LEDG : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	 LEDR : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);
    DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
    DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
    DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
    DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
    DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
    LCD_DATA : INOUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    LCD_ON, LCD_BLON, LCD_EN, LCD_RS, LCD_RW : out STD_LOGIC;
	 SD_CMD, SD_DAT, SD_DAT3 : INOUT STD_LOGIC;
	 SD_CLK : OUT STD_LOGIC;
	 VGA_R:out	std_logic_vector(9	downto	0);	
	 VGA_G:out	std_logic_vector(9	downto	0);	
	 VGA_B:out	std_logic_vector(9	downto	0);	
	 VGA_CLK:	out	std_logic;
	 VGA_BLANK:	out	std_logic;	
	 VGA_HS:out	std_logic;	
	 VGA_VS:out	std_logic;	
	 VGA_SYNC:out	std_logic;
	 SRAM_DQ	:	INOUT	STD_LOGIC_VECTOR(15	downto	0);
	 SRAM_ADDR	:	OUT	STD_LOGIC_VECTOR(17	downto	0);
	 SRAM_LB_N	:	OUT	STD_LOGIC;
	 SRAM_UB_N	:	OUT	STD_LOGIC;
	 SRAM_CE_N	:	OUT	STD_LOGIC;
	 SRAM_OE_N	:	OUT	STD_LOGIC;
	 SRAM_WE_N	:	OUT	STD_LOGIC
);
end LaserSharknado;


architecture behavioural of LaserSharknado is
    COMPONENT nios_system PORT (
        clk_clk : IN STD_LOGIC;
        reset_reset_n : IN STD_LOGIC;
        sdram_clk_clk : OUT STD_LOGIC;
        leds_export : OUT STD_LOGIC_VECTOR(25 DOWNTO 0);
        switches_export : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
        sdram_wire_addr : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
        sdram_wire_ba : BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
        sdram_wire_cas_n : OUT STD_LOGIC;
        sdram_wire_cke : OUT STD_LOGIC;
        sdram_wire_cs_n : OUT STD_LOGIC;
        sdram_wire_dq : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
        sdram_wire_dqm : BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
        sdram_wire_ras_n : OUT STD_LOGIC;
        sdram_wire_we_n : OUT STD_LOGIC ;
        lcd_data_DATA   : INOUT STD_LOGIC_VECTOR(7 downto 0);
        lcd_data_ON     : OUT STD_LOGIC;
        lcd_data_BLON   : OUT STD_LOGIC;
        lcd_data_EN     : OUT STD_LOGIC;
        lcd_data_RS     : OUT STD_LOGIC;
        lcd_data_RW     : OUT STD_LOGIC;
		  sd_card_b_SD_cmd : INOUT STD_LOGIC;
		  sd_card_b_SD_dat : INOUT STD_LOGIC;
		  sd_card_b_SD_dat3 : INOUT STD_LOGIC;
		  sd_card_o_SD_clock : OUT STD_LOGIC;
		  keys_export		: IN STD_LOGIC_VECTOR(3 downto 0);
		  vga_controller_CLK	: OUT STD_LOGIC;	
		  vga_controller_HS : OUT STD_LOGIC;	
		  vga_controller_VS : OUT STD_LOGIC;
		  vga_controller_BLANK : OUT STD_LOGIC;	
		  vga_controller_SYNC : OUT STD_LOGIC;
		  vga_controller_R : OUT STD_LOGIC_VECTOR(9 downto 0);	
		  vga_controller_G : OUT STD_LOGIC_VECTOR(9 downto 0);
		  vga_controller_B : OUT STD_LOGIC_VECTOR(9 downto 0);
		  processorgpin_export : IN STD_LOGIC_VECTOR(7 downto 0);
		  processorgpout_export : OUT STD_LOGIC_VECTOR(7 downto 0);
		  sram_DQ : INOUT STD_LOGIC_VECTOR(15 downto 0);
		  sram_ADDR	: OUT STD_LOGIC_VECTOR(17 downto 0);
		  sram_LB_N : OUT STD_LOGIC;
		  sram_UB_N : OUT STD_LOGIC;
		  sram_CE_N : OUT STD_LOGIC;
		  sram_OE_N : OUT STD_LOGIC;
		  sram_WE_N : OUT STD_LOGIC
        );
    END COMPONENT;

    SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
    SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
	 
	 signal placeholder_ProcessorInput : std_logic_vector(7 downto 0);
	 signal placeholder_ProcessorOutput : std_logic_vector(7 downto 0);

BEGIN
	DRAM_BA_0 <= BA(0);
	DRAM_BA_1 <= BA(1);
	DRAM_UDQM <= DQM(1);
	DRAM_LDQM <= DQM(0);

    NiosII: nios_system PORT MAP (
        clk_clk => CLOCK_50,
        reset_reset_n => SW(0),
        sdram_clk_clk => DRAM_CLK,
        leds_export(7 downto 0) => LEDG,
		  leds_export(25 downto 8) => LEDR,
        switches_export => SW,
        sdram_wire_addr => DRAM_ADDR,
        sdram_wire_ba => BA,
        sdram_wire_cas_n => DRAM_CAS_N,
        sdram_wire_cke => DRAM_CKE,
        sdram_wire_cs_n => DRAM_CS_N,
        sdram_wire_dq => DRAM_DQ,
        sdram_wire_dqm => DQM,
        sdram_wire_ras_n => DRAM_RAS_N,
        sdram_wire_we_n => DRAM_WE_N, 
        lcd_data_DATA   => LCD_DATA,
        lcd_data_ON     => LCD_ON,
        lcd_data_BLON   => LCD_BLON,
        lcd_data_EN     => LCD_EN,
        lcd_data_RS     => LCD_RS,
        lcd_data_RW     => LCD_RW,
		  keys_export		=> not KEY,
		  processorgpin_export  => placeholder_ProcessorInput,
		  processorgpout_export => placeholder_ProcessorOutput,
		  vga_controller_CLK	=>	VGA_CLK,	
		  vga_controller_HS	=>	VGA_HS,	
		  vga_controller_VS	=>	VGA_VS,	
		  vga_controller_BLANK	=>	VGA_BLANK,	
		  vga_controller_SYNC	=>	VGA_SYNC,	
		  vga_controller_R	=>	VGA_R,	
		  vga_controller_G	=>	VGA_G,	
		  vga_controller_B	=>	VGA_B,
		  sd_card_b_SD_cmd => SD_CMD,
		  sd_card_b_SD_dat => SD_DAT,
		  sd_card_b_SD_dat3 => SD_DAT3,
		  sd_card_o_SD_clock => SD_CLK,
		  sram_DQ	=>	SRAM_DQ,
		  sram_ADDR	=>	SRAM_ADDR,
		  sram_LB_N	=>	SRAM_LB_N,
		  sram_UB_N	=>	SRAM_UB_N,
		  sram_CE_N	=>	SRAM_CE_N,
		  sram_OE_N	=>	SRAM_OE_N,
		  sram_WE_N	=>	SRAM_WE_N
    );
end behavioural;
