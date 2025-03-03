#pragma once

#include <cstdint>

namespace Status {
	extern int8_t target;
	extern int8_t brightness;
	extern int8_t lastBrightness;

	extern bool lastFanStatus;

	bool getFanStatus();
	void fanAutoControl();
}