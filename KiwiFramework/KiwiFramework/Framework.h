#pragma once

#include "ApplicationEvent.h"
#include "ButtonManager.h"
#include "Process.h"
#include "ConcurrentQueue.h"

#include <thread>

namespace kiwi
{
inline namespace v1
{

///Inherited by Application class in other projects
///Primary responsibilities are the main event loop
///Contains segments with sub-responsibilities
class Framework
{
public:
	//=====Constructors=====
	Framework();
	Framework(const Framework&) = delete;
	Framework(Framework&& src);

	~Framework();

	//=====Public interface=====

	///Executes runImpl in the same thread
	int run();

	///Executes run in a different thread
	int runAsync();

	bool popEvent(ApplicationEvent& e);

	//=====Components=====
	ButtonManager buttons;
	//ResourceManage resources;



	//=====Required implementation functions=====

	///Executed before event loop begins
	virtual bool init() = 0;

	///All events are passed through this function after framework processing
	virtual ProcessStatus process(const ApplicationEvent& event) = 0;

	///Executed at the end of a successful run
	virtual void finish() = 0;

private:

	sf::RenderWindow window;
	ConcurrentQueue<ApplicationEvent> eventQueue;

	///Main event loop
	int runImpl();

	///Initializes the status of the Framework
	bool initFramework();

	void finishFramework();

	volatile bool quit = false;
	bool asynchronous = false;

	void pollEvents();

	//Since the operating system limits polling windows for events
	//  to the window's parent thread, this member can be used for 
	//  ensuring that events are only polled in the correct thread.
	std::thread::id mainThread = std::this_thread::get_id();

};//class Framework

}//namespace v1
}//namespace kiwi