#pragma once

#include "ApplicationEvent.h"
#include "ButtonManager.h"
#include "Process.h"
#include <future>


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
	std::future<int> runAsync();

	///Determines whether the application is running
	bool isRunning() const;

	//=====Components=====
	struct
	{
		sf::RenderWindow window;
		ButtonManager buttons;
		//ResourceManage resources;
		

		

	} core;

protected:

	//=====Required implementation functions=====

	///Executed before event loop begins
	virtual bool init() = 0;

	///All events are passed through this function after framework processing
	virtual ProcessStatus process(const ApplicationEvent& event) = 0;

	virtual void finish() = 0;

private:

	///Main event loop
	int runImpl();

	///Initializes the status of the Framework
	bool initFramework();

	void finishFramework();

	bool running = false;
	bool quit = false;
	bool asynchronous = false;

};//class Framework

}//namespace v1
}//namespace kiwi