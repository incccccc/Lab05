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
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[1];
uint8_t TxBuffer[100];


uint8_t HOME = 0;
uint8_t LED = 0;
uint8_t BUTTON = 0;
uint8_t WRONGINPUT = 0;
uint8_t Hz = 1;
uint8_t LEDOnOff = 1;
uint8_t B4Button;

char On[] = "On";
char Off[] = "Off";
char OnOff[10];
char Pressed[] = "Pressed";
char Unpressed[] = "Unpressed";
char PressedUN[11]= "Unpressed";

enum Nameofstate {Home,Led,Button,WrongInput};
uint8_t page = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void DummyTask();
void UARTInterruptConfig();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


 UARTInterruptConfig();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //DummyTask();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	switch (page){
	case Home:
		if(HOME == 0){
			HOME = 1;
			LED = 0;
			BUTTON = 0;
			WRONGINPUT = 0;
			RxBuffer[0] = 0;
			sprintf((char*)TxBuffer,"\r\n\n\n\n\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"LAB5 64340500072\r\nWelcome to my home page\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"Please press button that is shown below to access to other function\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"------------------------------------------\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[1] LED\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[2] Button\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"------------------------------------------\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
		}


		if(RxBuffer[0] == 49) page = Led;
		if(RxBuffer[0] == 50) page = Button;
		else if(RxBuffer[0] != 49 && RxBuffer[0] != 50 && RxBuffer[0] != 0) page = WrongInput;
		break;
	case Led:
		if(LED == 0){
			HOME = 0;
			LED = 1;
			BUTTON = 0;
			WRONGINPUT = 0;
			RxBuffer[0] = 0;
			sprintf((char*)TxBuffer,"\r\n\n\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"LED Control\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"LED is now %s blinking - %d Hz (MAX-20, MIN-0)\r\n", OnOff , Hz);
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"------------------------------------------\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[q] On/Off \r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[w] + 1 Hz\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[e] - 1 Hz\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"[0] HOME\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			sprintf((char*)TxBuffer,"------------------------------------------\r\n");
			HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
		}
		else if(RxBuffer[0] == 48) page = Home;
		else if(RxBuffer[0] == 113){                //q
			if(LEDOnOff == 1) LEDOnOff = 0;
			else if(LEDOnOff == 0) LEDOnOff = 1;
			LED = 0;
			RxBuffer[0] = 0;
		}
		else if(RxBuffer[0] == 119){                //w
			Hz = Hz + 1;
			LED = 0;   // for return to Led case
			if(Hz > 20) Hz =20;
			RxBuffer[0] = 0;
		}
		else if(RxBuffer[0] == 101) {
			Hz = Hz - 1;
			LED = 0;   //e
			if(Hz < 0) Hz = 0;
			RxBuffer[0] = 0;
		}
		else if(RxBuffer[0] != 48 && RxBuffer[0] != 101 && RxBuffer[0] != 113 && RxBuffer[0] != 119 && RxBuffer[0] != 0) page = WrongInput;
		break;
	case Button:
			if(BUTTON == 0){
				HOME = 0;
				LED = 0;
				BUTTON = 1;
				WRONGINPUT = 0;
				RxBuffer[0] = 0;
				sprintf((char*)TxBuffer,"\r\n\n\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"Button check\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"Button is now %s  \r\n", PressedUN );
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"------------------------------------------\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"[0] HOME\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"------------------------------------------\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			}
			else if(RxBuffer[0] == 48) page = Home;
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1 && B4Button == 0) {
				strcpy(PressedUN  , Unpressed) ;
				BUTTON = 0;   //e

			}
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0 && B4Button == 1) {
				strcpy(PressedUN  , Pressed) ;
				BUTTON = 0;

			}
			else if(RxBuffer[0] != 48 && RxBuffer[0] != 0) page = WrongInput;
			break;
	case WrongInput:
			if(WRONGINPUT == 0){
				HOME = 0;
				LED = 0;
				BUTTON = 0;
				WRONGINPUT = 1;
				sprintf((char*)TxBuffer,"\r\n\n\n\n\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"Wrong Input\r\nPlease return to home page\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"------------------------------------------\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"[0] HOME\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
				sprintf((char*)TxBuffer,"------------------------------------------\r\n");
				HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*)TxBuffer), 50);
			}
			else if(RxBuffer[0] == 48) page = Home;
			break;
	}
	  if(LEDOnOff == 1 && Hz != 0){
			static uint32_t timestamp = 0;
			if(HAL_GetTick() > timestamp){
				timestamp = HAL_GetTick() + (500/Hz);
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				strcpy(OnOff , On) ;
			}


  }
	  else if(LEDOnOff == 0 || Hz == 0){
		  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin , 0);
		  strcpy(OnOff , Off) ;
	  }
	  B4Button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  }

  /* USER CODE END 3 */
}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void DummyTask()
{
	static uint32_t timestamp=0;
	if(HAL_GetTick()>=timestamp)
	{
		timestamp = HAL_GetTick()+100;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}

void UARTInterruptConfig()
{
	HAL_UART_Receive_IT(&huart2, RxBuffer, 1); //interupt mode

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart ==&huart2)
	{
		HAL_UART_Receive_IT(&huart2, RxBuffer, 1); //recall Receive
	}
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
