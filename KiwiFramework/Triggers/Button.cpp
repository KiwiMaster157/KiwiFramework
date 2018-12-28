#include "Button.h"

namespace kiwi
{

Button::Button(const ButtonID& id, const Animation* srcAnimation, const Region* srcRegion)
{
	setID(id);
	setAnimation(srcAnimation);
	setRegion(srcRegion);
}

const ButtonID& Button::getID() const
{
	return identity;
}

void Button::setID(const ButtonID& id)
{
	identity = id;
}

bool Button::isHovering() const
{
	return hovering;
}

MouseButtons Button::getButtons() const
{
	return btns;
}

void Button::resetButton()
{
	hovering = false;
	btns.reset();
}

void Button::drawButton(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(attachedTo());
	states.transform *= getTransform();
	if (region)
		drawAnimated(target, states, region->getPoints(), region->getType());
	else
		drawAnimated(target, states);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//No need to adjust transform
	drawButton(target, states);
}

bool Button::trigger(const sf::Event& from, ButtonEvent& to)
{
	if (!region)
		return false;

	sf::Vector2f transformedPoint;
	switch (from.type)
	{
	case sf::Event::MouseButtonPressed:
		if (isHovering())
		{
			btns.set(from.mouseButton.button);

			to.id = getID();
			to.type = ButtonEvent::Press;
			to.active = from.mouseButton.button;
			to.btns = btns;
			to.coords.x = from.mouseButton.x;
			to.coords.y = from.mouseButton.y;
			
			return true;
		}
		break;
	case sf::Event::MouseButtonReleased:
		
		if (btns.test(from.mouseButton.button))
		{
			btns.reset(from.mouseButton.button);
			to.id = getID();
			to.type = (isHovering() ? ButtonEvent::Release : ButtonEvent::Reset);
			to.active = from.mouseButton.button;
			to.btns = btns;
			to.coords.x = from.mouseButton.x;
			to.coords.y = from.mouseButton.y;

			return true;
		}

		break;
	case sf::Event::MouseMoved:
		transformedPoint.x = static_cast<float>(from.mouseMove.x);
		transformedPoint.y = static_cast<float>(from.mouseMove.y);

		transformedPoint -= attachedTo();
		transformedPoint = getInverseTransform().transformPoint(transformedPoint);

		if (region->contains(transformedPoint))
		{
			if (isHovering())
			{
				to.id = getID();
				to.type = ButtonEvent::HoverMove;
				to.active = sf::Mouse::Button::ButtonCount;
				to.btns = btns;
				to.coords.x = from.mouseMove.x;
				to.coords.y = from.mouseMove.y;

				return true;
			}
			else
			{
				hovering = true;
				to.id = getID();
				to.type = ButtonEvent::HoverBegin;
				to.active = sf::Mouse::Button::ButtonCount;
				to.btns = btns;
				to.coords.x = from.mouseMove.x;
				to.coords.y = from.mouseMove.y;

				return true;
			}
		}
		else if(isHovering())
		{
			hovering = false;
			to.id = getID();
			to.type = ButtonEvent::HoverEnd;
			to.active = sf::Mouse::Button::ButtonCount;
			to.btns = btns;
			to.coords.x = from.mouseMove.x;
			to.coords.y = from.mouseMove.y;

			return true;
		}

		break;
	}

	return false;
}

void Button::setRegion(const Region* newRegion)
{
	region = newRegion;
}

const Region* Button::getRegion() const
{
	return region;
}

void Button::attach(const sf::Vector2f* point)
{
	attachTarget = point;
}

bool Button::isAttached() const
{
	return attachTarget;
}

sf::Vector2f Button::attachedTo() const
{
	if (isAttached())
		return *attachTarget;
	return sf::Vector2f(0, 0);
}

}