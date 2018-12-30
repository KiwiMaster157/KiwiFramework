#pragma once

#include "Library.h"
#include "Button.h"

#include <queue>
#include <thread>
#include <future>
#include <functional>

namespace kiwi
{

class ApplicationBase
{
public:
	using Duration = std::chrono::steady_clock::duration;
	using TimePoint = std::chrono::steady_clock::time_point;
	using ButtonPredicate = std::function<bool(const std::string&)>;
	using ButtonContainer = std::map<std::string, Button>;

	ApplicationBase() = default;
	ApplicationBase(const ApplicationBase&) = delete;
	ApplicationBase(ApplicationBase&&) = default;

	virtual ~ApplicationBase() = default;

	int run(const std::vector<std::string>& args);
	std::future<int> runAsync(const std::vector<std::string>& args);

protected:

	//Virtual interface

	virtual void initialize(const std::vector<std::string>& args) = 0;
	virtual int finalize() = 0;

	enum EventReturn
	{
		Normal,   //Successfully processed
		FatalError,//Forced crash (will NOT run finalize() before exiting program. Returns -1.)
		Retry,     //Reprocess event.
		Quit,      //Exit program (runs finalize() and exits program)
		Render,    //Run render sequence immediately

		COUNT      //Must be listed last
	};

	virtual EventReturn eventClose() = 0;
	virtual EventReturn eventFocus(bool gained) = 0;
	virtual EventReturn eventResize(int x, int y) = 0;
	virtual EventReturn eventText(sf::Uint32 unicode) = 0;
	virtual EventReturn eventKeyPress(sf::Keyboard::Key key) = 0;
	virtual EventReturn eventKeyHold(sf::Keyboard::Key key) = 0;
	virtual EventReturn eventKeyRelease(sf::Keyboard::Key key) = 0;
	virtual EventReturn eventTimer() = 0;
	virtual EventReturn eventScroll(float delta, sf::Mouse::Wheel wheel) = 0;
	virtual EventReturn eventButton(const ButtonEvent& button) = 0;

	//Run before checking mouse move and button events.
	//Button keys are run through return value to determine if they should be processed.
	//In-out parameter allows changing appparent mouse position.
	//If not defined, coords remain unchanged, and predicate always returns true.
	virtual ButtonPredicate mouseEventPreprocess(sf::Vector2i& coords);
	
	virtual void render() = 0;
	
	//Application management

	//Window
	//No effect if window is already open
	void openWindow(int x, int y, const std::string& title);
	void openWindow(sf::Vector2i size, const std::string& title);
	void closeWindow();
	void resizeWindow(int x, int y);
	void resizeWindow(sf::Vector2i size);
	void setWindowPosition(int x, int y);
	void setWindowPosition(sf::Vector2i position);
	void setWindowTitle(const std::string& title);

	void draw(const sf::Drawable& entity, sf::RenderStates states = sf::RenderStates::Default);
	
	bool isWindowOpen() const;
	sf::Vector2i getWindowSize() const;
	sf::Vector2i getWindowPosition() const;
	std::string getWindowTitle() const;

	//Cursor
	void setSystemCursor(sf::Cursor::Type cursorType);
	void setCustomCursor(const sf::Cursor& customCursor);
	void showCursor(bool visible);
	void setCursorPosition(int x, int y);
	void setCursorPosition(sf::Vector2i position);
	
	sf::Vector2i getCursorPosition() const;
	bool isCursorVisible() const;

	//Timer Interrupts
	void startTimer(const std::string& key, const Duration& setDuration);
	void startTimer(const std::string& key, const TimePoint& setTime);
	//If more than one timer with same key is present, will pick the one set to expire soonest
	void adjustTimer(const std::string& key, const Duration& timeAdded);
	void adjustTimer(const std::string& key, const TimePoint& setTime);
	void cancelTimer(const std::string& key);

	int countTimers() const;

	//Returns max duration / time_point if timer does not exist

	//Next timer
	std::string nextTimerKey() const;//Returns empty string if no active timers

	Duration remainingTime() const;
	TimePoint endingTime() const;
	
	//Specific timer
	Duration remainingTime(const std::string& key) const;
	TimePoint endingTime(const std::string& key) const;

	//Buttons
	void addButton(const std::string& key);
	//Use empty string for animationKey to disable drawing
	void addButton(const std::string& key, const Library::Key& animationKey, const Library::Key& regionKey);
	void setButton(const std::string& key, const Button& src);
	void removeButton(const std::string& key);

	int loadButtonFile(const std::string& file);

	Button* getButton(const std::string& key);//Returns nullptr if non-existent
	int countButtons() const;
	int countButtons(const ButtonPredicate& pred) const;
	void clearButtons();

	ButtonContainer::iterator buttonsBegin();
	ButtonContainer::iterator buttonsEnd();

	//Misc.

	void clearEventQueue();
	TimePoint getTime() const;

	Library appLibrary; //Direct access to resource manager

private:
	using AppTimer = std::pair<TimePoint, std::string>;

	sf::RenderWindow appWindow;
	std::bitset<sf::Keyboard::KeyCount> appKeys;
	sf::Cursor appCursor;
	std::vector<AppTimer> appTimers;
	ButtonContainer appButtons; //std::map<std::string, Button>

	std::string appWindowTitle;
	sf::Vector2i appMouseCoords = sf::Vector2i(0, 0);
	bool appMouseVisible = true;
	std::deque<ButtonEvent> appButtonQueue;
};

}
