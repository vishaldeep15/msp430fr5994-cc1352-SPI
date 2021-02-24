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
#define SLAVE_MSG      ("Hello from slave, msg#: ")

#define MAX_LOOP        (10)

unsigned char slaveRxBuffer[SPI_MSG_LENGTH];
unsigned char slaveTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block master until slave is ready for transfer */
static Semaphore_Handle slaveSem;

/* Status indicating whether or not SPI transfer succeeded. */
bool transferStatus;

Task_Struct spiSlaveTask;
static Task_Params spiSlaveTaskParams;
static  Char spiSlaveTaskStack[TASKSTACKSIZE];

UART_Handle uart;
UART_Params uartParams;

int transactionStatus = 0;
/*
 *  ======== transferCompleteFxn ========
 *  Callback function for SPI_transfer().
 */
void transferCompleteFxn(SPI_Handle handle, SPI_Transaction *transaction)
{
    if (transaction->status != SPI_TRANSFER_COMPLETED) {
        transferStatus = false;
    }
    else {
        transferStatus = true;
    }

    transactionStatus = transaction->status;
    Semaphore_post(slaveSem);
}


void spiSlaveFxn(UArg arg0, UArg arg1) {
    SPI_Handle      slaveSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    bool            transferOK;
    char uartStr[10];

    /* Configure spi master and slave ready gpio pins */
    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_SPI_MASTER_READY, GPIO_CFG_INPUT);

    /* Set master ready pin */
    GPIO_write(CONFIG_SPI_SLAVE_READY, 1);
    UART_write(uart, "CONFIG_SPI_SLAVE_READY\n", sizeof("CONFIG_SPI_SLAVE_READY\n"));

    /* Wait for slave to be ready */
    while (GPIO_read(CONFIG_SPI_MASTER_READY) == 0) {}
    UART_write(uart, "handshake complete! \n", sizeof("handshake complete! \n"));


    /* Create a semaphore; the master will wait on this semaphore
     * until the slave is ready. */
    Semaphore_Params params;
    Error_Block eb;
    /* Init params */
    Semaphore_Params_init(&params);
    Error_init(&eb);
    /* Create semaphore instance */
    slaveSem = Semaphore_create(0, &params, &eb);
    if(slaveSem == NULL)
    {
        System_printf("Semaphore creation failed");
        System_flush();
    }

    /*
    * Wait until master SPI is open
    */
    while (GPIO_read(CONFIG_SPI_MASTER_READY)) {}
    UART_write(uart, "CONFIG_SPI_MASTER_READY \n", sizeof("CONFIG_SPI_MASTER_READY \n"));

    /* Open SPI as master */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.mode = SPI_SLAVE;
    spiParams.transferCallbackFxn = transferCompleteFxn;
    spiParams.transferMode = SPI_MODE_CALLBACK;

    slaveSpi = SPI_open(Board_SPI0, &spiParams);

    if (slaveSpi == NULL) {
        UART_write(uart, "Error initializing slave SPI\n", sizeof("Error initializing slave SPI\n"));
         while (1);
     }
     else {
         UART_write(uart, "SLave SPI initialized\n", sizeof("Slave SPI initialized\n"));
     }


    /* Copy message to transmit buffer */
    strncpy((char *) slaveTxBuffer, SLAVE_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {

        /* Initialize slave SPI transaction structure */
        slaveTxBuffer[sizeof(SLAVE_MSG) - 1] = (i % 10) + '0';
        memset((void *) slaveRxBuffer, 0, SPI_MSG_LENGTH);
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) slaveTxBuffer;
        transaction.rxBuf = (void *) slaveRxBuffer;

        UART_write(uart, "Created transaction\n", sizeof("Created transaction\n"));

        /* Toggle user LED, indicating a SPI transfer is in progress */
        UART_write(uart, "Transfer in progress\n", sizeof("Transfer in progress\n"));
        GPIO_toggle(Board_LED1);

        /* Perform SPI transfer */
        transferOK = SPI_transfer(slaveSpi, &transaction);

        sprintf(uartStr, "%d", transferOK );
        UART_write(uart, "transfer ok = ", sizeof("transfer ok = "));
        UART_write(uart, uartStr, sizeof(uartStr));

        if (transferOK) {
            GPIO_write(CONFIG_SPI_SLAVE_READY, 0);

            UART_write(uart, "\nWait until transfer\n", sizeof("\nWait until transfer\n"));

            /* Wait until transfer has completed */
            Semaphore_pend(slaveSem, BIOS_WAIT_FOREVER);

            /*
             * Drive CONFIG_SPI_SLAVE_READY high to indicate slave is not ready
             * for another transfer yet.
             */
            GPIO_write(CONFIG_SPI_SLAVE_READY, 1);

            sprintf(uartStr, "%d", transactionStatus);
            UART_write(uart, "Transaction status = ", sizeof("Transaction status = "));
            UART_write(uart, uartStr, sizeof(uartStr));
            UART_write(uart, "\n", sizeof("\n"));

            UART_write(uart, slaveRxBuffer, sizeof(slaveRxBuffer));

            if (transferStatus == false) {
                UART_write(uart, "\nSPI transfer failed!\n", sizeof("SPI transfer failed!\n"));
            }
            else {
//                UART_write(uart, "Slave Received! \n", sizeof("Slave Received! \n"));
                UART_write(uart, slaveRxBuffer, sizeof(slaveRxBuffer));
            }
        }
        else {
            UART_write(uart, "\nUnsuccessful Slave transfer\n", sizeof("Unsuccessful Slave transfer\n"));
        }
    }

    SPI_close(slaveSpi);

    /* Example complete - set pins to a known state */
    GPIO_setConfig(CONFIG_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_SPI_SLAVE_READY, 0);

    UART_write(uart, "Done!\n", sizeof("Done!\n"));

}

void spiSlaveTask_init() {
    Task_Params_init(&spiSlaveTaskParams);
    spiSlaveTaskParams.stackSize = TASKSTACKSIZE;
    spiSlaveTaskParams.stack = &spiSlaveTaskStack;
    Task_construct(&spiSlaveTask, (Task_FuncPtr)spiSlaveFxn, &spiSlaveTaskParams, NULL);
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


    System_printf("Starting SPI Slave MSP430");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

//    uartTask_init();
    spiSlaveTask_init();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
