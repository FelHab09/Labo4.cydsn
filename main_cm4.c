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


void ChangeLed()//Fonction de Partie 1
{
    for (;;)
    {
    vTaskDelay(pdMS_TO_TICKS(500));
    Cy_GPIO_Write(GreenLed_0_PORT,GreenLed_0_NUM,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    Cy_GPIO_Write(GreenLed_0_PORT,GreenLed_0_NUM,1);
    }
}
task_params_t task_A={
    .delay=1000,
    .message="Tache A en cours \n\r"
};
task_params_t task_B={
    .delay=4000,
    .message="Tache B en cours \n\r"
};
QueueHandle_t print_queue;
void print()// Fonction de partie 3b)
{
    char *message;
    xQueueReceive(print_queue,&message, portMAX_DELAY);
    UART_PutString(message);
}
void print_loop(task_params_t*params)// Fonction de partie 3b)
{
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(params->delay));
        char*message=params->message; // Pour la partie 3a), les trois lignes de code (char*message=params->message; xQueueSend(print_queue,&message, portMAX_DELAY); print();) etaient remplacees par UART_PutString(params->message);
        xQueueSend(print_queue,&message, portMAX_DELAY);
        print();
    }
}
SemaphoreHandle_t Semaphore;
int i=0;
void isr_bouton()//Fonction de Partie 2
{
    i++;
    xSemaphoreGiveFromISR(Semaphore,NULL);
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
}
void bouton_task()//Fonction de Partie 2
{
    for(;;){
        xSemaphoreTake(Semaphore,portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(20));
        if (i%2!=0){
        UART_PutString("Bouton appuye \r\n");
        }
        else{
        UART_PutString("Bouton relache \r\n");
        }
    }
    xSemaphoreGive(Semaphore);
}

int main(void)
{
    Semaphore=xSemaphoreCreateBinary();
    print_queue=xQueueCreate(2,sizeof(char*));
    __enable_irq();
    UART_Start();
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    xTaskCreate(ChangeLed,"Change Led",80,NULL,3,NULL);
    xTaskCreate(bouton_task,"Bouton task",80,NULL,3,NULL);
    xTaskCreate(print_loop,"Task A",configMINIMAL_STACK_SIZE,(void*)&task_A,1,NULL);
    xTaskCreate(print_loop,"Task B",configMINIMAL_STACK_SIZE,(void*)&task_B,1,NULL);
    vTaskStartScheduler();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
