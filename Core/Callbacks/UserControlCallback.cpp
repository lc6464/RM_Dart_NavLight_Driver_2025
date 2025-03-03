#include <algorithm>

#include "Button_Shared.h"
#include "Status.h"

enum class LastBrightness {
	EightyPercent,
	SixtyPercent,
	FortyPercent,
	None
};

static LastBrightness lastBrightness = LastBrightness::None;

inline static void keyButtonShortPressCallback(void) {
	Status::target = 0;
	lastBrightness = LastBrightness::None;
}

inline static void keyButtonLongPressCallback(void) {
	switch (lastBrightness) {
	case LastBrightness::SixtyPercent:
		Status::target = 40;
		lastBrightness = LastBrightness::FortyPercent;
		break;
	case LastBrightness::FortyPercent:
	case LastBrightness::None:
		Status::target = 80;
		lastBrightness = LastBrightness::EightyPercent;
		break;
	case LastBrightness::EightyPercent:
		Status::target = 60;
		lastBrightness = LastBrightness::SixtyPercent;
		break;
	}
}

// 注册用户控制回调函数
void RegisterUserControlCallbacks(void) {
	keyButton.RegisterShortPressCallback(keyButtonShortPressCallback);
	keyButton.RegisterLongPressCallback(keyButtonLongPressCallback);
}
