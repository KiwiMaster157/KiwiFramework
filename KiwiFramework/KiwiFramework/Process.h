#pragma once

namespace kiwi
{
inline namespace v1
{

///Returned from Framework::process() to determine next course of action
enum class ProcessStatus : char
{
	Success,   //Successfully processed
	Failure,   //Generic failure
	BadEvent,  //Failed due to bad event
	Ignored,   //Event not processed
	FatalError,//Forced crash (will NOT run finish())
	Retry,     //Reprocess event
	Quit,      //Exit program (runs finish())
	Render,    //Run render sequence now

	COUNT      //Must be listed last
};

}//namespace v1
}//namesapce kiwi
