#include "Status.h"

namespace Status {
	int8_t target = 0;
	int8_t brightness = 0;
	int8_t lastBrightness = 0;
	bool isFanOn = false;
	bool lastFanStatus = false;

	// 风扇关灯后保持
	uint16_t fanOffTick = 0;
	uint16_t fanOffTickMax = 150; // 150 * 0.1s = 15s
	bool isFanStopped = false;

	bool getFanStatus() {
		return isFanOn;
	}

	void fanAutoControl() {
		if (brightness > 0) {
			isFanOn = true;
			fanOffTick = 0;
			isFanStopped = false;
		} else {
			if (!isFanStopped) {
				if (++fanOffTick >= fanOffTickMax) {
					isFanOn = false;
					fanOffTick = 0;
					isFanStopped = true;
				}
			}
		}
	}
}