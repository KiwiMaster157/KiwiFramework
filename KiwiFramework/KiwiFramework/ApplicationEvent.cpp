#include "ApplicationEvent.h"
#include "SFML/Window/Event.hpp"

namespace kiwi
{
namespace v1
{

ApplicationEvent noCaptureConverter(sf::Event e)
{
	using AE = ApplicationEvent;
	AE appEvent;

	switch (e.type)
	{
	case sf::Event::Closed:
		appEvent.type = AE::Type::System;
		appEvent.subtype = AE::Subtypes::Closed;
		break;
	case sf::Event::GainedFocus:
		appEvent.type = AE::Type::System;
		appEvent.subtype = AE::Subtypes::GainedFocus;
		break;
	case sf::Event::KeyPressed:
		appEvent.type = AE::Type::Keyboard;
		appEvent.subtype = AE::Subtypes::KeyPressed;
		appEvent.w.i = e.key.code;
		break;
	case sf::Event::KeyReleased:
		appEvent.type = AE::Type::Keyboard;
		appEvent.subtype = AE::Subtypes::KeyReleased;
		appEvent.w.i = e.key.code;
		break;
	case sf::Event::LostFocus:
		appEvent.type = AE::Type::System;
		appEvent.subtype = AE::Subtypes::LostFocus;
		break;
	case sf::Event::MouseButtonPressed:
		appEvent.type = AE::Type::Mouse;
		appEvent.subtype = AE::Subtypes::MouseButtonPressed;
		appEvent.w.i = e.mouseButton.button;
		appEvent.x.i = e.mouseButton.x;
		appEvent.y.i = e.mouseButton.y;
		break;
	case sf::Event::MouseButtonReleased:
		appEvent.type = AE::Type::Mouse;
		appEvent.subtype = AE::Subtypes::MouseButtonReleased;
		appEvent.w.i = e.mouseButton.button;
		appEvent.x.i = e.mouseButton.x;
		appEvent.y.i = e.mouseButton.y;
		break;
	case sf::Event::MouseEntered:
		appEvent.type = AE::Type::Mouse;
		appEvent.subtype = AE::Subtypes::MouseEntered;
		break;
	case sf::Event::MouseMoved:
		appEvent.type = AE::Type::Mouse;
		appEvent.subtype = AE::Subtypes::MouseEntered;
		break;
	case sf::Event::MouseWheelScrolled:
		appEvent.type = AE::Type::Mouse;
		appEvent.subtype = AE::Subtypes::MouseWheelScrolled;
		appEvent.w.i = e.mouseWheelScroll.wheel;
		appEvent.x.i = e.mouseWheelScroll.x;
		appEvent.y.i = e.mouseWheelScroll.y;
		appEvent.z.f = e.mouseWheelScroll.delta;
		break;
	case sf::Event::Resized:
		appEvent.type = AE::Type::System;
		appEvent.subtype = AE::Subtypes::Resized;
		appEvent.x.i = e.size.width;
		appEvent.y.i = e.size.height;
		break;
	case sf::Event::TextEntered:
		appEvent.type = AE::Type::Keyboard;
		appEvent.subtype = AE::Subtypes::TextEntered;
		appEvent.z.i = e.text.unicode;
		break;
	default:
		appEvent.type = AE::Type::Error;
	}

	return appEvent;
}

}
}