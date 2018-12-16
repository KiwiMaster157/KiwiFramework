#pragma once

#include <SFML/Window/Event.hpp>
#include <string>

namespace kiwi
{
inline namespace v1
{

///sf::Event is transformed into ApplicationEvent to aid in Framework
struct ApplicationEvent
{
	enum class Type : char
	{
		None,     //No type or unspecified
		Error,    //An error occurred
		System,   //System or window events (e.g. resize or close) 
		Network,  //Network messages
		Button,   //Button interaction occurred
		Keyboard, //Keyboard event not intercepted
		Mouse,    //Mouse event not intercepted
		Raw,      //Miscellaneous raw event not intercepted
		Custom,   //User-defined cusom event (custom type specified in subtype field)
		COUNT //must be listed last
	} type = Type::None;

	int subtype = 0;

	union Field
	{
		int i = 0;
		float f;
	} w, x, y, z;
	
	std::string details;

	struct Subtypes
	{
		enum
		{
			None,
			Closed,
			GainedFocus,
			KeyPressed,
			KeyReleased,
			LostFocus,
			MouseButtonPressed,
			MouseButtonReleased,
			MouseEntered,
			MouseMoved,
			MouseWheelScrolled,
			Resized,
			TextEntered
		};
	};
};

ApplicationEvent noCaptureConverter(sf::Event e);

}//namespace v1
}//namespace kiwi