#include <algorithm>
#include <cstring>

#include "usart.h"

#include "data.h"
#include "Status.h"
#include "UserControlCallback.h"

// HAL 库串口回调函数
extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART1) {
		// 获取数据有效部分
		auto data = uart_data_buffer.data();
		data[Size] = 0;

		auto str = reinterpret_cast<char *>(data);

		if (strncmp(str, "关灯", 6) == 0) { // 判断"关灯"
			Status::target = 0;
			lastBrightnessStatus = LastBrightness::None;
		} else if (strncmp(str, "亮度", 6) == 0 && Size > 6 && Size < 10) { // 判断"亮度\d{1,3}"
			auto brightness = atoi(str + 6);
			if (brightness >= 0 && brightness <= 100) {
				Status::target = brightness;
				lastBrightnessStatus = LastBrightness::None;
			}
		} else if (strncmp(str, "开灯", 6) == 0) { // 判断"开灯"
			switch (lastBrightnessStatus) {
			case LastBrightness::SixtyPercent:
				Status::target = 40;
				lastBrightnessStatus = LastBrightness::FortyPercent;
				break;
			case LastBrightness::FortyPercent:
			case LastBrightness::None:
				Status::target = 80;
				lastBrightnessStatus = LastBrightness::EightyPercent;
				break;
			case LastBrightness::EightyPercent:
				Status::target = 60;
				lastBrightnessStatus = LastBrightness::SixtyPercent;
				break;
			}
		}

		// 重新启动接收
		uart_data_buffer.fill(0);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart_data_buffer.data(), uart_data_buffer.size());
	}
}