/* kernel/arch/arm/mach-ux500/gpio-pdp.c
 *
 * Copyright (C) 2010 Sony Ericsson Mobile Communications AB.
 * Copyright (C) 2012 Sony Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/string.h>

#include <asm/mach-types.h>
#include <plat/pincfg.h>
#include <plat/gpio-nomadik.h>
#include <linux/mfd/abx500/ab8500-gpio.h>

#include <mach/hardware.h>
#include <mach/suspend.h>

#include "pins-db8500.h"
#include "pins.h"
#include "board-pins-sleep-force.h"

enum custom_pin_cfg_t {
	PINS_FOR_DEFAULT,
	PINS_FOR_U9500_21,
};

static enum custom_pin_cfg_t pinsfor = PINS_FOR_DEFAULT;
static pin_cfg_t mop500_pins_default[] = {
	/* UART */
	/* uart-0 pins gpio configuration should be
	 * kept intact to prevent glitch in tx line
	 * when tty dev is opened. Later these pins
	 * are configured to uart mop500_pins_uart0
	 *
	 * It will be replaced with uart configuration
	 * once the issue is solved.
	 */
	GPIO0_GPIO		|	PIN_INPUT_PULLUP,
	GPIO1_GPIO		|	PIN_OUTPUT_HIGH,
	GPIO2_GPIO		|	PIN_INPUT_PULLUP,
	GPIO3_GPIO		|	PIN_OUTPUT_HIGH,

	GPIO4_GPIO		|	PIN_INPUT_PULLUP,
	GPIO5_GPIO		|	PIN_OUTPUT_LOW,
	GPIO6_IP_GPIO0		|	PIN_OUTPUT_LOW,
	GPIO7_GPIO		|	PIN_INPUT_PULLUP,
	GPIO8_IPI2C_SDA,
	GPIO9_IPI2C_SCL,

	GPIO12_MSP0_TXD,
	GPIO13_MSP0_TFS,
	GPIO14_MSP0_TCK,
	GPIO15_MSP0_RXD,

	GPIO18_U2_RXD		|	PIN_INPUT_PULLUP,
	GPIO19_U2_TXD		|	PIN_OUTPUT_HIGH,
	GPIO20_GPIO		|	PIN_OUTPUT_LOW,
	GPIO21_GPIO		|	PIN_OUTPUT_LOW,
	GPIO22_GPIO		|	PIN_OUTPUT_LOW,
	GPIO24_UARTMOD_RXD,

	GPIO29_SPI3_CLK,
	GPIO30_SPI3_RXD		|	PIN_PULL_DOWN,
	GPIO31_GPIO		|	PIN_OUTPUT_HIGH,
	GPIO32_SPI3_TXD,
	GPIO33_MSP1_TXD,
	GPIO34_MSP1_TFS,
	GPIO35_MSP1_TCK,
	GPIO36_MSP1_RXD,

	GPIO64_GPIO		|	PIN_OUTPUT_LOW,
	GPIO65_GPIO		|	PIN_OUTPUT_LOW,
	GPIO66_GPIO		|	PIN_INPUT_PULLUP,
	GPIO67_GPIO		|	PIN_OUTPUT_LOW,
	GPIO68_LCD_VSI0		|	PIN_INPUT_PULLUP,
	GPIO69_GPIO		|	PIN_INPUT_PULLUP,

	GPIO75_GPIO		|	PIN_INPUT_PULLDOWN,
	GPIO76_U2_TXD,
	GPIO77_GPIO		|	PIN_INPUT_PULLUP,
	GPIO78_GPIO		|	PIN_INPUT_PULLUP,
	GPIO79_GPIO		|	PIN_INPUT_PULLUP,
	GPIO80_GPIO		|	PIN_INPUT_PULLUP,
	GPIO81_GPIO		|	PIN_INPUT_PULLUP,
	GPIO82_GPIO		|	PIN_INPUT_PULLUP,
	GPIO83_GPIO		|	PIN_INPUT_PULLUP,
	GPIO84_GPIO		|	PIN_INPUT_NOPULL,
	GPIO85_GPIO		|	PIN_INPUT_PULLUP,
	GPIO86_GPIO		|	PIN_OUTPUT_LOW,
	GPIO87_GPIO		|	PIN_INPUT_PULLUP,
	GPIO88_GPIO		|	PIN_INPUT_PULLUP,
	GPIO89_GPIO		|	PIN_INPUT_PULLUP,
	GPIO90_SM_ADQ4,
	GPIO91_GPIO		|	PIN_INPUT_PULLUP,
	GPIO92_GPIO		|	PIN_INPUT_PULLUP,
	GPIO93_GPIO		|	PIN_INPUT_PULLUP,
	GPIO94_GPIO		|	PIN_OUTPUT_LOW,
	GPIO95_GPIO		|	PIN_INPUT_PULLUP,
	GPIO96_GPIO		|	PIN_INPUT_NOPULL,
	GPIO97_GPIO		|	PIN_INPUT_PULLUP,

	GPIO130_GPIO		|	PIN_INPUT_PULLUP,

	GPIO139_GPIO		|	PIN_OUTPUT_LOW,
	GPIO140_GPIO		|	PIN_INPUT_PULLUP,
	GPIO141_GPIO		|	PIN_OUTPUT_LOW,
	GPIO142_GPIO		|	PIN_INPUT_PULLUP,
	GPIO143_SSP0_CLK,
	GPIO144_SSP0_FRM,
	GPIO145_SSP0_RXD	|	PIN_INPUT_PULLDOWN,
	GPIO146_SSP0_TXD,
	GPIO147_GPIO		|	PIN_INPUT_PULLUP,
	GPIO148_GPIO		|	PIN_INPUT_PULLUP,
	GPIO149_GPIO		|	PIN_INPUT_PULLUP,
	GPIO150_GPIO		|	PIN_INPUT_PULLUP,

	GPIO169_GPIO		|	PIN_INPUT_NOPULL,
	GPIO170_GPIO		|	PIN_INPUT_PULLUP,
	GPIO171_GPIO		|	PIN_INPUT_PULLUP,

	GPIO192_GPIO		|	PIN_INPUT_PULLUP,
	GPIO193_GPIO		|	PIN_INPUT_PULLUP,
	GPIO194_GPIO		|	PIN_INPUT_PULLUP,
	GPIO195_GPIO		|	PIN_INPUT_PULLUP,
	GPIO196_GPIO		|	PIN_INPUT_PULLUP,
	GPIO209_GPIO		|	PIN_INPUT_PULLUP,

	GPIO217_GPIO		|	PIN_OUTPUT_LOW,

	GPIO226_GPIO		|	PIN_INPUT_PULLUP,
	GPIO227_GPIO		|	PIN_OUTPUT_LOW,
	GPIO228_GPIO		|	PIN_INPUT_PULLUP,
	GPIO229_GPIO		|	PIN_INPUT_PULLUP,
	GPIO230_GPIO		|	PIN_INPUT_PULLUP,
};

/* USB */
static UX500_PINS(mop500_pins_usb,
	GPIO256_USB_NXT		| PIN_SLPM_INPUT_PULLDOWN,
	GPIO257_USB_STP		| PIN_SLPM_OUTPUT_HIGH | PIN_SLPM_PDIS_ENABLED,
	GPIO258_USB_XCLK	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO259_USB_DIR		| PIN_SLPM_INPUT_PULLDOWN,
	GPIO260_USB_DAT7	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO261_USB_DAT6	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO262_USB_DAT5	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO263_USB_DAT4	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO264_USB_DAT3	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO265_USB_DAT2	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO266_USB_DAT1	| PIN_SLPM_INPUT_PULLDOWN,
	GPIO267_USB_DAT0	| PIN_SLPM_INPUT_PULLDOWN,
);

static pin_cfg_t u9500_21_pins[] = {
	GPIO4_U1_RXD    | PIN_INPUT_PULLUP,
	GPIO5_U1_TXD    | PIN_OUTPUT_HIGH,
};

/* sdi1 - WLAN */
static UX500_PINS(mop500_pins_sdi1,
	GPIO208_MC1_CLK,
	GPIO210_MC1_CMD		|	PIN_INPUT_PULLUP,
	GPIO211_MC1_DAT0	|	PIN_INPUT_PULLUP,
	GPIO212_MC1_DAT1	|	PIN_INPUT_PULLUP,
	GPIO213_MC1_DAT2	|	PIN_INPUT_PULLUP,
	GPIO214_MC1_DAT3	|	PIN_INPUT_PULLUP,

);

/* sdi2 - POP eMMC */
static UX500_PINS(mop500_pins_sdi2,
	GPIO128_MC2_CLK		|	PIN_OUTPUT_LOW,
	GPIO129_MC2_CMD		|	PIN_INPUT_PULLUP,

	GPIO131_MC2_DAT0	|	PIN_INPUT_PULLUP,
	GPIO132_MC2_DAT1	|	PIN_INPUT_PULLUP,
	GPIO133_MC2_DAT2	|	PIN_INPUT_PULLUP,
	GPIO134_MC2_DAT3	|	PIN_INPUT_PULLUP,
	GPIO135_MC2_DAT4	|	PIN_INPUT_PULLUP,
	GPIO136_MC2_DAT5	|	PIN_INPUT_PULLUP,
	GPIO137_MC2_DAT6	|	PIN_INPUT_PULLUP,
	GPIO138_MC2_DAT7	|	PIN_INPUT_PULLUP,
);

/* sdi3 - SD/MMC card */
static UX500_PINS(mop500_pins_sdi3,
	GPIO215_MC3_DAT2DIR	|	PIN_OUTPUT_HIGH,
	GPIO216_MC3_CMDDIR	|	PIN_OUTPUT_LOW,
	GPIO218_MC3_DAT0DIR	|	PIN_OUTPUT_HIGH,
	GPIO219_MC3_CLK		|	PIN_OUTPUT_LOW,
	GPIO220_MC3_FBCLK	|	PIN_INPUT_NOPULL,
	GPIO221_MC3_CMD		|	PIN_INPUT_PULLUP,
	GPIO222_MC3_DAT0	|	PIN_INPUT_PULLUP,
	GPIO223_MC3_DAT1	|	PIN_INPUT_PULLUP,
	GPIO224_MC3_DAT2	|	PIN_INPUT_PULLUP,
	GPIO225_MC3_DAT3	|	PIN_INPUT_PULLUP,
);


/* sdi4 (PCB eMMC) */
static UX500_PINS(mop500_pins_sdi4,
	GPIO197_MC4_DAT3	| PIN_INPUT_PULLUP,
	GPIO198_MC4_DAT2	| PIN_INPUT_PULLUP,
	GPIO199_MC4_DAT1	| PIN_INPUT_PULLUP,
	GPIO200_MC4_DAT0	| PIN_INPUT_PULLUP,
	GPIO201_MC4_CMD		| PIN_INPUT_PULLUP,
	GPIO202_MC4_FBCLK	| PIN_INPUT_NOPULL,
	GPIO203_MC4_CLK		| PIN_OUTPUT_LOW,
	GPIO204_MC4_DAT7	| PIN_INPUT_PULLUP,
	GPIO205_MC4_DAT6	| PIN_INPUT_PULLUP,
	GPIO206_MC4_DAT5	| PIN_INPUT_PULLUP,
	GPIO207_MC4_DAT4	| PIN_INPUT_PULLUP,
);

/*
 * I2C
 */

static UX500_PINS(mop500_pins_i2c1,
	GPIO16_I2C1_SCL,
	GPIO17_I2C1_SDA,
);

static UX500_PINS(mop500_pins_i2c2,
	GPIO10_I2C2_SDA,
	GPIO11_I2C2_SCL,
);

static UX500_PINS(mop500_pins_ske,
	GPIO153_KP_I7	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO154_KP_I6	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO155_KP_I5	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO156_KP_I4	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO161_KP_I3	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO162_KP_I2	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO163_KP_I1	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO164_KP_I0	|	PIN_INPUT_PULLDOWN	|	PIN_SLPM_INPUT_PULLUP,
	GPIO157_KP_O7	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO158_KP_O6	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO159_KP_O5	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO160_KP_O4	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO165_KP_O3	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO166_KP_O2	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO167_KP_O1	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
	GPIO168_KP_O0	|	PIN_INPUT_PULLUP	|	PIN_SLPM_OUTPUT_LOW,
);

static struct ux500_pin_lookup mop500_pins[] = {
	PIN_LOOKUP("nmk-i2c.1", &mop500_pins_i2c1),
	PIN_LOOKUP("nmk-i2c.2", &mop500_pins_i2c2),
	PIN_LOOKUP("ske", &mop500_pins_ske),
	PIN_LOOKUP("sdi1", &mop500_pins_sdi1),
	PIN_LOOKUP("sdi2", &mop500_pins_sdi2),
	PIN_LOOKUP("sdi3", &mop500_pins_sdi3),
	PIN_LOOKUP("sdi4", &mop500_pins_sdi4),
	PIN_LOOKUP("musb-ux500.0", &mop500_pins_usb),
};
void __init mop500_pins_init(void)
{
	nmk_config_pins(mop500_pins_default, ARRAY_SIZE(mop500_pins_default));
	ux500_pins_add(mop500_pins, ARRAY_SIZE(mop500_pins));

	switch (pinsfor) {
	case PINS_FOR_U9500_21:
		nmk_config_pins(u9500_21_pins, ARRAY_SIZE(u9500_21_pins));
		break;

	case PINS_FOR_DEFAULT:
	default:
		break;
	}
}
