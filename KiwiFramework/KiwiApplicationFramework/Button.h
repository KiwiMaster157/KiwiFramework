#pragma once

#include "Animatic.h"
#include "Region.h"

#include <bitset>

namespace kiwi
{

using ButtonID = std::string;
using MouseButtons = std::bitset<sf::Mouse::ButtonCount>;

const int BUTTON_CODES = 64; //This value can be changed based on needs
using Code = std::bitset<BUTTON_CODES>;

struct ButtonEvent
{
	ButtonID id;
	
	enum Type
	{
		Press,
		Release,
		Reset,
		
		HoverBegin,
		HoverMove,
		HoverEnd,

		COUNT
	} type;

	sf::Mouse::Button active;

	MouseButtons btns;
	sf::Vector2i coords;
};

class Button : public sf::Drawable, public sf::Transformable, public AnimatedBase
{
public:
	
	Button() = default;
	Button(const Animation* srcAnimation, const Region* srcRegion);
	virtual ~Button() = default;

	bool isHovering() const;
	MouseButtons getButtons() const;
	//Resets state, but not identity
	void resetButton();

	void drawButton(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool trigger(const sf::Event& from, ButtonEvent& to);

	void setRegion(const Region* newRegion);
	const Region* getRegion() const;

	void attach(const sf::Vector2f* point);
	bool isAttached() const;
	sf::Vector2f attachedTo() const; //point (0, 0) if not attached
	
	//From Drawable:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	const Region* region = nullptr;

	const sf::Vector2f* attachTarget = nullptr;

	//Tracks which mouse buttons have been activated on this button
	MouseButtons btns;
	bool hovering = false;
};

}