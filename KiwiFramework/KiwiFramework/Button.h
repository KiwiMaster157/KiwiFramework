#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <bitset>
#include "ApplicationEvent.h"

namespace kiwi
{
inline namespace v1
{

const int MAX_BUTTON_HOTKEY = 2;

struct _impl_ButtonKey
{
	bool pressed;
	sf::Keyboard::Key key;
};

///Can 
class Button
{
public:

	using BoundingBox = sf::IntRect;
	using Corner = sf::Vector2i;

	///Click = button down
	///Release = button up
	///Drag = mouse move while button down (Note: only AnyDrag will be reported)

	///Hover = movement with no presses

	///Left = Primary (not necessarily physical left)
	///Right = Secondary (not necessarily physical right)
	enum ButtonAction
	{
		NONE = -1,
		HoverStart,
		HoverMove,
		HoverStop,
		LeftClick,
		LeftRelease,
		LeftDrag,
		MiddleClick,
		MiddleRelease,
		MiddleDrag,
		RightClick,
		RightRelease,
		RightDrag,
		Scroll,

		COUNT,

		//Combinations / Misc
		Any,
		AnyClick,
		AnyRelease,
		AnyDrag,

		HotkeyPress,
		HotkeyRelease,

		COUNT_FULL
	};
	
	Button();
	Button(const Button& src) = default;
	Button(Button&& src) = default;

	//=====Aggregate=====

	ButtonAction triggerTest(sf::Event e);

	void clearState();

	//=====Mouse=====

	///Returns the screen position of button
	BoundingBox getPosition() const;
	///Sets the screen position of button
	void setPosition(BoundingBox pos);
	void setPositionPtr(BoundingBox* pos);
	void setCornerPtr(Corner* pos);
	///Clears position
	void clearPosition();

	///Returns true if act is a trigger of button
	bool hasAction(ButtonAction act) const;
	///Sets specified action
	void enableAction(ButtonAction act);
	///Unsets specified action
	void disableAction(ButtonAction act);
	///Clears all actions
	void clearActions();

	///Returns true if mouse is in bounding box
	bool inBox() const;
	///Returns true if specified point is in bounding box
	bool inBox(int x, int y) const;
	bool inBox(sf::Vector2i xy) const;

	//=====Keyboard=====

	///Marks key as pressed in hotkey and modifier
	///key = Unknown will set all
	void pressKey(sf::Keyboard::Key key = sf::Keyboard::Unknown);
	///Marks key as unpressed in hotkey and modifier
	///key = Unknown will reset all
	void releaseKey(sf::Keyboard::Key key = sf::Keyboard::Unknown);

	///Returns true if key is a hotkey of button
	bool hasHotkey(sf::Keyboard::Key key) const;
	///Replaces an existing hotkey with new (inserts hotkey by default)
	///Returns true on success
	bool setHotkey(sf::Keyboard::Key add, sf::Keyboard::Key replace = sf::Keyboard::Unknown);
	///Clears existing hotkeys
	void clearHotkeys();
	///Returns true if any hotkeys are pressed
	bool hotkeyPressed() const;

	///Returns true if key is a modifier key of button
	bool hasModifier(sf::Keyboard::Key key) const;
	///Replaces an existing modifier key with new (inserts modifier by default)
	///Returns true on success
	bool setModifier(sf::Keyboard::Key add, sf::Keyboard::Key replace = sf::Keyboard::Unknown);
	///Clears existing modifier keys
	void clearModifiers();
	///Returns true if any modifiers are pressed
	bool modifierPressed() const;

	///Returns true if key press passes hotkey and modifier tests
	///key = Unknown will check the key automatically
	bool keyPressed(sf::Keyboard::Key key = sf::Keyboard::Unknown) const;

private:
	///Button's position on screen
	BoundingBox position = { 0,0,0,0 };
	///if not nullptr, use *positionPtr for position
	BoundingBox* positionPtr = nullptr;
	///if not nullptr, use *positionCornerPtr for location
	///    and (position) for size
	Corner* positionCornerPtr = nullptr;

	///Which mouse actions will trigger an event
	std::bitset<ButtonAction::COUNT> mouseFlags = 0;
	///Which mouse buttons are registered as pressed
	std::bitset<sf::Mouse::ButtonCount> mouseButtonFlags = 0;
	///true if the mouse is in the bounding box
	bool mousePositionFlag = false;
	///Button's hotkey
	std::array<_impl_ButtonKey, MAX_BUTTON_HOTKEY> hotkey;
	///Hotkey modifier (Used for instances like Ctrl+S)
	///Modifiers are related by OR. Combinations like Ctrl+Shift+S are not natively supported
	std::array<_impl_ButtonKey, MAX_BUTTON_HOTKEY> modifier;
	///Button's sprite
	sf::Sprite image;
};

}//namespace v1
}//namespace kiwi