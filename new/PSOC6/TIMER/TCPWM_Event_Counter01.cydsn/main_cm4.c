/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.0
*
* Description: This example demonstrates the use of PSoC 6 MCU TCPWM to count 
* the number of external events and displays the results over UART.
*
* Related Document: CE221118_PSoC6MCU_TCPWMEventCounter.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer kit
*
******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation.
******************************************************************************
* This software, including source code, documentation and related materials
* ("Software") is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and 
* foreign), United States copyright laws and international treaty provisions. 
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the 
* Cypress source code and derivative works for the sole purpose of creating 
* custom software in support of licensee product, such licensee product to be
* used only in conjunction with Cypress's integrated circuit as specified in the
* applicable agreement. Any reproduction, modification, translation, compilation,
* or representation of this Software except as specified above is prohibited 
* without the express written permission of Cypress.
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes to the Software without notice. 
* Cypress does not assume any liability arising out of the application or use
* of Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use as critical components in any products 
* where a malfunction or failure may reasonably be expected to result in 
* significant injury or death ("ACTIVE Risk Product"). By including Cypress's 
* product in a ACTIVE Risk Product, the manufacturer of such system or application
* assumes all risk of such use and in doing so indemnifies Cypress against all
* liability. Use of this Software may be limited by and subject to the applicable
* Cypress software license agreement.
*****************************************************************************/

#include "project.h"
#include "stdio.h"

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
void handle_error(void);
void TenSeconds_ISR (void);

/*******************************************************************************
*        Constants
*******************************************************************************/
#define LED_ON               (0u)      /* Value to switch LED ON  */
#define LED_OFF              (!LED_ON) /* Value to switch LED OFF */

/*******************************************************************************
*        Global variables
*******************************************************************************/
bool its_ten_seconds;

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
*
* This function processes unrecoverable errors such as UART component 
* initialization error. In case of such error the system will Turn on 
* ERROR_RED_LED and stay in an infinite loop of this function.
*
*******************************************************************************/
void handle_error(void)
{
     /* Disable all interrupts */
    __disable_irq();
	
    /* Turn on error LED */
    Cy_GPIO_Write(ERROR_RED_LED_0_PORT, ERROR_RED_LED_0_NUM, LED_ON);
    while(1u) {}
}

/***************************************************************************//**
* Function Name: TenSeconds_ISR
********************************************************************************
*
* TIMER Component interrupt service routine.
* Set "its_ten_seconds" to 1 and clear TIMER interrupt
*
*******************************************************************************/
void TenSeconds_ISR(void)
{
    its_ten_seconds = 1;
    Cy_TCPWM_ClearInterrupt(TIMER_HW, TIMER_CNT_NUM, 
                          Cy_TCPWM_GetInterruptStatus(TIMER_HW, TIMER_CNT_NUM));
}

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary
*
*******************************************************************************/
int main(void)
{
    uint32_t event_count;
    
    /* UART initialization status */
    cy_en_scb_uart_status_t uart_init_status;
    
    /* Initialize TIMER interrupt */
    Cy_SysInt_Init(&TenSeconds_ISR_cfg, TenSeconds_ISR);
    NVIC_EnableIRQ(tcpwm_0_interrupts_0_IRQn);
    
    /* Initialize the EVENT_COUNTER */
    (void)Cy_TCPWM_Counter_Init(EVENT_COUNTER_HW, EVENT_COUNTER_CNT_NUM, &EVENT_COUNTER_config);
	Cy_TCPWM_Enable_Multiple(EVENT_COUNTER_HW, EVENT_COUNTER_CNT_MASK);
    Cy_TCPWM_TriggerStart(EVENT_COUNTER_HW, EVENT_COUNTER_CNT_MASK);
    
    /* Initialize the TIMER */
    (void)Cy_TCPWM_Counter_Init(TIMER_HW, TIMER_CNT_NUM, &TIMER_config);
	Cy_TCPWM_Enable_Multiple(TIMER_HW, TIMER_CNT_MASK);
    Cy_TCPWM_TriggerStart(TIMER_HW, TIMER_CNT_MASK);
    
    /* Initialize the UART operation */
    uart_init_status = Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    if(uart_init_status!=CY_SCB_UART_SUCCESS)
    {
        handle_error();
    }
    
    Cy_SCB_UART_Enable(UART_HW);
	Cy_SCB_UART_PutString(UART_HW, "\r\n\r\nUART initialization complete\r\n");
    
    /* Enable the global interrupts */
    __enable_irq();
    
    for(;;)
    {
        /* TIMER Component generates interrupt every minute and sets "its_a_minute" to 1*/
        /* For every minute, EVENT_COUNTER count value is read and displayed */
        /* After reading the EVENT_COUNTER count value, count is reset to 0 */
        if(its_ten_seconds == 1)
        {
            its_ten_seconds = 0;
            event_count = Cy_TCPWM_Counter_GetCounter(EVENT_COUNTER_HW, EVENT_COUNTER_CNT_NUM);
            Cy_TCPWM_Counter_SetCounter(EVENT_COUNTER_HW, EVENT_COUNTER_CNT_NUM,0);
            printf("******************************************************************\n\r");
            printf("The number of times SW2 is pressed during previous minute = %d\n\r",(unsigned int) event_count);
            printf("******************************************************************\n\r");
        }
        
        /*
        The EVENT_COUNTER.count (TCPWM.count) signal is not accessible through pin P0.4(SW2 kit button).
        Following workaround is done to emulate the connection from SW2 to EVENT_COUNTER.count signal:
          1. P0.4 (SW2) is configured as PSoC 6 MCU input pin
          2. P10.1 (SW2_Out) is configured as PSoC 6 MCU output pin
          3. P0.4 (SW2) is routed to P10.1 (SW2_Out) in firmware
          4. P10.0 (CountPin) is configured as EVENT_COUNTER.count signal
          5. An external hardware jumper wire is connected between P10.1 (SW2_Out) and P10.0 (CountPin).
        */
        Cy_GPIO_Write(SW2_Out_0_PORT,SW2_Out_0_NUM, Cy_GPIO_Read(SW2_0_PORT,SW2_0_NUM));
    }
}

/*******************************************************************************
* Function Name: _write
*******************************************************************************/
#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library to allow
    the usage of the floating point conversion specifier */    
    int _write(int file, char *ptr, int len)
    {
        int nChars = 0;

        /* Suppress the compiler warning about an unused variable. */
        if (0 != file)
        {
        }
        
        for (/* Empty */; len != 0; --len)
        {
            /* Block until there is space in the TX FIFO or buffer. */
            while (0UL == Cy_SCB_UART_Put(UART_HW, *ptr))
            {
            }
            
            ++nChars;
            ++ptr;
        }
        
        return (nChars);
    }
#elif defined(__ARMCC_VERSION)
    /* For the MDK/RVDS compiler, revise the fputc() function for the printf functionality */
    struct __FILE
    {
        int handle;
    };
    
    enum
    {
        STDIN_HANDLE,
        STDOUT_HANDLE,
        STDERR_HANDLE
    };
    
    FILE __stdin = {STDIN_HANDLE};
    FILE __stdout = {STDOUT_HANDLE};
    FILE __stderr = {STDERR_HANDLE};
    
    int fputc(int ch, FILE *file)
    {
        int ret = EOF;
        switch(file->handle)
        {
            case STDOUT_HANDLE:
                while (0UL == Cy_SCB_UART_Put(UART_HW, ch))
                {
                }
                ret = ch;
            break;
                
            case STDERR_HANDLE:
                ret = ch;
            break;
                
            default:
                file = file;
            break;
        }
        return(ret);
    }
    
#elif defined (__ICCARM__)
    /* For the IAR compiler, revise the  __write() function for the printf functionality */
    size_t __write(int handle, const unsigned char * buffer, size_t size)
    {
        size_t nChars = 0;
        
        for (/* Empty */; size != 0; --size)
        {
            Cy_SCB_UART_Put(UART_HW, *buffer++);
            ++nChars;
        }
    return (nChars);
}

#endif /* (__GNUC__) */

/* [] END OF FILE */

