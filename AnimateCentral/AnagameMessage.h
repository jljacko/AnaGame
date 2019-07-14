#pragma once

typedef enum AnaGameMessageType {
	agmt_MouseLDown,
	agmt_MouseRDown,
	agmt_MouseLUp,
	agmt_MouseRUp,
	agmt_MouseLDoubleClick,
	agmt_MouseRDoubleClick,
	agmt_MouseMove,
	agmt_WindowSize,
	agmt_OnChar,
	agmt_KeyDown
}AnaGameMessageType;