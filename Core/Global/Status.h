#pragma once

#include <cstdint>

namespace Status {
	extern uint8_t brightness;
	bool getFanStatus();
	void fanAutoControl();
}