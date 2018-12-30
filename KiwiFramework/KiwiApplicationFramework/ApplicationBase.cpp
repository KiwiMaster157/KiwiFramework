#include "ApplicationBase.h"

#include <algorithm>
#include <fstream>

namespace kiwi
{

int ApplicationBase::run(const std::vector<std::string>& args)
{
	appCursor.loadFromSystem(sf::Cursor::Arrow);
	
	initialize(args);

	sf::Event systemEvent;
	EventReturn status = EventReturn::Normal;

	while (status != EventReturn::Quit)
	{
		//Rendering
		appWindow.clear();

		render();

		appWindow.display();

		//Event handling

		while ([&]()->bool
			{
				if (status == EventReturn::Quit || status == EventReturn::Render || status == EventReturn::FatalError)
					return false;
				if (status == EventReturn::Retry)
					return true;
				if (!appButtonQueue.empty())
				{
					appButtonQueue.pop_front();
					if (!appButtonQueue.empty())
						return true;
				}
				return appWindow.pollEvent(systemEvent);
			}())
		{
			if (!appButtonQueue.empty())
				status = eventButton(appButtonQueue.front());
			else switch (systemEvent.type)
			{
			case sf::Event::Closed:
				status = eventClose();
				break;
			case sf::Event::GainedFocus:
				status = eventFocus(true);
				break;
			case sf::Event::LostFocus:
				status = eventFocus(false);
				break;
			case sf::Event::Resized:
				status = eventResize(systemEvent.size.width, systemEvent.size.height);
				break;
			case sf::Event::TextEntered:
				status = eventText(systemEvent.text.unicode);
				break;
			case sf::Event::KeyPressed:
				if (appKeys[systemEvent.key.code])
				{
					status = eventKeyHold(systemEvent.key.code);
				}
				else
				{
					appKeys.set(systemEvent.key.code);
					status = eventKeyPress(systemEvent.key.code);
				}
				break;
			case sf::Event::KeyReleased:
				appKeys.reset(systemEvent.key.code);
				status = eventKeyRelease(systemEvent.key.code);
				break;
			case sf::Event::MouseMoved:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			{
				ButtonEvent btnEvent;
				sf::Vector2i coords;
				coords.x = ((systemEvent.type == sf::Event::MouseMoved) ? systemEvent.mouseMove.x : systemEvent.mouseButton.x);
				coords.y = ((systemEvent.type == sf::Event::MouseMoved) ? systemEvent.mouseMove.y : systemEvent.mouseButton.y);
				ButtonPredicate pred = mouseEventPreprocess(coords);
				((systemEvent.type == sf::Event::MouseMoved) ? systemEvent.mouseMove.x : systemEvent.mouseButton.x) = coords.x;
				((systemEvent.type == sf::Event::MouseMoved) ? systemEvent.mouseMove.y : systemEvent.mouseButton.y) = coords.y;

				for (auto& btn : appButtons)
				{
					if (pred(btn.first))
					{
						if (btn.second.trigger(systemEvent, btnEvent))
						{
							btnEvent.id = btn.first;
							appButtonQueue.push_back(btnEvent);
						}
					}
				}
				status = EventReturn::Normal;
			} break;
				
			//default:	
				//Other events
			}
		}
		if (status == EventReturn::FatalError)
			return -1;

	}

	return finalize();
}

std::future<int> ApplicationBase::runAsync(const std::vector<std::string>& args)
{
	return std::async([=]() {return this->run(args); });
}

//virtual
ApplicationBase::ButtonPredicate ApplicationBase::mouseEventPreprocess(sf::Vector2i& coords)
{
	return [](const std::string&) {return true; };
}

void ApplicationBase::openWindow(int x, int y, const std::string& title)
{
	if (!appWindow.isOpen())
	{
		appWindow.create(sf::VideoMode(x, y), title);
		appWindowTitle = title;
	}
}

void ApplicationBase::openWindow(sf::Vector2i size, const std::string& title)
{
	this->openWindow(size.x, size.y, title);
}

void ApplicationBase::closeWindow()
{
	appWindow.close();
}

void ApplicationBase::resizeWindow(int x, int y)
{
	appWindow.setSize(sf::Vector2u(x, y));
}

void ApplicationBase::resizeWindow(sf::Vector2i size)
{
	appWindow.setSize(static_cast<sf::Vector2u>(size));
}

void ApplicationBase::setWindowPosition(int x, int y)
{
	appWindow.setPosition(sf::Vector2i(x, y));
}

void ApplicationBase::setWindowPosition(sf::Vector2i position)
{
	appWindow.setPosition(position);
}

void ApplicationBase::setWindowTitle(const std::string& title)
{
	appWindow.setTitle(title);
	appWindowTitle = title;
}

void ApplicationBase::draw(const sf::Drawable& entity, sf::RenderStates states)
{
	appWindow.draw(entity, states);
}

bool ApplicationBase::isWindowOpen() const
{
	return appWindow.isOpen();
}

sf::Vector2i ApplicationBase::getWindowSize() const
{
	return static_cast<sf::Vector2i>(appWindow.getSize());
}

sf::Vector2i ApplicationBase::getWindowPosition() const
{
	return appWindow.getPosition();
}

std::string ApplicationBase::getWindowTitle() const
{
	return appWindowTitle;
}

void ApplicationBase::setSystemCursor(sf::Cursor::Type cursorType)
{
	appCursor.loadFromSystem(cursorType);
	appWindow.setMouseCursor(appCursor);
}

void ApplicationBase::setCustomCursor(const sf::Cursor& customCursor)
{
	appWindow.setMouseCursor(customCursor);
}

void ApplicationBase::showCursor(bool visible)
{
	appWindow.setMouseCursorVisible(visible);
	appMouseVisible = visible;
}

void ApplicationBase::setCursorPosition(int x, int y)
{
	sf::Mouse::setPosition(sf::Vector2i(x, y), appWindow);
	appMouseCoords = sf::Vector2i(x, y);
}

void ApplicationBase::setCursorPosition(sf::Vector2i position)
{
	sf::Mouse::setPosition(position, appWindow);
	appMouseCoords = position;
}

sf::Vector2i ApplicationBase::getCursorPosition() const
{
	return appMouseCoords;
}

bool ApplicationBase::isCursorVisible() const
{
	return appMouseVisible;
}

void ApplicationBase::startTimer(const std::string& key, const Duration& setDuration)
{
	TimePoint setTime = std::chrono::steady_clock::now();
	setTime += setDuration;

	appTimers.push_back(AppTimer(setTime, key));
	std::push_heap(appTimers.begin(), appTimers.end());
}

void ApplicationBase::startTimer(const std::string& key, const TimePoint& setTime)
{
	appTimers.push_back(AppTimer(setTime, key));
	std::push_heap(appTimers.begin(), appTimers.end());
}

void ApplicationBase::adjustTimer(const std::string& key, const Duration& timeAdded)
{
	for (std::size_t i = 0; i < appTimers.size(); i++)
	{
		if (appTimers[i].second == key)
		{
			appTimers[i].first += timeAdded;
			std::make_heap(appTimers.begin(), appTimers.end());
			return;
		}
	}
}

void ApplicationBase::adjustTimer(const std::string& key, const TimePoint& setTime)
{
	for (std::size_t i = 0; i < appTimers.size(); i++)
	{
		if (appTimers[i].second == key)
		{
			appTimers[i].first = setTime;
			std::make_heap(appTimers.begin(), appTimers.end());
			return;
		}
	}
}

void ApplicationBase::cancelTimer(const std::string& key)
{
	for (std::size_t i = 0; i < appTimers.size(); i++)
	{
		if (appTimers[i].second == key)
		{
			std::swap(appTimers[i], appTimers.back());
			appTimers.pop_back();
			std::make_heap(appTimers.begin(), appTimers.end());
			return;
		}
	}
}

int ApplicationBase::countTimers() const
{
	return appTimers.size();
}

std::string ApplicationBase::nextTimerKey() const
{
	if (!appTimers.empty())
		return appTimers[0].second;
	return "";
}

ApplicationBase::Duration ApplicationBase::remainingTime() const
{
	if (!appTimers.empty())
		return getTime() - appTimers[0].first;
	return Duration::max();
}

ApplicationBase::TimePoint ApplicationBase::endingTime() const
{
	if (!appTimers.empty())
		return appTimers[0].first;
	return TimePoint::max();
}

ApplicationBase::Duration ApplicationBase::remainingTime(const std::string& key) const
{
	for (std::size_t i = 0; i < appTimers.size(); i++)
	{
		if (appTimers[i].second == key)
		{
			return getTime() - appTimers[i].first;
		}
	}
	return Duration::max();
}

ApplicationBase::TimePoint ApplicationBase::endingTime(const std::string& key) const
{
	for (std::size_t i = 0; i < appTimers.size(); i++)
	{
		if (appTimers[i].second == key)
		{
			return appTimers[i].first;
		}
	}
	return TimePoint::max();
}

void ApplicationBase::addButton(const std::string& key)
{
	appButtons[key];
}

void ApplicationBase::addButton(const std::string& key, const Library::Key& animationKey, const Library::Key& regionKey)
{
	appButtons[key].setAnimation(appLibrary.getAnimation(animationKey));
	appButtons[key].setRegion(appLibrary.getRegion(regionKey));
}

void ApplicationBase::setButton(const std::string& key, const Button& src)
{
	appButtons[key] = src;
}

void ApplicationBase::removeButton(const std::string& key)
{
	appButtons.erase(key);
}

int ApplicationBase::loadButtonFile(const std::string& file)
{
	std::ifstream fin;

	fin.open(file);

	if (!fin)
		throw std::runtime_error(std::string("No such file exists: ") + file);

	int count = 0;
	std::string line;

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')
			continue; //Line is comment
		count++;
		int eqSign = line.find("=");
		int comma = line.find(",");
		if (eqSign == std::string::npos)
			throw std::logic_error(std::string("Syntax error: expected '=' in ") + file);
		if (comma == std::string::npos)
			throw std::logic_error(std::string("Syntax error: expected ',' in ") + file);
		addButton(line.substr(0, eqSign), line.substr(eqSign + 1, comma - eqSign - 1), line.substr(comma + 1));
	}

	return count;
}

Button* ApplicationBase::getButton(const std::string& key)
{
	auto it = appButtons.find(key);
	if (it != appButtons.end())
		return &(it->second);
	return nullptr;
}

int ApplicationBase::countButtons() const
{
	return appButtons.size();
}

int ApplicationBase::countButtons(const ButtonPredicate& pred) const
{
	int counter = 0;
	for (const auto& x : appButtons)
	{
		if (pred(x.first)) counter++;
	}

	return counter;
}

void ApplicationBase::clearButtons()
{
	appButtons.clear();
}

ApplicationBase::ButtonContainer::iterator ApplicationBase::buttonsBegin()
{
	return appButtons.begin();
}

ApplicationBase::ButtonContainer::iterator ApplicationBase::buttonsEnd()
{
	return appButtons.end();
}

void ApplicationBase::clearEventQueue()
{
	sf::Event e;
	while (appWindow.pollEvent(e));
	appButtonQueue.clear();
}

ApplicationBase::TimePoint ApplicationBase::getTime() const
{
	return std::chrono::steady_clock::now();
}

}