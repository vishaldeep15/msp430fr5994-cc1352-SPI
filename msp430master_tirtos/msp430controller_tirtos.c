#include <xdc/std.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* XDC Module Headers */
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Board Header file */
#include "Board.h"
#include "driverlib.h"
//#include "UARTUtils.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>

#define TASKSTACKSIZE               768

#define SPI_MSG_LENGTH  (30)
#define MASTER_MSG      ("Hello from master, msg#: ")

#define MAX_LOOP        (10)

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block master until slave is ready for transfer */
static Semaphore_Handle masterSem;

Task_Struct spiMasterTask;
static Task_Params spiMasterTaskParams;
static  Char spiMasterTaskStack[TASKSTACKSIZE];

UART_Handle uart;
UART_Params uartParams;

void slaveReadyFxn(unsigned int index)
{
    Semaphore_post(masterSem);
}

void spiMasterFxn(UArg arg0, UArg arg1) {
    SPI_Handle      masterSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    bool            transferOK;
    char uartStr[100];

    /* Configure spi master and slave ready gpio pins */
    GPIO_setConfig(CONFIG_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_INPUT);

    /* Set master ready pin */
    GPIO_write(CONFIG_SPI_MASTER_READY, 1);
    UART_write(uart, "CONFIG_SPI_MASTER_READY\n", sizeof("CONFIG_SPI_MASTER_READY\n"));

    /* Wait for slave to be ready */
    while (GPIO_read(CONFIG_SPI_SLAVE_READY) == 0) {}
    UART_write(uart, "handshake complete! \n", sizeof("handshake complete! \n"));

    /* Handshake complete; now configure interrupt on CONFIG_SPI_SLAVE_READY */
    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(CONFIG_SPI_SLAVE_READY, slaveReadyFxn);
    GPIO_enableInt(CONFIG_SPI_SLAVE_READY);
    UART_write(uart, "configure interrupt on slave\n", sizeof("configure interrupt on slave\n"));

    /* Create a semaphore; the master will wait on this semaphore
     * until the slave is ready. */
    Semaphore_Params params;
    Error_Block eb;
    /* Init params */
    Semaphore_Params_init(&params);
    Error_init(&eb);
    /* Create semaphore instance */
    masterSem = Semaphore_create(0, &params, &eb);
    if(masterSem == NULL)
    {
        System_printf("Semaphore creation failed");
        System_flush();
    }

    /* Open SPI as master */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.mode = SPI_MASTER;
    spiParams.bitRate = 1000000;
    masterSpi = SPI_open(Board_SPI0, &spiParams);

    if (masterSpi == NULL) {
        UART_write(uart, "Error initializing master SPI\n", sizeof("Error initializing master SPI\n"));
         while (1);
     }
     else {
         UART_write(uart, "Master SPI initialized\n", sizeof("Master SPI initialized\n"));
     }

    /*
     * Master has opened CONFIG_SPI_MASTER; set CONFIG_SPI_MASTER_READY low to
     * inform the slave.
     */
    GPIO_write(CONFIG_SPI_MASTER_READY, 0);

    /* Copy message to transmit buffer */
    strncpy((char *) masterTxBuffer, MASTER_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {
        /*
         * Wait until slave is ready for transfer; slave will pull
         * CONFIG_SPI_SLAVE_READY low.
         */
        UART_write(uart, "\nWaiting for master_sem\n", sizeof("Waiting for master_sem\n"));
        Semaphore_pend(masterSem, BIOS_WAIT_FOREVER);

        UART_write(uart, "Create transaction\n", sizeof("Create transaction\n"));

        /* Initialize master SPI transaction structure */
        masterTxBuffer[sizeof(MASTER_MSG) - 1] = (i % 10) + '0';
        memset((void *) masterRxBuffer, 0, SPI_MSG_LENGTH);
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) masterTxBuffer;
        transaction.rxBuf = (void *) masterRxBuffer;

        /* Toggle user LED, indicating a SPI transfer is in progress */
        GPIO_toggle(Board_LED1);
        GPIO_write(PERIPHERAL_SELECT, 0);
        UART_write(uart, "Transfer in progress\n", sizeof("Transfer in progress\n"));

        /* Perform SPI transfer */
        transferOK = SPI_transfer(masterSpi, &transaction);

        GPIO_write(PERIPHERAL_SELECT, 1);
        sprintf(uartStr, "%d", transferOK );
        UART_write(uart, "transfer ok = ", sizeof("transfer ok = "));
        UART_write(uart, uartStr, sizeof(uartStr));
        UART_write(uart, "\n", sizeof("\n"));

        if (transferOK) {
//            UART_write(uart, "\nMaster received = ", sizeof("Master received = "));
            UART_write(uart, masterRxBuffer, sizeof(masterRxBuffer));
            UART_write(uart, "\n", sizeof("\n"));
        }
        else {
            UART_write(uart, "\nUnsuccessful Master received\n", sizeof("Unsuccessful Master received\n"));
        }

        /* Sleep for a bit before starting the next SPI transfer  */
        Task_sleep(1000);
    }

    SPI_close(masterSpi);

    /* Example complete - set pins to a known state */
    GPIO_disableInt(CONFIG_SPI_SLAVE_READY);
    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_SPI_MASTER_READY, 0);

    UART_write(uart, "Done!\n", sizeof("Done!\n"));
}

void spiMasterTask_init() {
    Task_Params_init(&spiMasterTaskParams);
    spiMasterTaskParams.stackSize = TASKSTACKSIZE;
    spiMasterTaskParams.stack = &spiMasterTaskStack;
    Task_construct(&spiMasterTask, (Task_FuncPtr)spiMasterFxn, &spiMasterTaskParams, NULL);
}

void uart_config()
{
   /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        System_abort("Error opening the UART");
    }
}

Int main(Void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();
    Board_initSPI();

    uart_config();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);


    System_printf("Starting SPI Master MSP430");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

//    uartTask_init();
    spiMasterTask_init();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
