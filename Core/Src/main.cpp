#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "data.h"
#include "Status.h"
#include "UserControlCallback.h"

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();

	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&voltage_adc_buffer, 1);

	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART1_UART_Init();

	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	HAL_TIM_Base_Start_IT(&htim4); // 1kHz

	RegisterUserControlCallbacks();

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_data_buffer.data(), uart_data_buffer.size());
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

	while (1) {

		if (Status::getFanStatus() != Status::lastFanStatus) {
			Status::lastFanStatus = Status::getFanStatus();
			// 风扇状态改变

			HAL_GPIO_WritePin(Fan_Enable_GPIO_Port, Fan_Enable_Pin, Status::getFanStatus() ? GPIO_PIN_SET : GPIO_PIN_RESET);

			// 风扇开关指示灯
			__HAL_TIM_SetCompare(&htim3, LED2_Channel, Status::getFanStatus() ? 1000 : 0);
		}

		HAL_Delay(10);

		if (Status::brightness != Status::lastBrightness) {
			Status::lastBrightness = Status::brightness;
			// 亮度改变

			__HAL_TIM_SetCompare(&htim1, Main_LED_Channel, Status::brightness);
			HAL_GPIO_WritePin(LED_Enable_GPIO_Port, LED_Enable_Pin, Status::brightness == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);

			// 主灯开关指示灯
			__HAL_TIM_SetCompare(&htim3, LED1_Channel, Status::brightness == 0 ? 0 : 1000);
		}

	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct{ };
	RCC_ClkInitTypeDef RCC_ClkInitStruct{ };
	RCC_PeriphCLKInitTypeDef PeriphClkInit{ };

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
void assert_failed(uint8_t *file, uint32_t line) {
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
		 /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
