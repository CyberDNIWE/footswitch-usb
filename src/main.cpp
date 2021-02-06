#include <Arduino.h>
#include <Keyboard.h>

// pins 0-1 are not used
#define BUTTON_UP     2
#define BUTTON_DOWN   3
#define BUTTON_SPACE  4
#define BUTTON_BACK   5
#define LASTPIN_PLUS1 6 // not used

#define DEBOUNCE 50

bool button_pin_state[LASTPIN_PLUS1];

void setup()
{
  // all button pins are active low
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SPACE, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);

  for (int i=0 ; i<LASTPIN_PLUS1 ; i++)
  {
    button_pin_state[i] = HIGH;
  }
}

// Check button GPIO pin status and return true is if has changed
bool button_change(uint8_t button)
{
  bool ret_val = 0;
  if (digitalRead(button) == !button_pin_state[button])
  {
    delay(DEBOUNCE);
    if (digitalRead(button) == !button_pin_state[button])
    {
      button_pin_state[button] = digitalRead(button);
      ret_val = 1;
    }
  }
  return ret_val;
}

void loop()
{
  // Arrow Up key
  if (button_change(BUTTON_UP))
  {
    if (button_pin_state[BUTTON_UP])
    {
      Keyboard.release(KEY_UP_ARROW);
    }
    else
    {
      Keyboard.press(KEY_UP_ARROW);
    }
  }
  // Arrow Down key
  if (button_change(BUTTON_DOWN))
  {
    if (button_pin_state[BUTTON_DOWN])
    {
      Keyboard.release(KEY_DOWN_ARROW);
    }
    else
    {
      Keyboard.press(KEY_DOWN_ARROW);
    }
  }
  // Space bar
  if (button_change(BUTTON_SPACE))
  {
    if (button_pin_state[BUTTON_SPACE])
    {
      Keyboard.release(' ');
    }
    else
    {
      Keyboard.press(' ');
    }
  }
  // Arrow Left key
  if (button_change(BUTTON_BACK))
  {
    if (button_pin_state[BUTTON_BACK])
    {
      Keyboard.release(KEY_LEFT_ARROW);
    }
    else
    {
      Keyboard.press(KEY_LEFT_ARROW);
    }
  }
}
