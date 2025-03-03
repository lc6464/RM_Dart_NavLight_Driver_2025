#include "tim.h"

#include <algorithm>
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
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM4) {
		TIM4_Update_Callback();
	}
}
