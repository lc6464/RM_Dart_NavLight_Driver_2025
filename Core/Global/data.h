#pragma once

#include <array>
#include <cstdint>

extern uint16_t voltage_adc_buffer; // 输入电压的 ADC 数据

extern float power_voltage; // 输入电压值最近 50 次平均值

extern std::array<uint8_t, 30> uart_data_buffer; // 串口数据缓冲区