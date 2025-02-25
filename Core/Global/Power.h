#pragma once

#include <cstdint>

namespace Power {
	// 计算电源电压
	float updateVoltage(uint16_t adc_value);

	// 读取上一次采样的电源电压
	float getVoltage();
}