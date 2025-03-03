#include "Power.h"

#include "adc.h"
#include "data.h"

namespace Power {
	// 计算电源电压
	float updateVoltage(uint16_t adc_value) {
		// ADC 采样值转换为电压值
		float voltage = adc_value * 3.3f / 4096;

		// 上 47k 下 5.6k 电压分压
		auto result = voltage * (47.0f + 5.6f) / 5.6f;

		power_voltage = result;
		return result;
	}

	// 读取上一次采样的电源电压
	float getVoltage() {
		return power_voltage;
	}
}