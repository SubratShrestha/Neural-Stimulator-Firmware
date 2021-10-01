/***************************************************************************//**
* \file transport_uart.c
* \version 3.0
*
* This file provides the source code of the DFU communication APIs
* for the SCB Component UART mode.
*
* Note that by default UART component is used with a name "UART".
* Hoverer this file allows defining any component name or can even be used
* without component, only with PDL.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "transport_uart.h"
#include "cy_scb_uart.h"

/*
* USER CONFIGURABLE: Byte to byte time interval: calculated basing on current 
* component baud rate configuration.
* Set it to approximately to (50e6 / baud_rate) value in microseconds.
* E.g. baud_rate = 115200, UART_BYTE_TO_BYTE_TIMEOUT_US ~ 434
*/
#define UART_BYTE_TO_BYTE_TIMEOUT_US  (868u)


#if defined(CY_PSOC_CREATOR_USED)
    
    /* USER CONFIGURABLE: UART component instance name */
    #include "UART.h"
    
    /* USER CONFIGURABLE: Instance name of the UART component */
    #define CY_DFU_UART_INSTANCE       UART
    
    #define UART_API(fn)          UART_API_IMPL(CY_DFU_UART_INSTANCE, fn)
    #define UART_API_IMPL(a, b)    UART_API_IMPL2(a, b)
    #define UART_API_IMPL2(a, b)   a ## b

    #define CY_DFU_UART_HW    UART_API_IMPL(CY_DFU_UART_INSTANCE, _SCB__HW)
    
#else

/* USER CONFIGURABLE: the pointer to the base address of the hardware */
#define CY_DFU_UART_HW     SCB5

/**
* UART_initVar indicates whether the UART
* component has been initialized. The variable is initialized to 0
* and set to 1 the first time UART_Start() is called.
* This allows  the component to restart without reinitialization
* after the first call to the UART_Start() routine.
*
* If re-initialization of the component is required, then the
* UART_Init() function can be called before the
* UART_Start() or UART_Enable() function.
*/
static uint8 UART_initVar = 0U;


/**
* The instance-specific configuration structure.
* The pointer to this structure should be passed to UART_Init() function
* to initialize component with GUI selected settings.
*/
static cy_stc_scb_uart_config_t const UART_config =
{
    .uartMode                   = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode   = false,
    .smartCardRetryOnNack       = false,
    .irdaInvertRx               = false,
    .irdaEnableLowPowerReceiver = false,

    .oversample                 = 12UL,

    .enableMsbFirst             = false,
    .dataWidth                  = 8UL,
    .parity                     = CY_SCB_UART_PARITY_NONE,
    .stopBits                   = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter          = false,
    .breakWidth                 = 11UL,
    .dropOnFrameError           = false,
    .dropOnParityError          = false,

    .receiverAddress            = 0x0UL,
    .receiverAddressMask        = 0x0UL,
    .acceptAddrInFifo           = false,

    .enableCts                  = false,
    .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel             = 0UL,
    .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

    .rxFifoTriggerLevel         = 0UL,
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = 0UL,
    .txFifoIntEnableMask        = 0x0UL
};

/**
* The instance-specific context structure.
* It is used while the UART operation for internal configuration and
* data keeping. The user should not modify anything in this structure.
*/
static cy_stc_scb_uart_context_t UART_context;

/*******************************************************************************
* Function Name: UART_Start
****************************************************************************//**
* Invokes UART_Init() and UART_Enable().
* Also configures interrupt if it is internal.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref UART_initVar - used to check initial configuration,
* modified  on first function call.
*******************************************************************************/
static void UART_Start(void);
static void UART_Start(void)
{
    if (0U == UART_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_UART_Init(CY_DFU_UART_HW, &UART_config, &UART_context);

        /* Component is configured */
        UART_initVar = 1U;
    }

    Cy_SCB_UART_Enable(CY_DFU_UART_HW);
}

#endif /* defined(CY_PSOC_CREATOR_USED) */

/* Returns a number of bytes to copy into a DFU buffer */
#define UART_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/*******************************************************************************
* Function Name: UART_UartCyBtldrCommStart
****************************************************************************//**
* Starts the UART component.
* 
* \note This function does not initialize UART pins.
* So in Non PSoC Creator projects based only on PDL it is required to initialize
* GPIO pins for UART before calling this function.
*******************************************************************************/
void UART_UartCyBtldrCommStart(void)
{
#if defined(CY_PSOC_CREATOR_USED)
    UART_API(_Start)();
#else
    UART_Start();
#endif /* defined(CY_PSOC_CREATOR_USED) */
}


/*******************************************************************************
* Function Name: UART_UartCyBtldrCommStop
****************************************************************************//**
* Disables the UART component.
*******************************************************************************/
void UART_UartCyBtldrCommStop(void)
{
#if defined(CY_PSOC_CREATOR_USED)
    UART_API(_Disable)();
#else
    Cy_SCB_UART_Disable(CY_DFU_UART_HW, &UART_context);
#endif /* defined(CY_PSOC_CREATOR_USED) */
}


/*******************************************************************************
* Function Name: UART_UartCyBtldrCommReset
****************************************************************************//**
* Resets the receive and transmit communication buffers.
*******************************************************************************/
void UART_UartCyBtldrCommReset(void)
{
    /* Clear RX and TX buffers */
    Cy_SCB_UART_ClearRxFifo(CY_DFU_UART_HW);
    Cy_SCB_UART_ClearTxFifo(CY_DFU_UART_HW);
}


/*******************************************************************************
* Function Name: UART_UartCyBtldrCommRead
****************************************************************************//**
* Allows the caller to read data from the DFU host (the host writes the
* data). The function handles polling to allow a block of data to be completely
* received from the host device.
*
* \param pData   Pointer to a buffer to store received command.
* \param size    Number of bytes to be read.
* \param count   Pointer to the variable that contains number of bytes that were
*                received.
* \param timeout Time to wait before the function returns because of timeout,
*                in milliseconds.
*
* \return
* The status of the operation:
* - \ref CY_DFU_SUCCESS if successful.
* - \ref CY_DFU_ERROR_TIMEOUT if no data has been received.
* - See \ref cy_en_dfu_status_t.
*******************************************************************************/
cy_en_dfu_status_t UART_UartCyBtldrCommRead(uint8_t pData[], uint32_t size, uint32_t *count, uint32_t timeout)
{
    cy_en_dfu_status_t status;
    uint32_t byteCount;

    status = CY_DFU_ERROR_UNKNOWN;

    if ((pData != NULL) && (size > 0u))
    {
        status = CY_DFU_ERROR_TIMEOUT;
        /* Wait with timeout 1 ms for packet end */
        byteCount = 0u;
        do
        {
            /* Check packet start */
            if (Cy_SCB_UART_GetNumInRxFifo(CY_DFU_UART_HW) != 0u)
            {
                /* Wait for end of packet */
                do
                {
                    byteCount = Cy_SCB_UART_GetNumInRxFifo(CY_DFU_UART_HW);
                    Cy_SysLib_DelayUs(UART_BYTE_TO_BYTE_TIMEOUT_US);
                }
                while (byteCount != Cy_SCB_UART_GetNumInRxFifo(CY_DFU_UART_HW));

                byteCount = UART_BYTES_TO_COPY(byteCount, size);
                *count = byteCount;
                status = CY_DFU_SUCCESS;

                break;
            }

            Cy_SysLib_Delay(1u);
            --timeout;
        }
        while (timeout != 0u);

        if (status == CY_DFU_SUCCESS)
        {
            /* Get data from RX buffer into DFU buffer */
            (void) Cy_SCB_UART_GetArray(CY_DFU_UART_HW, (void*)pData, byteCount);
        }
    }

    return (status);
}


/*******************************************************************************
* Function Name: UART_UartCyBtldrCommWrite
****************************************************************************//**
* Allows the caller to write data to the DFU host (the host reads the
* data). The function does not use timeout and returns after data has been
* copied into the transmit buffer. The data transmission starts immediately
* after the first data element is written into the buffer and lasts until all
* data elements from the buffer are sent.
*
* \param pData     Pointer to the block of data to be written to the DFU
*                  host.
* \param size      Number of bytes to be written.
* \param count     Pointer to the variable to write the number of bytes
*                  actually written.
* \param timeOut   The time out is not used by this function.
*                  The function returns as soon as data is copied into the
*                  transmit buffer.
* \return
* The status of the operation:
* - \ref CY_DFU_SUCCESS if successful.
* - See \ref cy_en_dfu_status_t.
*******************************************************************************/
cy_en_dfu_status_t UART_UartCyBtldrCommWrite(uint8_t pData[], uint32_t size, uint32_t *count, uint32_t timeout)
{
    cy_en_dfu_status_t status;

    status = CY_DFU_ERROR_UNKNOWN;

    if ((pData != NULL) && (size > 0u))
    {
        /* Transmit data. This function does not wait until data is sent. */
        Cy_SCB_UART_PutArrayBlocking(CY_DFU_UART_HW, (void*)pData, size);

        *count = size;
        status = CY_DFU_SUCCESS;

        if (timeout != 0u)
        {
            /* empty */
        }
    }

    return (status);
}


/* [] END OF FILE */
