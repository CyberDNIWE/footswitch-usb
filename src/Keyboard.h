/*
  This is a mock-up of KEYBOARD library Keyboard class (with other goodies)
  to be able to build in places with no HID available (like online simulators etc)
  Keyboard.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

*/

#ifndef KEYBOARD_h
#define KEYBOARD_h

//================================================================================
//================================================================================
//  Keyboard

#define KEY_LEFT_CTRL     0x80
#define KEY_LEFT_SHIFT    0x81
#define KEY_LEFT_ALT      0x82
#define KEY_LEFT_GUI      0x83
#define KEY_RIGHT_CTRL    0x84
#define KEY_RIGHT_SHIFT   0x85
#define KEY_RIGHT_ALT     0x86
#define KEY_RIGHT_GUI     0x87

#define KEY_UP_ARROW      0xDA
#define KEY_DOWN_ARROW    0xD9
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7
#define KEY_BACKSPACE     0xB2
#define KEY_TAB           0xB3
#define KEY_RETURN        0xB0
#define KEY_ESC           0xB1
#define KEY_INSERT        0xD1
#define KEY_DELETE        0xD4
#define KEY_PAGE_UP       0xD3
#define KEY_PAGE_DOWN     0xD6
#define KEY_HOME          0xD2
#define KEY_END           0xD5
#define KEY_CAPS_LOCK     0xC1
#define KEY_F1            0xC2
#define KEY_F2            0xC3
#define KEY_F3            0xC4
#define KEY_F4            0xC5
#define KEY_F5            0xC6
#define KEY_F6            0xC7
#define KEY_F7            0xC8
#define KEY_F8            0xC9
#define KEY_F9            0xCA
#define KEY_F10           0xCB
#define KEY_F11           0xCC
#define KEY_F12           0xCD
#define KEY_F13           0xF0
#define KEY_F14           0xF1
#define KEY_F15           0xF2
#define KEY_F16           0xF3
#define KEY_F17           0xF4
#define KEY_F18           0xF5
#define KEY_F19           0xF6
#define KEY_F20           0xF7
#define KEY_F21           0xF8
#define KEY_F22           0xF9
#define KEY_F23           0xFA
#define KEY_F24           0xFB

// Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
	uint8_t modifiers;
	uint8_t reserved;
	uint8_t keys[6];
} KeyReport;

class Keyboard_
{
private:
	KeyReport _keyReport;
	const uint8_t *_asciimap;
	void sendReport(KeyReport* keys) {};
public:
	Keyboard_(void)
	{
		Serial.begin(115200);
	};
	void begin(const uint8_t *layout = nullptr) {};
	void end(void) {};
	size_t write(uint8_t k)
	{
		Serial.println(k);
		return 0;
	};
	size_t write(const uint8_t *buffer, size_t size) { return 0; };
	size_t press(uint8_t k)
	{
		Serial.print("Keyboard::press: ");
		Serial.print(k);
		Serial.print('\n');

		delay(300);
		return 0;
	};
	size_t release(uint8_t k)
	{
		Serial.print("Keyboard::release: ");
		Serial.print(k);
		Serial.print('\n');

		delay(300);
		return 0;
	};
	void releaseAll(void) {};
};

Keyboard_ Keyboard;

#endif