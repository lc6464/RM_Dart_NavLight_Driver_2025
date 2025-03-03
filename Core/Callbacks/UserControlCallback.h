#pragma once

void RegisterUserControlCallbacks(void);

enum class LastBrightness {
	EightyPercent,
	SixtyPercent,
	FortyPercent,
	None
};

extern LastBrightness lastBrightnessStatus;