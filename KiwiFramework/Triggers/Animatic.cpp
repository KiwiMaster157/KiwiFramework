#include "Animatic.h"

namespace kiwi
{

//default arg: type = sf::TriangleFan
Frame::Frame(const sf::Texture* srcTexture, int count, sf::PrimitiveType type)
	: texture(srcTexture), varray(type, count)
{}

void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture;
	target.draw(varray, states);
}

sf::Vertex& Frame::operator[](int x)
{
	return varray[x];
}

//=====Animation=====
Frame Animation::frame(int index) const
{
	const float width = static_cast<float>(size.x);
	const float height = static_cast<float>(size.y);

	//Uses TriangleFan
	return frame(index, {
		sf::Vector2f(0, 0),
		sf::Vector2f(0, height),
		sf::Vector2f(width, height),
		sf::Vector2f(width, 0) });
}

//default arg: type = sf::TriangleFan
Frame Animation::frame(int index, const std::vector<sf::Vector2f>& outline, sf::PrimitiveType type) const
{
	if (index < 0)
		throw std::out_of_range("Negative frame number does not exist.");

	if (index < count)
	{
		Frame retval(texture, outline.size(), type);

		int row = index / columns;
		int col = index % columns;

		sf::Vector2f texOrigin = sf::Vector2f(
			static_cast<float>(origin.x + size.x * col),
			static_cast<float>(origin.y + size.y * row));

		for (int i = 0; i < (int)outline.size(); i++)
		{
			retval[i].position = outline[i];
			retval[i].texCoords = outline[i] + texOrigin;
		}

		return retval;
	}

	if (looping)
		return frame(index % count, outline, type);
	//else
	return frame(count-1, outline, type);
}

//=====AnimatedBase=====

//default arg: offset = 0, go = true
AnimatedBase::AnimatedBase(const Animation* srcAnimation, int offset, bool go)
{
	setAnimation(srcAnimation, offset, go);
}

const Animation* AnimatedBase::getAnimation() const
{
	return animation;
}

//default arg: offset = 0, go = true
void AnimatedBase::setAnimation(const Animation* srcAnimation, int offset, bool go)
{
	animation = srcAnimation;
	if (go)
		play(offset);
	else
		chrono.frame = offset;
}

//default arg: offset = 0
void AnimatedBase::play(int offset)
{
	chrono.timer = std::chrono::steady_clock::now() + animation->period * offset;
	running = true;
}

void AnimatedBase::pause()
{
	if(running)
		chrono.frame = getFrameNumber();
	running = false;
}

void AnimatedBase::setFrame(int offset)
{
	if (running)
		play(offset);
	else
		chrono.frame = offset;
}

bool AnimatedBase::isRunning() const
{
	return running;
}

int AnimatedBase::getFrameNumber() const
{
	return running ? static_cast<int>((std::chrono::steady_clock::now() - chrono.timer) / animation->period) : chrono.frame;
}

void AnimatedBase::drawAnimated(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (animation)
	{
		target.draw(animation->frame(getFrameNumber()), states);
	}
}

//default arg: type = sf::TriangleFan
void AnimatedBase::drawAnimated(sf::RenderTarget& target, sf::RenderStates states, const std::vector<sf::Vector2f>& outline, sf::PrimitiveType type) const
{
	if (animation)
	{
		target.draw(animation->frame(getFrameNumber(), outline, type), states);
	}
}


AnimatedBase::ChronoData::ChronoData()
	: timer()
{}

}