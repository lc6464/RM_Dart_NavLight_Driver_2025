#include "data.h"

uint16_t voltage_adc_buffer = 0; // 输入电压的 ADC 数据

float power_voltage = 0.0f; // 输入电压值最近 50 次平均值

std::array<uint8_t, 30> uart_data_buffer; // 串口数据缓冲区