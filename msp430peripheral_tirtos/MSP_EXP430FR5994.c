/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== MSP_EXP430FR5994.c ========
 *  This file is responsible for setting up the board specific items for the
 *  MSP_EXP430FR5994 board.
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>

#include <msp430.h>
#include <inc/hw_memmap.h>
#include <dma.h>
#include <eusci_a_uart.h>
#include <eusci_a_spi.h>
#include <eusci_b_i2c.h>
#include <eusci_b_spi.h>
#include <gpio.h>
#include <pmm.h>
#include <wdt_a.h>

#include "MSP_EXP430FR5994.h"

#include <ti/drivers/SPI.h>

const SPI_Config SPI_config[];

/*
 *  =============================== General ===============================
 */
/*
 *  ======== MSP_EXP430FR5994_initGeneral ========
 */
void MSP_EXP430FR5994_initGeneral(void) {
    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PM5CTL0 &= ~LOCKLPM5;
}

/*
 *  =============================== DMA ===============================
 */
/*
 *  ======== MSP_EXP430FR5994_isrDMA ========
 *  This is a application defined DMA ISR. This ISR must map and call the
 *  appropriate Driver_event(handle) API to indicate completed DMA transfers.
 */
Void MSP_EXP430FR5994_isrDMA(UArg arg)
{
    /* Call the SPI DMA function, passing the SPI handle used for WiFi */
    SPI_serviceISR((SPI_Handle) &(SPI_config[0]));
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIOMSP430_config, ".const:GPIOMSP430_config")
#endif

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP430.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP430FR5994.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    /* MSP_EXP430FR5994_S1 */
    GPIOMSP430_P5_6 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,
    /* MSP_EXP430FR5994_S2 */
    GPIOMSP430_P5_5 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,
    /* MSP_EXP430FR5994_P4_1 */
    GPIOMSP430_P4_1,

    /* Output pins */
    /* MSP_EXP430FR5994_LED1 */
    GPIOMSP430_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP430FR5994_LED2 */
    GPIOMSP430_P1_1 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP430FR5994_P7_0 */
    GPIOMSP430_P7_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP430FR5994_P4_1 */
    GPIOMSP430_P4_2 | GPIO_CFG_OUT_STD,
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP430FR5994.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* MSP_EXP430FR5994_S1 */
    NULL,  /* MSP_EXP430FR5994_S2 */
    NULL   /* MSP_EXP430FR5994_P4_2 */
};

/* The device-specific GPIO_config structure */
const GPIOMSP430_Config GPIOMSP430_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn)
};

/*
 *  ======== MSP_EXP430FR5994_initGPIO ========
 */
void MSP_EXP430FR5994_initGPIO(void)
{
    /* Initialize peripheral and pins */
    GPIO_init();
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cEUSCIBHWAttrs, ".const:i2cEUSCIBHWAttrs")
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CEUSCIB.h>

I2CEUSCIB_Object i2cEUSCIBObjects[MSP_EXP430FR5994_I2CCOUNT];

const I2CEUSCIB_HWAttrs i2cEUSCIBHWAttrs[MSP_EXP430FR5994_I2CCOUNT] = {
    {
        .baseAddr = EUSCI_B1_BASE,
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CEUSCIB_fxnTable,
        .object = &i2cEUSCIBObjects[0],
        .hwAttrs = &i2cEUSCIBHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430FR5994_initI2C ========
 */
void MSP_EXP430FR5994_initI2C(void)
{
    /*
     * NOTE: TI-RTOS examples configure EUSCIB1 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
        GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    I2C_init();
}

/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiEUSCIBHWAttrsV1, ".const:sdspiEUSCIBHWAttrsV1")
#endif

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPIEUSCIB.h>

SDSPIEUSCIB_Object sdspiEUSCIBObjects[MSP_EXP430FR5994_SDSPICOUNT];

const SDSPIEUSCIB_HWAttrsV1 sdspiEUSCIBHWAttrsV1[MSP_EXP430FR5994_SDSPICOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        .portSCK = GPIO_PORT_P2,
        .pinSCK = GPIO_PIN2,
        .pinSCKModeFunction = GPIO_SECONDARY_MODULE_FUNCTION,

        .portMISO = GPIO_PORT_P1,
        .pinMISO = GPIO_PIN7,
        .pinMISOModeFunction = GPIO_SECONDARY_MODULE_FUNCTION,

        .portMOSI = GPIO_PORT_P1,
        .pinMOSI = GPIO_PIN6,
        .pinMOSIModeFunction = GPIO_SECONDARY_MODULE_FUNCTION,

        .portCS = GPIO_PORT_P4,
        .pinCS = GPIO_PIN0
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPIEUSCIB_fxnTable,
        .object = &sdspiEUSCIBObjects[0],
        .hwAttrs = &sdspiEUSCIBHWAttrsV1[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430FR5994_initSDSPI ========
 */
void MSP_EXP430FR5994_initSDSPI(void)
{
    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiEUSCIADMAHWAttrs, ".const:spiEUSCIADMAHWAttrs")
#endif

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIEUSCIADMA.h>

SPIEUSCIADMA_Object spiEUSCIADMAObjects[MSP_EXP430FR5994_SPICOUNT];
uint8_t spiEUSCIADMAscratchBuf[MSP_EXP430FR5994_SPICOUNT];

const SPIEUSCIADMA_HWAttrs spiEUSCIADMAHWAttrs[MSP_EXP430FR5994_SPICOUNT] = {
    {
        .baseAddr = EUSCI_A3_BASE,
        .clockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_SPI_MSB_FIRST,
        .scratchBufPtr = &spiEUSCIADMAscratchBuf[0],
        .defaultTxBufValue = 0,

        /* DMA */
        .dmaBaseAddr = DMA_BASE,
        /* Rx Channel */
        .rxDMAChannelIndex = DMA_CHANNEL_4,
        .rxDMASourceTrigger = DMA_TRIGGERSOURCE_16,
        /* Tx Channel */
        .txDMAChannelIndex = DMA_CHANNEL_5,
        .txDMASourceTrigger = DMA_TRIGGERSOURCE_17
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPIEUSCIADMA_fxnTable,
        .object = &spiEUSCIADMAObjects[0],
        .hwAttrs = &spiEUSCIADMAHWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP430FR5994_initSPI ========
 */
void MSP_EXP430FR5994_initSPI(void)
{
    /* EUSCIA3 */

    /* SOMI/MISO */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
        GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* SIMO/MOSI */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6,
        GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    /* CLK */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6,
        GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartEUSCIAHWAttrs, ".const:uartEUSCIAHWAttrs")
#endif

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTEUSCIA.h>

UARTEUSCIA_Object uartEUSCIAObjects[MSP_EXP430FR5994_UARTCOUNT];

/*
 * The baudrate dividers were determined by using the MSP430 baudrate
 * calculator
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const UARTEUSCIA_BaudrateConfig uartEUSCIABaudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    {
        .outputBaudrate = 115200,
        .inputClockFreq = 8000000,
        .prescalar = 4,
        .hwRegUCBRFx = 5,
        .hwRegUCBRSx = 85,
        .oversampling = 1
    },
    {9600, 8000000, 52, 1, 0, 1},
    {9600,   32768,  3, 0, 3, 0},
};

const UARTEUSCIA_HWAttrs uartEUSCIAHWAttrs[MSP_EXP430FR5994_UARTCOUNT] = {
    {
        .baseAddr = EUSCI_A0_BASE,
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries = sizeof(uartEUSCIABaudrates)/sizeof(UARTEUSCIA_BaudrateConfig),
        .baudrateLUT = uartEUSCIABaudrates
    },
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTEUSCIA_fxnTable,
        .object = &uartEUSCIAObjects[0],
        .hwAttrs = &uartEUSCIAHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430FR5994_initUART ========
 */
void MSP_EXP430FR5994_initUART(void)
{
    /* P2.0,1 = EUSCI_A0 TXD/RXD */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
        GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
        GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);

    /* Initialize the UART driver */
    UART_init();
}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogMSP430HWAttrs, ".const:watchdogMSP430HWAttrs")
#endif

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP430.h>

WatchdogMSP430_Object watchdogMSP430Objects[MSP_EXP430FR5994_WATCHDOGCOUNT];

const WatchdogMSP430_HWAttrs watchdogMSP430HWAttrs[MSP_EXP430FR5994_WATCHDOGCOUNT] = {
    {
        .baseAddr = WDT_A_BASE,
        .sfrAddr = SFR_BASE,
        .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
        .clockDivider = WDT_A_CLOCKDIVIDER_8192K
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogMSP430_fxnTable,
        .object = &watchdogMSP430Objects[0],
        .hwAttrs = &watchdogMSP430HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430FR5994_initWatchdog ========
 */
void MSP_EXP430FR5994_initWatchdog(void)
{
    /* Initialize the Watchdog driver */
    Watchdog_init();
}
