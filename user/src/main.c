/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
////////
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
// #include "stm32_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#ifdef USE_STM32100B_EVAL
 #include "stm32100b_eval_lcd.h"
#elif defined USE_STM3210B_EVAL
 #include "stm3210b_eval_lcd.h"
#elif defined USE_STM3210E_EVAL
 #include "stm3210e_eval_lcd.h" 
#elif defined USE_STM3210C_EVAL
 #include "stm3210c_eval_lcd.h"
#elif defined USE_STM32100E_EVAL
 #include "stm32100e_eval_lcd.h"
#endif

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM32100B_EVAL
  #define MESSAGE1   "STM32 MD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100B-EVAL    " 
#elif defined (USE_STM3210B_EVAL)
  #define MESSAGE1   "STM32 Medium Density" 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210B-EVAL    " 
#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
  #define MESSAGE1   "  STM32 XL Density  " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210E_EVAL)
  #define MESSAGE1   " STM32 High Density " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    " 
#elif defined (USE_STM3210C_EVAL)
  #define MESSAGE1   " STM32 Connectivity " 
  #define MESSAGE2   " Line Device running" 
  #define MESSAGE3   " on STM3210C-EVAL   "
#elif defined (USE_STM32100E_EVAL)
  #define MESSAGE1   "STM32 HD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100E-EVAL    "   
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



#define ONE_LED 0
#if ONE_LED
static volatile uint8_t is_led = 0;
static SemaphoreHandle_t xMutex_IsLed = NULL;

void vTask_led(void *pvParameters) {

  for (;;) {

    if (xSemaphoreTake(xMutex_IsLed, pdMS_TO_TICKS(10)) == pdTRUE) {

      if (!is_led) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(pdMS_TO_TICKS(2000));
        is_led = 1;
      }

      xSemaphoreGive(xMutex_IsLed);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void vTask_led_2(void *pvParameters) {
  // 任务循环（FreeRTOS 任务通常是无限循环，除非主动删除）
  for (;;) {
    // 1. 获取互斥信号量（阻塞等待，最多等10ms）
    if (xSemaphoreTake(xMutex_IsLed, pdMS_TO_TICKS(10)) == pdTRUE) {
      // 临界区：操作共享变量is_led
      if (is_led) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);    // LED亮
        vTaskDelay(pdMS_TO_TICKS(500));      // 延时期间释放CPU
        is_led = 0;                          // 修改共享变量
      }
      // 2. 释放互斥信号量
      xSemaphoreGive(xMutex_IsLed);
    }
    // 短延时，避免空循环占用CPU
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
#else
void vTask_led1(void *pvParameters) {
  uint8_t led_idx = 0;
  uint16_t led_pins[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3};
  uint8_t pin_num = sizeof(led_pins)/sizeof(led_pins[0]);

  while (1) {

    GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
    // vTaskDelay(pdMS_TO_TICKS(500));
    // Delay_ms(500);
    // GPIO_SetBits(GPIOA, led_pins[led_idx]);
    Delay_ms(500);
    GPIO_SetBits(GPIOA, led_pins[0]);
    Delay_ms(500);
    GPIO_SetBits(GPIOA, led_pins[1]);
    Delay_ms(500);
    GPIO_SetBits(GPIOA, led_pins[2]);
    Delay_ms(500);
    GPIO_SetBits(GPIOA, led_pins[3]);

    // Delay_ms(500);

    vTaskDelay(pdMS_TO_TICKS(500));

    led_idx = (led_idx + 1) % pin_num;
  }
}


void vTask_led2(void *pvParameters) {
  uint8_t led_idx = 0;
  uint16_t led_pins[] = {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6};
  uint8_t pin_num = sizeof(led_pins)/sizeof(led_pins[0]);

  while (1) {

    GPIO_ResetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
    // vTaskDelay(pdMS_TO_TICKS(500));
    // // Delay_ms(500);
    // GPIO_SetBits(GPIOB, led_pins[led_idx]);
    // Delay_ms(500);
    Delay_ms(500);
    GPIO_SetBits(GPIOB, led_pins[0]);
    Delay_ms(500);
    GPIO_SetBits(GPIOB, led_pins[1]);
    Delay_ms(500);
    GPIO_SetBits(GPIOB, led_pins[2]);
    Delay_ms(500);
    GPIO_SetBits(GPIOB, led_pins[3]);
    vTaskDelay(pdMS_TO_TICKS(500));

    led_idx = (led_idx + 1) % pin_num;
  }
}

#endif
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

  GPIO_InitTypeDef GPIO_InitStructure_1;
  GPIO_InitStructure_1.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure_1.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStructure_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure_1);
  GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);


  // xMutex_IsLed = xSemaphoreCreateMutex();
  //
  // if (xMutex_IsLed == NULL) {
  //
  //   while(1);
  // }

  // 3. 创建任务
  TaskHandle_t is_led_task = NULL;
  TaskHandle_t is_led_task_2 = NULL;

  BaseType_t xReturn1 = xTaskCreate(vTask_led1, "task_led", 256, NULL, tskIDLE_PRIORITY + 1, &is_led_task);
  BaseType_t xReturn2 = xTaskCreate(vTask_led2, "Task_led_2", 256, NULL, tskIDLE_PRIORITY + 1, &is_led_task_2);

  // 4. 启动调度器
  if(pdPASS == xReturn1 && pdPASS == xReturn2) {
  // if(pdPASS == xReturn2) {
    vTaskStartScheduler();   /* 启动任务，开启调度 */

  }

  // 如果调度器启动失败，进入死循环
  while (1) {
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */


  /* Loop until the end of transmission */
  while (1)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


