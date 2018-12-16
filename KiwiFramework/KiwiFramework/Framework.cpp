#include "Framework.h"

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
	running = true;
	int x = runImpl();
	running = false;
	asynchronous = false;
	return x;
}

std::future<int> Framework::runAsync()
{
	asynchronous = true;
	return std::async( [=] () { return run(); } );
}

bool Framework::isRunning() const
{
	return running;
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

}//namespace v1
}//namespace kiwi