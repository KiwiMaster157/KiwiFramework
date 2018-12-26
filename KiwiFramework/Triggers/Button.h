#pragma once

#include "Trigger.h"
#include "Animatic.h"

#include <bitset>

namespace kiwi
{

using ButtonID = std::string;

const int BUTTON_CODES = 64; //This value can be changed based on needs
using Code = std::bitset<BUTTON_CODES>;

struct ButtonEvent
{
	ButtonID id;
	
	enum Type
	{
		Click,  
		Unclick,
		Hover,  
		Unhover,
		Drag,
		KeyDown,
		KeyHold,
		KeyUp,
		COUNT
	} type;
	
	union
	{
		sf::Keyboard::Key key;
		sf::Mouse::Button btn;
	};
	sf::Vector2i coords;
};

class Button : public ButtonBase<sf::Event, ButtonEvent>, public AnimatedSprite
{
public:
	Button() = default;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	


private:


};

}