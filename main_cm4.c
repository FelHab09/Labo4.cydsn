/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "params.h"
#include "queue.h"

void ChangeLed(){
    for (;;)
    {
    vTaskDelay(pdMS_TO_TICKS(500));
    Cy_GPIO_Write(GreenLed_0_PORT,GreenLed_0_NUM,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    Cy_GPIO_Write(GreenLed_0_PORT,GreenLed_0_NUM,1);
    }
}
int main(void)
{
    __enable_irq();
    xTaskCreate(ChangeLed,"Change Led",80,NULL,3,NULL);
    vTaskStartScheduler();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
