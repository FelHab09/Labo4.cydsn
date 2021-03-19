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
SemaphoreHandle_t Semaphore;
int i=0;
void isr_bouton(){
    i++;
    xSemaphoreGiveFromISR(Semaphore,NULL);
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
}
void bouton_task(){
    for(;;){
        xSemaphoreTake(Semaphore,portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(20));
        if (i%2!=0){
        UART_PutString("Bouton appuye \r\n");
        }
        else{
        UART_PutString("Bouton relache \r\n");
        }
        xSemaphoreGive(Semaphore);
    }
}

int main(void)
{
    Semaphore=xSemaphoreCreateBinary();
    __enable_irq();
    UART_Start();
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    xTaskCreate(ChangeLed,"Change Led",80,NULL,3,NULL);
    xTaskCreate(bouton_task,"Bouton task",80,NULL,3,NULL);
    vTaskStartScheduler();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
