#include "Button.h"

namespace kiwi
{
namespace v1
{

Button::Button()
{
	clearHotkeys();
	clearModifiers();
	clearState();
}

//=====Aggregate=====

Button::ButtonAction Button::triggerTest(sf::Event e)
{
	//Handle non-mouse events and determine if mouse is in bounding box
	if (e.type == sf::Event::KeyPressed)
	{
		pressKey(e.key.code);
		return keyPressed(e.key.code)?HotkeyPress:NONE;
	}
	
	if (e.type == sf::Event::KeyReleased)
	{
		releaseKey(e.key.code);
		return NONE;
	}


	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
		if (!mousePositionFlag)
			return NONE;
		switch (e.mouseButton.button)
		{
		case sf::Mouse::Left:
			mouseButtonFlags[sf::Mouse::Left] = true;
			return hasAction(LeftClick)?LeftClick:NONE;
		case sf::Mouse::Middle:
			mouseButtonFlags[sf::Mouse::Middle] = true;
			return hasAction(MiddleClick)?MiddleClick:NONE;
		case sf::Mouse::Right:
			mouseButtonFlags[sf::Mouse::Right] = true;
			return hasAction(RightClick)?RightClick:NONE;
		}
		break;
	case sf::Event::MouseButtonReleased:
		switch (e.mouseButton.button)
		{
		case sf::Mouse::Left:
			mouseButtonFlags[sf::Mouse::Left] = false;
			return (hasAction(LeftRelease) && mousePositionFlag)?LeftRelease:NONE;
		case sf::Mouse::Middle:
			mouseButtonFlags[sf::Mouse::Middle] = false;
			return (hasAction(MiddleRelease) && mousePositionFlag)?MiddleRelease:NONE;
		case sf::Mouse::Right:
			mouseButtonFlags[sf::Mouse::Right] = false;
			return (hasAction(RightRelease) && mousePositionFlag)?RightRelease:NONE;
		}
		break;
	case sf::Event::MouseMoved:
	{
		bool currentPosition = inBox(e.mouseMove.x, e.mouseMove.y);
		if (mouseButtonFlags.any())
		{
			/*  [Comment | Uncomment] next line as necessary  */
			//mousePositionFlag = currentPosition;
			return (hasAction(AnyDrag) && mousePositionFlag) ? AnyDrag : NONE;
		}
		else if (!mousePositionFlag)
		{
			mousePositionFlag = currentPosition;
			return (hasAction(HoverStart) && currentPosition) ? HoverStart : NONE;
		}
		else
		{
			mousePositionFlag = currentPosition;
			if (currentPosition)
				return hasAction(HoverMove) ? HoverMove : NONE;
			else
				return hasAction(HoverStop) ? HoverStop : NONE;
		}
	}
	case sf::Event::MouseWheelScrolled:
		return mousePositionFlag ? Scroll : NONE;
	}
	return NONE;
}

void Button::clearState()
{
	mouseButtonFlags.reset();
	mousePositionFlag = false;
}

//=====Mouse=====

Button::BoundingBox Button::getPosition() const
{
	const BoundingBox& temp = positionPtr ? *positionPtr : position;
	Corner corner = positionCornerPtr ? *positionCornerPtr : Corner(temp.left, temp.top);
	return BoundingBox(
		corner.x,
		corner.y,
		temp.width,
		temp.height);
}

void Button::setPosition(BoundingBox pos)
{
	position = pos;
	*positionPtr = pos;
}

void Button::setPositionPtr(BoundingBox* pos)
{
	positionPtr = pos;
}

void Button::setCornerPtr(Corner* pos)
{
	positionCornerPtr = pos;
}

void Button::clearPosition()
{
	positionPtr = nullptr;
	positionCornerPtr = nullptr;
	position = { 0,0,0,0 };
}

bool Button::hasAction(ButtonAction act) const
{
	switch (act)
	{
	case HoverStart:
	case HoverMove:
	case HoverStop:
	case LeftClick:
	case LeftRelease:
	case LeftDrag:
	case MiddleClick:
	case MiddleRelease:
	case MiddleDrag:
	case RightClick:
	case RightRelease:
	case RightDrag:
	case Scroll:
		return mouseFlags[act];
		break;
	case Any:
		return mouseFlags.any();
		break;
	case AnyClick:
		return mouseFlags[LeftClick] || mouseFlags[MiddleClick] || mouseFlags[RightClick];
		break;
	case AnyRelease:
		return mouseFlags[LeftRelease] || mouseFlags[MiddleRelease] || mouseFlags[RightRelease];
		break;
	case AnyDrag:
		return mouseFlags[LeftDrag] || mouseFlags[MiddleDrag] || mouseFlags[RightDrag];
		break;
	default:
		return false;
	}
}

void Button::enableAction(ButtonAction act)
{
	switch (act)
	{
	case HoverStart:
	case HoverMove:
	case HoverStop:
	case LeftClick:
	case LeftRelease:
	case LeftDrag:
	case MiddleClick:
	case MiddleRelease:
	case MiddleDrag:
	case RightClick:
	case RightRelease:
	case RightDrag:
	case Scroll:
		mouseFlags.set(act);
		break;
	case Any:
		mouseFlags.set();
		break;
	case AnyClick:
		mouseFlags.set(LeftClick);
		mouseFlags.set(MiddleClick);
		mouseFlags.set(RightClick);
		break;
	case AnyRelease:
		mouseFlags.set(LeftRelease);
		mouseFlags.set(MiddleRelease);
		mouseFlags.set(RightRelease);
		break;
	case AnyDrag:
		mouseFlags.set(LeftDrag);
		mouseFlags.set(MiddleDrag);
		mouseFlags.set(RightDrag);
		break;
	}
}

void Button::disableAction(ButtonAction act)
{
	switch (act)
	{
	case HoverStart:
	case HoverMove:
	case HoverStop:
	case LeftClick:
	case LeftRelease:
	case LeftDrag:
	case MiddleClick:
	case MiddleRelease:
	case MiddleDrag:
	case RightClick:
	case RightRelease:
	case RightDrag:
	case Scroll:
		mouseFlags.reset(act);
		break;
	case Any:
		mouseFlags.reset();
		break;
	case AnyClick:
		mouseFlags.reset(LeftClick);
		mouseFlags.reset(MiddleClick);
		mouseFlags.reset(RightClick);
		break;
	case AnyRelease:
		mouseFlags.reset(LeftRelease);
		mouseFlags.reset(MiddleRelease);
		mouseFlags.reset(RightRelease);
		break;
	case AnyDrag:
		mouseFlags.reset(LeftDrag);
		mouseFlags.reset(MiddleDrag);
		mouseFlags.reset(RightDrag);
		break;
	}
}

void Button::clearActions()
{
	mouseFlags.reset();
}

bool Button::inBox() const
{
	return getPosition().contains(sf::Mouse::getPosition());
}

bool Button::inBox(int x, int y) const
{
	return getPosition().contains(x, y);
}

bool Button::inBox(sf::Vector2i xy) const
{
	return getPosition().contains(xy);
}

//=====Keyboard=====

//default argument: key = sf::Keyboard::Unknown
void Button::pressKey(sf::Keyboard::Key key)
{
	bool all = false;
	if (key == sf::Keyboard::Unknown)
		all = true;
	for (auto& x : hotkey)
	{
		if (x.key == key || all)
			x.pressed = true;
	}
	for (auto& x : modifier)
	{
		if (x.key == key || all)
			x.pressed = true;
	}
}

//default argument: key = sf::Keyboard::Unknown
void Button::releaseKey(sf::Keyboard::Key key)
{
	bool all = false;
	if (key == sf::Keyboard::Unknown)
		all = true;
	for (auto& x : hotkey)
	{
		if (x.key == key || all)
			x.pressed = false;
	}
	for (auto& x : modifier)
	{
		if (x.key == key || all)
			x.pressed = false;
	}
}

bool Button::hasHotkey(sf::Keyboard::Key key) const
{
	for (auto x : hotkey)
	{
		if (x.key == key)
			return true;
	}
	return false;
}

//default argument: replace = sf::Keyboard::Unknown
bool Button::setHotkey(sf::Keyboard::Key key, sf::Keyboard::Key replace)
{
	for (auto& x : hotkey)
	{
		if (x.key == replace)
		{
			x.pressed = false;
			x.key = key;
			return true;
		}
	}
	return false;
}

void Button::clearHotkeys()
{
	for (auto& x : hotkey)
	{
		x.pressed = false;
		x.key = sf::Keyboard::Unknown;
	}
}

bool Button::hotkeyPressed() const
{
	for (auto x : hotkey)
	{
		if (x.pressed)
			return true;
	}
	return false;
}

bool Button::hasModifier(sf::Keyboard::Key key) const
{
	for (auto x : modifier)
	{
		if (x.key == key)
			return true;
	}
	return false;
}

//default argument: replace = sf::Keyboard::Unknown
bool Button::setModifier(sf::Keyboard::Key key, sf::Keyboard::Key replace)
{
	for (auto& x : modifier)
	{
		if (x.key == replace)
		{
			x.pressed = false;
			x.key = key;
			return true;
		}
	}
	return false;
}

void Button::clearModifiers()
{
	for (auto& x : modifier)
	{
		x.pressed = false;
		x.key = sf::Keyboard::Unknown;
	}
}

bool Button::modifierPressed() const
{
	for (auto x : modifier)
	{
		if (x.pressed)
			return true;
	}
	return false;
}

//default argument: key = sf::Keyboard::Unknown
bool Button::keyPressed(sf::Keyboard::Key key) const
{
	if (key == sf::Keyboard::Unknown)
		return hotkeyPressed() && modifierPressed();
	return hasHotkey(key) && modifierPressed();
}

}//namespace v1
}//namespace kiwi