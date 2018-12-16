#include "ButtonManager.h"

namespace kiwi
{
namespace v1
{

ApplicationEvent defaultConverter(sf::Event e, Button::ButtonAction action, ButtonHandle handle)
{
	ApplicationEvent appEvent;

	appEvent.type = ApplicationEvent::Type::Button;
	appEvent.subtype = action;
	appEvent.w.i = handle.category();
	appEvent.z.i = handle.index();

	return appEvent;
}

//=====ButtonHandle=====
bool ButtonHandle::operator!() const
{
	return !manager;
}

//explicit
ButtonHandle::operator bool() const
{
	return manager;
}

Button& ButtonHandle::operator*() const
{
	return getManager()[*this];
}

Button* ButtonHandle::operator->() const
{
	return &(**this);
}

void ButtonHandle::enable() const
{
	getManager().activate(categoryIndex, true);
}

void ButtonHandle::disable() const
{
	getManager().activate(categoryIndex, false);
}

bool ButtonHandle::activation() const
{
	return getManager().activation(categoryIndex);
}

void ButtonHandle::activate(bool set) const
{
	getManager().activate(categoryIndex, set);
}

void ButtonHandle::setEventFactory(AppEventFactory fact) const
{
	(**this);
}

int ButtonHandle::category() const
{
	return categoryIndex;
}

int ButtonHandle::index() const
{
	return buttonIndex;
}

//private
ButtonHandle::ButtonHandle(int cat, int btn, ButtonManager* man)
{
	categoryIndex = cat;
	buttonIndex = btn;
	manager = man;
}

//private
ButtonManager& ButtonHandle::getManager() const
{
	if (!manager)
	{
		std::string err = "Invalid ButtonHandle: ";
		err += std::to_string(categoryIndex);
		err += ", ";
		err += std::to_string(buttonIndex);
		throw std::logic_error(err);
	}
	return *manager;
}


//=====ButtonManager======

Button& ButtonManager::operator[](const ButtonHandle& at)
{
	return container[at.category()].buttons[at.index()];
}

void ButtonManager::enable(int index)
{
	this->activate(index, true);
}

void ButtonManager::disable(int index)
{
	this->activate(index, false);
}

bool ButtonManager::activation(int index) const
{
	return container[index].active;
}

void ButtonManager::activate(int index, bool set)
{
	container[index].active = set;
}

void ButtonManager::setEventFactory(int index, AppEventFactory fact)
{
	container[index].converter = fact;
}

void ButtonManager::setEventFactory(AppEventFactory fact)
{
	converter = fact;
}

ButtonHandle ButtonManager::getHandle(int category, int index) const
{
	int size = container.size();

	category = (category < 0) ? category + size : category;

	if (category >= size || category < 0)
		return ButtonHandle();//invalid handle
	
	size = container[category].buttons.size();

	index = (index < 0) ? index + size : index;

	if (index >= size || index < 0)
		return ButtonHandle();//invalid handle

	return ButtonHandle(category, index, const_cast<ButtonManager*>(this));
}

//defaule argument: index = -1
ButtonHandle ButtonManager::addButton(int index)
{
	ButtonHandle test = getHandle(index, 0);

	if (!test)
		return test;

	container[test.category()].buttons.push_back(Button());

	return getHandle(index, -1);
}

ButtonHandle ButtonManager::addCategory()
{
	container.push_back(ButtonCategory());
	return getHandle(-1, 0);
}

ApplicationEvent ButtonManager::test(sf::Event e)
{
	ApplicationEvent retval;
	Button::ButtonAction action = Button::NONE;
	for (std::size_t i = 0; i < container.size(); i++)
	{
		if (container[i].active)
		{
			for (std::size_t j = 0; j < container[i].buttons.size(); j++)
			{
				action = container[i].buttons[j].triggerTest(e);
				if (action != Button::NONE)
				{
					return transform(e, i, j, action);
				}
			}
		}
	}

	return noCaptureConverter(e);
}

ApplicationEvent ButtonManager::transform(sf::Event e, int category, int index, Button::ButtonAction act) const
{
	if (container[category].converter)
		return container[category].converter(e, act, getHandle(category, index));

	if(converter)
		return converter(e, act, getHandle(category, index));
	
	return defaultConverter(e, act, getHandle(category, index));
}

}
}