#include "tim.h"

#include <array>
#include <cstring>

#include "Button_Shared.h"
#include "Status.h"

static uint8_t tim4_scaler_100 = 0;

inline static void TIM4_Update_Callback() {
	// 1ms
	keyButton.Tick();

	if (++tim4_scaler_100 == 100) {
		// 100ms
		tim4_scaler_100 = 0;

		Status::fanAutoControl();
		HAL_GPIO_WritePin(Fan_Enable_GPIO_Port, Fan_Enable_Pin, Status::getFanStatus() ? GPIO_PIN_SET : GPIO_PIN_RESET);

		__HAL_TIM_SetCompare(&htim1, Main_LED_Channel, Status::brightness);
		HAL_GPIO_WritePin(LED_Enable_GPIO_Port, LED_Enable_Pin, Status::brightness == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM4) {
		TIM4_Update_Callback();
	}
}
