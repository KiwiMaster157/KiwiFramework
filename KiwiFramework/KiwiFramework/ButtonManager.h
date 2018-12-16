#pragma once

#include "Button.h"
#include <vector>
#include <string>

namespace kiwi
{
namespace v1 //inline
{

class ButtonManager;
class ButtonHandle;
using AppEventFactory = ApplicationEvent(*)(sf::Event, Button::ButtonAction, ButtonHandle);

///ButtonHandle is used by applications for accessing buttons
class ButtonHandle
{
public:
	friend class ButtonManager;

	ButtonHandle() = default;
	ButtonHandle(const ButtonHandle&) = default;
	ButtonHandle(ButtonHandle&&) = default;

	bool operator!() const;
	explicit operator bool() const;

	Button& operator*() const;
	Button* operator->() const;

	void enable() const;
	void disable() const;
	bool activation() const;
	void activate(bool set) const;

	void setEventFactory(AppEventFactory fact) const;

	int category() const;
	int index() const;

private:
	int categoryIndex = 0;
	int buttonIndex = 0;
	ButtonManager* manager = nullptr;

	ButtonHandle(int cat, int btn, ButtonManager* man);
	ButtonManager& getManager() const;
};



ApplicationEvent defaultConverter(sf::Event e, Button::ButtonAction action, ButtonHandle handle);

struct ButtonCategory
{
	ButtonCategory() = default;
	ButtonCategory(const ButtonCategory&) = delete;
	ButtonCategory(ButtonCategory&&) = default;
	///Buttons do nothing if not active
	bool active = false;
	///Contains all the buttons for checking
	std::vector<Button> buttons = { Button() };
	///if not nullptr, this function will be called to create the application event,
	///    otherwise, a default function is called
	AppEventFactory converter = nullptr;
};

///Manages buttons and activation
class ButtonManager
{
public:
	ButtonManager() = default;
	ButtonManager(const ButtonManager&) = delete;
	ButtonManager(ButtonManager&&) = default;

	Button& operator[](const ButtonHandle& at);
	
	void enable(int index);
	void disable(int index);
	bool activation(int index) const;
	void activate(int index, bool set);

	///Sets the event factory for a single category
	void setEventFactory(int index, AppEventFactory fact);
	///Sets the default event factory
	void setEventFactory(AppEventFactory fact);

	///Returns a valid handle if one exists
	///    otherwise, returns an invalid handle
	ButtonHandle getHandle(int category, int index) const;

	///Creates a new button in specified index if possible
	///    Uses reverse indexing when possible
	///    Returns handle to button if successful
	///    Returns invalid handle if uncuccessful
	ButtonHandle addButton(int index = -1);

	///Adds a new category to end with single button
	///    Returns handle to inserted button
	ButtonHandle addCategory();

	ApplicationEvent test(sf::Event e);

private:
	std::vector<ButtonCategory> container;
	///if not nullptr, this function will be used as the default event factory,
	///    otherwise, uses defaultConverter()
	AppEventFactory converter = nullptr;

	ApplicationEvent transform(sf::Event e, int category, int index, Button::ButtonAction act) const;
};

}//namespace v1
}//namespace kiwi
