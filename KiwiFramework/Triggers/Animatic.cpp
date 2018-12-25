#include "Animatic.h"

namespace kiwi
{

Frame::Frame(const sf::Texture* srcImage)
	: texture(srcImage), varray(sf::TriangleStrip, 4)
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
	if (index < 0)
		throw std::out_of_range("Negative frame number does not exist.");

	if (index <= count)
	{
		Frame retval(texture);

		int row = index / columns;
		int col = index % columns;

		float left = static_cast<float>(origin.x + size.x * col);
		float top  = static_cast<float>(origin.y + size.y * row);
		float width = static_cast<float>(size.x);
		float height = static_cast<float>(size.y);

		//Set position
		retval[0].position = sf::Vector2f(0, 0);
		retval[1].position = sf::Vector2f(0, height);
		retval[2].position = sf::Vector2f(width, 0);
		retval[3].position = sf::Vector2f(width, height);

		//Set texCoords
		retval[0].texCoords = sf::Vector2f(left, top);
		retval[1].texCoords = sf::Vector2f(left, top + height);
		retval[2].texCoords = sf::Vector2f(left + width, top);
		retval[3].texCoords = sf::Vector2f(left + width, top + height);

		return retval;
	}

	if (looping)
		return frame(index % count);
	//else
	return frame(count);
}

//=====AnimatedSprite=====

AnimatedSprite::AnimatedSprite(const Animation* srcAnimation)
{
	animation = srcAnimation;
	chrono.frame = 0;
}

//default arg: offset = 0, go = true
AnimatedSprite::AnimatedSprite(const Animation* srcAnimation, int offset, bool go)
{
	setAnimation(srcAnimation, offset, go);
}

const Animation* AnimatedSprite::getAnimation() const
{
	return animation;
}

//default arg: offset = 0, go = true
void AnimatedSprite::setAnimation(const Animation* srcAnimation, int offset, bool go)
{
	animation = srcAnimation;
	if (go)
		start(offset);
	else
		chrono.frame = offset;
}

//default arg: offset = 0
void AnimatedSprite::start(int offset)
{
	chrono.timer = std::chrono::steady_clock::now() + animation->period * offset;
	running = true;
}

void AnimatedSprite::stop()
{
	if(running)
		chrono.frame = getFrameNumber();
	running = false;
}

void AnimatedSprite::set(int offset)
{
	if (running)
		start(offset);
	else
		chrono.frame = offset;
}

bool AnimatedSprite::isRunning() const
{
	return running;
}

int AnimatedSprite::getFrameNumber() const
{
	return running ? static_cast<int>((std::chrono::steady_clock::now() - chrono.timer) / animation->period) : chrono.frame;
}

void AnimatedSprite::drawAnimated(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (animation)
	{
		states.transform *= getTransform();
		target.draw(animation->frame(getFrameNumber()), states);
	}
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	drawAnimated(target, states);
}

AnimatedSprite::ChronoData::ChronoData()
	: timer()
{}

}