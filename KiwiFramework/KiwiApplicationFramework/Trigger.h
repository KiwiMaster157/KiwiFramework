#pragma once

#include <SFML/Graphics.hpp>

namespace kiwi
{

template<typename TIn, typename TOut>
struct TriggerBase
{
	using InputEvent = TIn;
	using OutputEvent = TOut;

	//! Virtual destructor
	virtual ~TriggerBase() = default;

	/** \brief Produces an output event given an input event
	    \param in = event to be processed
		\param out = event produced
		\return true iff out parameter is meaningful
	 */
	virtual bool trigger(const InputEvent& in, OutputEvent& out) = 0;
};

template<typename TIn, typename TOut>
struct ButtonBase : public TriggerBase<TIn, TOut>, public sf::Drawable
{
	virtual ~ButtonBase() = default;
	
	//From TriggerBase:
	virtual bool trigger(const TIn& in, TOut& out) override = 0;

	//From Drawable:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

struct Toggleable
{
	virtual ~Toggleable() = default;

	bool activated = true;
};

}