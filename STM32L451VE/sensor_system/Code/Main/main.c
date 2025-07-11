/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t f_add_cmpl=0;
uint8_t i2c_wr_buff[20];

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void ReadAcc ();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum {
     SYSTEM_STOPPED,
     SYSTEM_RUNNING,
 } SystemState;

 SystemState systemState = SYSTEM_STOPPED;

 int _write(int file, unsigned char * p, int len)
 {
 	HAL_UART_Transmit(&huart1, p, len, 10);
 	return len;
 }
 /*void acc(int16_t* acceleration)
 	 {
	 	 int8_t data[6];


	 	 //uint8_t accelRegisterAddress = 0x28;

	 	 	 if (HAL_I2C_Master_Receive(&hi2c1, 0xD4,  data, 6, HAL_MAX_DELAY))
	 	 	 	 {
	 	 		 	 acceleration[0] = (int16_t)((data[1] << 8) | data[0]);
	 	 		 	 acceleration[1] = (int16_t)((data[3] << 8) | data[2]);
	 	 		 	 acceleration[2] = (int16_t)((data[5] << 8) | data[4]);
	 	 	 	 }
 	 }
*/
 void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
 	 {
	 	 f_add_cmpl = 1;
	 	 printf("Tx completed~!!\r\n");
 	 }

 void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){}
 void I2C_Add_serch(void)
 {
	 uint8_t i;
	 for(i=0;i<0x80;i++)
	 {
		 f_add_cmpl=0;
		 HAL_I2C_Master_Transmit_DMA(&hi2c1,(uint16_t)(i<<1),i2c_wr_buff,1);
		 HAL_Delay(10);
		 if (f_add_cmpl == 1)
		 	 { printf("Device Address search complete. [%02X]\r\n",i);
		 	 return;
		 	 }
	 }
	printf("Device Address search fail.\r\n");
 }

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t SW1_c = 0;
  uint8_t SW2_c = 0;
  //int acceleration[3];

  I2C_Add_serch();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	      //GPIO_PinState startButtonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	 	 // GPIO_PinState stopButtonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);


	 	   /*if (flag==1)
	 	   {
	 		   HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_12);
	 		   flag=0;
	 	   }*/
	  //acc(acceleration[3]);

	  uint8_t SW1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	  uint8_t SW2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);



	  if ((SW1_c ==1) && (SW1==0))
	  	  	  {
		  //printf("0bit = %d\n1bit = %d\n2bit = %d;\n",acceleration[0],acceleration[1],acceleration[2]);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,SET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,RESET);
		  	  	  HAL_Delay(500);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,SET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,RESET);
		  	  	  HAL_Delay(500);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,SET);
		  	  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,RESET);
		  	  	  HAL_Delay(500);
		  	      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,RESET);
		  	  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,SET);
		  	  	  HAL_Delay(500);
		  	  	 //printf("0bit = %d\n; 1bit = %d\n; 2bit = %d;\n",acceleration[0],acceleration[1],acceleration[2]);
		  	  	  if (SW2 == 1 && SW2_c ==0)
		  	  	  {
		  	  		  /*while(2)
		  	  		  {
		  	  			SW1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
		  	  			if (SW1 == 1)
		  	  			{
		  	  				break;
		  	  			}
		  	  		  }*/
		  	  	  }

	          }
	  SW1_c = SW1;
	  SW2_c = SW2;
	 		     }




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */


  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA12 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC7 PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 flag=1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
