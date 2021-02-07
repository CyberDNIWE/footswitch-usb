#include <Arduino.h>
#include <Keyboard.h>

// pin modes
enum pin_modes
{
  NOT_USED,
  SEND_CODE_BUTTON_CLOSED,
  SEND_CODE_BUTTON_RELEASED,
  SEND_PRESS_RELEASE,
};

//
// definition of all pins that are connected to buttons
#define FIRSTPIN      2 // pins 0-1 are not used
#define BUTTON_UP     2
#define BUTTON_DOWN   3
#define BUTTON_SPACE  4
#define BUTTON_BACK   5
#define LASTPIN_PLUS1 6 // not used

#define DEBOUNCE 50

// setup all button keycodes and pin modes
bool            button_pin_state[LASTPIN_PLUS1];
uint8_t         button_keycode[LASTPIN_PLUS1] =
{
  0,            // pin 0 not used
  0,            // pin 1 not used
  KEY_UP_ARROW,
  KEY_DOWN_ARROW,
  ' ',
  KEY_LEFT_ARROW,
};

enum pin_modes  button_pin_mode[LASTPIN_PLUS1] =
{
  NOT_USED,     // pin 0 not used
  NOT_USED,     // pin 1 not used
  SEND_PRESS_RELEASE,
  SEND_PRESS_RELEASE,
  SEND_PRESS_RELEASE,
  SEND_CODE_BUTTON_CLOSED,
};

int i,j;

void setup()
{
  // all button pins are active low. turn on internal pullup
  for (i=FIRSTPIN ; i<LASTPIN_PLUS1 ; i++)
  {
    pinMode(i, INPUT_PULLUP);
    button_pin_state[i] = HIGH;
  }
}

// Check button GPIO pin status and return true if it has changed
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

// loop through all buttons and send keycodes/events depending on the
// selected mode. A pressed button (closed switch) == LOW (FALSE) since
// the inputs are active low.
void loop()
{
  for (j=FIRSTPIN ; j<LASTPIN_PLUS1 ; j++)
  {
    switch (button_pin_mode[j])
    {
      // send keycode when button is pressed (switch closed)
      case SEND_CODE_BUTTON_CLOSED:
        if (button_change(j))
        {
          if (!button_pin_state[j])
          {
            Keyboard.write(button_keycode[j]);
          }
        }
        break;

      // send keycode when button is released (switch opened)
      case SEND_CODE_BUTTON_RELEASED:
        if (button_change(j))
        {
          if (button_pin_state[j])
          {
            Keyboard.write(button_keycode[j]);
          }
        }
        break;

      // send pressed and released events when switched is closed or opened
      case SEND_PRESS_RELEASE:
        if (button_change(j))
        {
          if (button_pin_state[j])
          {
            Keyboard.release(button_keycode[j]);
          }
          else
          {
            Keyboard.press(button_keycode[j]);
          }
        }
        break;

      case NOT_USED:
      default:
        break;
    }
  }
}
