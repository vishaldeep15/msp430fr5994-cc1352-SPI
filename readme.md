# System setup for MSP430 running TI-RTOS
- MSP430 TI-RTOS version - 2.20.0.06
- [XDCTools version - 3.32.0.06_core](http://downloads.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/rtsc/)    
- [MSP430 Compiler tools version - 16.9.11.LTS](http://software-dl.ti.com/ccs/esd/documents/ccs_compiler-installation-selection.html)
    To install a specific compiler tools version:
    In CCS got to Help -> Install Code generation Compiler Tools -> TI Compiler Updates -> Choose a version and install
    To select a compiler version:
    Right click on project -> properties -> Resource -> CCS General -> Choose compiler version
    

# Peripherals Setup
All the examples/steps are specifically for MSP430FR5994
## 1. Adding a TI-RTOS driver to MSP430
By default TI-RTOS does not include all drivers. We need to perform the steps given in [TI-RTOS user manual section 5.2.8](https://www.ti.com/lit/ug/spruhd4m/spruhd4m.pdf?ts=1613072721257&ref_url=https%253A%252F%252Fwww.google.com%252F) for MSP430 devices to add a TI-RTOS driver such as UART, SPI, I2C etc.
## 2. GPIO
To add a GPIO, go to `<board>.c` and add the GPIO pin that needs to be configured in `gpioPinCofig[]` array as shown below. The name of the pin can be found in `GPIOMSP430.h`. 
If using an interrupt in the GPIO pin, update the `gpioCallbackFunctions[]` also with the callback function name or `NULL` if declaring dynamically using `GPIO_setConfig()`.
```
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    /* MSP_EXP430FR5994_S1 */
    GPIOMSP430_P5_6 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,

    /* Output pins */
    /* MSP_EXP430FR5994_LED1 */
    GPIOMSP430_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP430FR5994_LED2 */
    GPIOMSP430_P7_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
};
```
Then in `<board>.h` add it to the enum of GPIOName:
```
typedef enum MSP_EXP430FR5994_GPIOName {
    MSP_EXP430FR5994_S1 = 0,
    MSP_EXP430FR5994_LED1,
    MSP_EXP430FR5994_P7_0,

    MSP_EXP430FR5994_GPIOCOUNT
} MSP_EXP430FR5994_GPIOName;
```
After that in `Board.h` define a name of the GPIO pin that you just configured:
```
#define Board_P7_0                  MSP_EXP430FR5994_P7_0
```
Then GPIO pin can be directly accessed in the code like this:
```
GPIO_toggle(Board_P7_0);
```
## 2. UART
- In CCS project properties -> Resource -> CCS General -> MPU tab -> uncheck Enable Memory Protection Unit (MPU).
- Open .cfg file in the project and add following lines at the end or use the method described in 1:
```
var hwiParams = new halHwi.Params();
hwiParams.arg = 0;
halHwi.create(48, "&UARTEUSCIA_hwiIntFxn", hwiParams);
```
## 3. SPI
- Open .cfg file in the project and add following lines at the end or use the method described in 1. 42 is interrupt vector controller number for DMA.
```
var halHwi1Params = new halHwi.Params();
halHwi1Params.instance.name = "halHwi1";
halHwi1Params.arg = 0;
halHwi1Params.priority = 5;
Program.global.halHwi1 = halHwi.create(42, "&MSP_EXP430FR5994_isrDMA", halHwi1Params); 
```
In `MSP_EXP430FR5994.C` update the DMA channel and index according to the board specifications given in [Table 9-11 of the datasheet](https://www.ti.com/lit/ds/symlink/msp430fr5994.pdf?ts=1614090323923&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FMSP430FR5994).
```
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
```

# CC1352R and MSP430FR5994 SPI Pin Config
| CC1352R (Slave) | MSP430FR5994 (Master) | Function     |
| ------- | ------------ | --------     |
|  DIO10  | P6.2         | CLK          |  
|  DIO8   | P6.1         | MISO         |  
|  DIO9   | P6.0         | MOSI         |
|  DIO20  | P6.3         | Slave Select |
|  DIO21  | P4.2         | Slave ready  |
|  DIO15  | P4.1         | Master ready | 

# CC1352R Master and Slave SPI Pin Config
| CC1352R (Slave) | CC1352R (Master) | Function     |
| ------- | ------------ | --------     |
|  DIO10  | DIO10        | CLK          |  
|  DIO8   | DIO8         | MISO         |  
|  DIO9   | DIO9         | MOSI         |
|  DIO20  | DIO20        | Slave Select |
|  DIO21  | DIO21        | Slave ready  |
|  DIO15  | DIO15        | Master ready | 