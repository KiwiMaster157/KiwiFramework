#pragma once

#include "Trigger.h"
#include "Manager.h"

#include <queue>


namespace kiwi
{

template<typename TValue>
struct AgentBase
{
	using Value = TValue;

	virtual ~AgentBase() = default;

	virtual void operator()(Value&) = 0;
};

template<typename TIn, typename TOut, typename Container = std::queue<TOut>>
class TriggerAgent;

template<typename TIn, typename TOut, typename Container = std::queue<TOut>>
class TriggerAgentHelper : public AgentBase<TriggerBase<TIn,TOut>>
{
public:

	using Master = TriggerAgent<TIn, TOut, Container>;

	TriggerAgentHelper(Master* myMaster, const TIn* input)
	{
		master = myMaster;
		event = input;
	}

	virtual ~TriggerAgentHelper() = default;

	//From AgentBase:
	virtual void operator()(TriggerBase<TIn, TOut>& val) override
	{
		TOut out;
		if (val.trigger(*event, out))
			master->outputs.push(out);
	}

private:
	Master* master;
	const TIn* event;
};

//Default: Container = std::queue<TOut>
template<typename TIn, typename TOut, typename Container>
class TriggerAgent
{
public:

	using Helper = TriggerAgentHelper<TIn, TOut, Container>;

	friend Helper;

	virtual ~TriggerAgent() = default;

	Helper operator()(const TIn& input)
	{
		return Helper(this, &input);
	}

	void operator()(typename TriggerBase<TIn, TOut>&)
	{
		throw std::logic_error("TriggerAgent uses a helper class. Do not pass values directly.");
	}

	bool empty() const
	{
		return outputs.empty();
	}

	TOut next()
	{
		TOut retval = outputs.front();
		outputs.pop();
		return retval;
	}

	void clear()
	{
		outputs.clear();
	}

private:
	Container outputs;
};

class RenderAgent : public AgentBase<sf::Drawable>
{
public:
	
	virtual ~RenderAgent() = default;

	//From AgentBase:
	virtual void operator()(sf::Drawable& val) override
	{
		target->draw(val, states);
	}

private:
	sf::RenderTarget* target;
	sf::RenderStates states;
};

}