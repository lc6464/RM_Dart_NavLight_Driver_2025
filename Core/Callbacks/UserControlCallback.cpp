#include <algorithm>

#include "Button_Shared.h"
#include "Status.h"

enum class LastBrightness {
	Full,
	EightyPercent,
	SixtyPercent,
	None
};

static LastBrightness lastBrightness = LastBrightness::None;

inline static void keyButtonShortPressCallback(void) {
	switch (lastBrightness) {
	case LastBrightness::EightyPercent:
		Status::brightness = 60;
		lastBrightness = LastBrightness::SixtyPercent;
		break;
	case LastBrightness::SixtyPercent:
	case LastBrightness::None:
		Status::brightness = 100;
		lastBrightness = LastBrightness::Full;
		break;
	case LastBrightness::Full:
		Status::brightness = 80;
		lastBrightness = LastBrightness::EightyPercent;
		break;
	}
}

inline static void keyButtonLongPressCallback(void) {
	Status::brightness = 0;
	lastBrightness = LastBrightness::None;
}

// 注册用户控制回调函数
void RegisterUserControlCallbacks(void) {
	keyButton.RegisterShortPressCallback(keyButtonShortPressCallback);
	keyButton.RegisterLongPressCallback(keyButtonLongPressCallback);
}
