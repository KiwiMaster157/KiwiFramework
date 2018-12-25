#pragma once

#include "Trigger.h"
#include <functional>

namespace kiwi
{
namespace utility
{

enum class ToggleMode
{
	Enable,
	Disable,
	Toggle,
	Steady
};

inline void enableAgent(kiwi::Toggleable& tog)
{
	tog.activated = true;
}

inline void disableAgent(kiwi::Toggleable& tog)
{
	tog.activated = false;
}

inline void swapAgent(kiwi::Toggleable& tog)
{
	tog.activated = !tog.activated;
}

inline std::function<void(kiwi::Toggleable&)> toggleAgent(ToggleMode m)
{
	switch (m)
	{
	case ToggleMode::Enable:
		return enableAgent;
	case ToggleMode::Disable:
		return disableAgent;
	case ToggleMode::Toggle:
		return swapAgent;
	default:
		return [](kiwi::Toggleable&) {};
	}
}

}
}