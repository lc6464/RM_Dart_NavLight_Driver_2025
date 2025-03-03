#include "tim.h"
#include "usart.h"

#include <algorithm>
#include <array>

#include "Button_Shared.h"
#include "data.h"
#include "Power.h"
#include "Status.h"
#include "strings.h"

static uint8_t tim4_scaler_100 = 0;
static uint8_t tim4_scaler_100_5 = 0;

inline static void TIM4_Update_Callback() {
	// 1ms
	keyButton.Tick();

	if (++tim4_scaler_100 == 100) {
		// 100ms
		tim4_scaler_100 = 0;

		Status::fanAutoControl();

		auto diff = std::abs(Status::target - Status::brightness);
		auto step = 0;
		if (Status::target == 0) {
			step = 50;
		} else if (diff > 50) {
			step = 20;
		} else if (diff > 20) {
			step = 10;
		} else {
			step = 5;
		}

		if (Status::target > Status::brightness) {
			Status::brightness = std::min(static_cast<int8_t>(Status::brightness + step), Status::target);
		} else if (Status::target < Status::brightness) {
			Status::brightness = std::max(static_cast<int8_t>(Status::brightness - step * 2), Status::target);
		}

		if (++tim4_scaler_100_5 == 5) {
			// 500ms
			tim4_scaler_100_5 = 0;

			// 发送电压
			auto voltage = Power::updateVoltage(voltage_adc_buffer);
			char voltageString[12] = { 0 };
			auto length = floatToString(voltage, voltageString, 2);
			voltageString[length++] = 'V';
			HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t *>(voltageString), length);
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM4) {
		TIM4_Update_Callback();
	}
}
