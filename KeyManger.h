#pragma once
#include <windows.h>

enum {
	KEY_0 = 1,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_NUM0,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,

	KEY_RETURN,
	KEY_BACKSPACE,
	KEY_ESCAPE,

	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_ALT,
	KEY_RALT,
	KEY_CAPSLOCK,

	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,

	KEY_MOUSE1,
	KEY_MOUSE2,
	KEY_MOUSE3,
	KEY_MOUSE4,
	KEY_MOUSE5,

	KEY_LEFT,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,

	KEY_LASTKEY,
};

static const char* const strKeys[] = {
	"Nothing",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",

	"NUM0",
	"NUM1",
	"NUM2",
	"NUM3",
	"NUM4",
	"NUM5",
	"NUM6",
	"NUM7",
	"NUM8",
	"NUM9",

	"Enter",
	"BackSpace",
	"Escape",

	"LShift",
	"RShift",
	"LCtrl",
	"RCtrl",
	"Alt",
	"RAlt",
	"Caps",

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",

	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",

	"Mouse1",
	"Mouse2",
	"Mouse3",
	"Mouse4",
	"Mouse5",

	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",

};

struct sKeys {
	unsigned int VKeyID;
	bool Pressing = false;
	bool ClickedOnce = false;

};

sKeys KEYS[128];

class cKeyManger {
public:
	cKeyManger() {
	}

	void init() {

		for (int i = 0; i < KEY_LASTKEY; i++) {
			KEYS[i].VKeyID = -1;
			KEYS[i].Pressing = 0;
			KEYS[i].ClickedOnce = 0;
		}

		for (int i = 0; i <= 10; i++) {
			KEYS[KEY_0 + i].VKeyID = 0x30 + i;
		}


		//F nums
		for (int i = 0; i <= 14; i++) {
			KEYS[KEY_F1 + i].VKeyID = 0x70 + i;
		}

		//key a-z
		for (int i = 0; i <= 26; i++) {
			KEYS[KEY_A + i].VKeyID = 0x41 + i;
		}


		//keys num
		for (int i = 0; i <= 10; i++) {
			KEYS[KEY_NUM0 + i].VKeyID = 0x60 + i;
		}


		KEYS[KEY_MOUSE1].VKeyID = VK_LBUTTON;
		KEYS[KEY_MOUSE2].VKeyID = VK_RBUTTON;
		KEYS[KEY_MOUSE3].VKeyID = VK_MBUTTON;
		KEYS[KEY_MOUSE4].VKeyID = VK_XBUTTON1;
		KEYS[KEY_MOUSE5].VKeyID = VK_XBUTTON2;

		KEYS[KEY_LEFT].VKeyID = VK_LEFT;
		KEYS[KEY_UP].VKeyID = VK_UP;
		KEYS[KEY_RIGHT].VKeyID = VK_RIGHT;
		KEYS[KEY_DOWN].VKeyID = VK_DOWN;

		KEYS[KEY_LSHIFT].VKeyID = VK_LSHIFT;
		KEYS[KEY_RSHIFT].VKeyID = VK_RSHIFT;
		KEYS[KEY_LCONTROL].VKeyID = VK_LCONTROL;
		KEYS[KEY_RCONTROL].VKeyID = VK_RCONTROL;
		KEYS[KEY_ALT].VKeyID = VK_MENU;
		KEYS[KEY_RALT].VKeyID = VK_RMENU;
		KEYS[KEY_CAPSLOCK].VKeyID = VK_CAPITAL;

		KEYS[KEY_RETURN].VKeyID = VK_RETURN;
		KEYS[KEY_BACKSPACE].VKeyID = VK_BACK;
		KEYS[KEY_ESCAPE].VKeyID = VK_ESCAPE;
	}

	void Control() {
		for (int i = 0; i < KEY_LASTKEY; i++) {
			if (GetAsyncKeyState(KEYS[i].VKeyID)) {
				if (!KEYS[i].Pressing) {
					KEYS[i].Pressing = true;
					KEYS[i].ClickedOnce = true;
				}
				else {
					KEYS[i].ClickedOnce = false;
				}
			}
			else {
				KEYS[i].Pressing = false;
				KEYS[i].ClickedOnce = false;
			}
		}
	}

	bool iSNumClicked() {
		for (int i = 0; i <= 10; i++) {
			if (KEYS[KEY_0 + i].ClickedOnce)
				return true;
		}

		for (int i = 0; i <= 10; i++) {
			if (KEYS[KEY_NUM0 + i].ClickedOnce)
				return true;
		}
		return false;
	}

	int GetNum() {
		for (int i = 0; i <= 10; i++) {
			if (KEYS[KEY_0 + i].ClickedOnce)
				return i;
		}

		for (int i = 0; i <= 10; i++) {
			if (KEYS[KEY_NUM0 + i].ClickedOnce)
				return i;
		}
		return -1;
	}


	bool KeyPressed(int ID) {
		if (ID <= 0)
			return false;
		if (KEYS[ID].Pressing)
			return true;
		return false;
	}

	bool KeyClicked(int ID) {
		if (ID <= 0)
			return false;
		if (KEYS[ID].ClickedOnce)
			return true;
		return false;
	}

	bool BackspacePressed() {
		if (KEYS[KEY_BACKSPACE].Pressing)
			return true;

		return false;
	}

	int KeyIsClicked() {
		for (int i = 0; i < KEY_LASTKEY; i++) {
			if (KEYS[i].ClickedOnce) {
				return KEYS[i].VKeyID;
			}
		}
		return -1;
	}

	int GetKeyMangerID(int ID) {
		for (int i = 0; i < KEY_LASTKEY; i++) {
			if (KEYS[i].VKeyID == ID) {
				return i;
			}
		}
		return -1;
	}

	const char*RetString(int ID) {
		for (int i = 0; i < KEY_LASTKEY; i++) {
			if (KEYS[i].VKeyID == ID) {
				return strKeys[i];
			}
		}

	}

	bool FunctionKeyPressed() {
		if (KEYS[KEY_RETURN].Pressing)
			return true;
		if (KEYS[KEY_ESCAPE].Pressing)
			return true;

		return false;
	}

	bool FunctionKeyClicked() {
		if (KEYS[KEY_RETURN].ClickedOnce)
			return true;
		if (KEYS[KEY_ESCAPE].ClickedOnce)
			return true;

		return false;
	}

	bool Backspaceclicked() {
		if (KEYS[KEY_BACKSPACE].ClickedOnce)
			return true;

		return false;
	}



};

extern cKeyManger *KeyManger;