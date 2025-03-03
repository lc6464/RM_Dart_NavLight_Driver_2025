#include <algorithm>

#include "Button_Shared.h"
#include "Status.h"
#include "UserControlCallback.h"

LastBrightness lastBrightnessStatus = LastBrightness::None;

inline static void keyButtonShortPressCallback(void) {
	Status::target = 0;
	lastBrightnessStatus = LastBrightness::None;
}

inline static void keyButtonLongPressCallback(void) {
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

// 注册用户控制回调函数
void RegisterUserControlCallbacks(void) {
	keyButton.RegisterShortPressCallback(keyButtonShortPressCallback);
	keyButton.RegisterLongPressCallback(keyButtonLongPressCallback);
}
