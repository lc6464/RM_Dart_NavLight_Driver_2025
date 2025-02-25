#include "Button_Shared.h"

static const PortPinPair keyButtonPair(Key_GPIO_Port, Key_Pin);

Button keyButton(keyButtonPair);