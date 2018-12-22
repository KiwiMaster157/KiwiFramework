#include "Framework.h"
#include <future>

namespace kiwi
{
namespace v1
{

Framework::Framework()
{

}

Framework::Framework(Framework&&)
{

}

Framework::~Framework()
{

}

int Framework::run()
{
	asynchronous = false;
	return runImpl();
}

int Framework::runAsync()
{
	asynchronous = true;

	std::future<int> x = std::async([=]() { return runImpl(); });
	sf::Event e;

	while (!x.valid())
	{
		if (window.pollEvent(e))
			eventQueue.push(buttons.test(e));
		/*
		if (network.pollMessage(m))
			eventQueue.push(networkTransform(m));
		*/
	}
	
	return x.get();
}

bool Framework::popEvent(ApplicationEvent& e)
{
	////Temporary implementation
	if (eventQueue.empty())
		return false;
	e = eventQueue.front();
	eventQueue.pop();
	return true;
	////Eventual implementation
	//return eventQueue.pop(e);
}

int Framework::runImpl()
{
	if (!initFramework())
		return -1;
	if (!init())
	{
		finishFramework();
		return -2;
	}

	int retval = 0;
	ApplicationEvent e;

	while (!quit)
	{
		if (!asynchronous) 

		while (true || false)
		{
			switch (process(e))
			{
			case ProcessStatus::Success:

				break;
			case ProcessStatus::Failure:

				break;
			case ProcessStatus::BadEvent:

				break;
			case ProcessStatus::Ignored:

				break;
			case ProcessStatus::FatalError:

				break;
			case ProcessStatus::Retry:

				break;
			case ProcessStatus::Quit:

				break;
			case ProcessStatus::Render:

				break;
			default:
				throw std::logic_error("Unrecognized ProcessStatus");
			}
		}

		/*
		preRender();
		render();
		postRender();
		draw();
		*/
	}

	finish();
	finishFramework();
	return retval;
}

bool Framework::initFramework()
{
	return true;
}

void Framework::finishFramework()
{

}

void Framework::pollEvents()
{
	if (std::this_thread::get_id() != mainThread)
	{
		throw std::logic_error("Events must be polled in the main thread.");
	}

	sf::Event e;
	while (window.pollEvent(e))
	{
		eventQueue.push(buttons.test(e));
	}
}

}//namespace v1
}//namespace kiwi