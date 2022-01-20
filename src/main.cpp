#define WOKWI_NOHID_EMULATOR 1

#ifdef WOKWI_NOHID_EMULATOR
#include "Keyboard.h"
#else
#include <Keyboard.h>
#endif


class Pin
{
public:
	enum enState
	{
		OPEN = 0,
		CLOSED = 1
	};

	Pin(int num, bool pinLevel = HIGH, int8_t setupState = INPUT_PULLUP) noexcept : m_num(num), m_setupState(setupState), m_pinLevel(pinLevel)
	{
		setUp();
	}

	bool isTriggered(unsigned int debounceDelay = 0) noexcept
	{
		return debounceDelay ? isTriggeredWithDebounce(debounceDelay) : isTriggeredNoDebounce();
	}

	enState getState() const noexcept
	{
		return static_cast<enState>(!static_cast<bool>(m_pinLevel));
	}

protected:
	bool setUp() noexcept
	{
		pinMode(m_num, m_setupState);
		return true;
	}

	bool checkTriggered(bool& pinval) const noexcept
	{
		pinval = digitalRead(m_num);
		return !(pinval == m_pinLevel);
	}

	inline void setNewPinLevel(bool level) noexcept
	{
		m_pinLevel = level;
	}

	bool isTriggeredNoDebounce()
	{
		bool hasChanged = false;
		bool pinval = false;

		if(hasChanged = checkTriggered(pinval))
		{
			setNewPinLevel(pinval);
		}

		return hasChanged;
	}

	bool isTriggeredWithDebounce(unsigned int debounceDelay)
	{

		bool hasChanged = false;
		bool pinval = false;

		if(checkTriggered(pinval))
		{
			delay(debounceDelay);
			if(hasChanged = checkTriggered(pinval))
			{
				setNewPinLevel(pinval);
			}
		}
		return hasChanged;
	}

	// void tell()
	// {
	//   Serial.print("Pin num: "); Serial.print(m_num); Serial.print("; \n PinLevel: "); Serial.print(m_pinLevel); Serial.print("\n\n");
	//   delay(1000);
	// }


	int m_num;
	int8_t m_setupState;
	bool m_pinLevel;
};

class KeyboardObserver
{
public:
	constexpr KeyboardObserver(Keyboard_* kb = nullptr) noexcept : m_kb(kb)
	{}

	template<typename F>
	void notify(const F& func) const noexcept
	{
		if(m_kb)
		{
			func(*m_kb);
		}
	}

protected:
	Keyboard_* m_kb;
};

template<typename OBS = void>
class Button
{
public:
	constexpr Button(Pin pin, uint8_t id, const OBS* observer = nullptr, unsigned int debounceMs = 0) noexcept :
		m_pin(pin), m_id(id), m_debounceMs(debounceMs), m_observer(observer)
	{};

	void poll() noexcept
	{
		if(m_pin.isTriggered(m_debounceMs))
		{
			notifyStateChange();
		}
	}

protected:
	void notifyStateChange()
	{
		if(m_observer)
		{
			auto state = m_pin.getState();
			switch(state)
			{
			case Pin::enState::OPEN:
			{
				const auto id = m_id;
				m_observer->notify(
					[id](Keyboard_& kb)
				{
					kb.release(id);
				});
				break;
			}

			case Pin::enState::CLOSED:
			{
				const auto id = m_id;
				m_observer->notify(
					[id](Keyboard_& kb)
				{
					kb.press(id);
				});
				break;
			}

			default:
				break;
			}
		}
	}

	Pin m_pin;
	uint8_t m_id;
	unsigned int m_debounceMs;
	const OBS* m_observer = nullptr;

};

template<typename OBS = void>
constexpr Button<OBS> make_button(Pin p, uint8_t buttonID, const OBS* observer = nullptr, unsigned int debounceMs = 0) noexcept
{
	return Button<OBS>(p, buttonID, observer, debounceMs);
}

// definition of all pins that are connected to buttons
#define PIN_FOR_BUTTON_UP     2
#define PIN_FOR_BUTTON_DOWN   3
#define PIN_FOR_BUTTON_SPACE  4
#define PIN_FOR_BUTTON_BACK   5

// definition for special keys
#define KEY_SPACEBAR_CUSTOM ' '

// Debounce timing
#define DEBOUNCE 50

// Create keyboard observer and make it forward notifications to Keyboard
const KeyboardObserver kb_observer = KeyboardObserver(&Keyboard);
// Create buttons that notify kb_observer of their state changes
Button<KeyboardObserver> buttons[] =
{
  make_button(Pin(PIN_FOR_BUTTON_UP),    KEY_UP_ARROW,         &kb_observer, DEBOUNCE),
  make_button(Pin(PIN_FOR_BUTTON_DOWN),  KEY_DOWN_ARROW,       &kb_observer, DEBOUNCE),
  make_button(Pin(PIN_FOR_BUTTON_SPACE), KEY_SPACEBAR_CUSTOM,  &kb_observer, DEBOUNCE)
};

void setup()
{
	// Not really needed for now
}

void loop()
{
	// Poll every button
	for(const auto& button : buttons)
	{
		button.poll();
	}
}