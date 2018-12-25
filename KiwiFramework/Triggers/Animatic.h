#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

namespace kiwi
{

class Frame : public sf::Drawable
{
public:
	explicit Frame(const sf::Texture* srcTexture);
	virtual ~Frame() = default;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Vertex& operator[](int x);
private:
	const sf::Texture* texture;
	sf::VertexArray varray;
};

struct Animation
{
	using Period_t = std::chrono::steady_clock::duration;
	using Time_t = std::chrono::steady_clock::time_point;
	const sf::Texture* texture = nullptr;
	int columns = 0;
	int count = 0;
	sf::Vector2i origin = { 0, 0 };
	sf::Vector2i size = { 0, 0 };
	Period_t period = std::chrono::seconds(0);
	bool looping = true;

	Frame frame(int index) const;
};

//Prefer composition over inheritance
class AnimatedSprite: public sf::Drawable, public sf::Transformable
{
public:

	AnimatedSprite() = default;
	explicit AnimatedSprite(const Animation* srcAnimation);
	AnimatedSprite(const Animation* srcAnimation, int offset = 0, bool go = true);

	virtual ~AnimatedSprite() = default;

	const Animation* getAnimation() const;

	void setAnimation(const Animation* srcAnimation, int offset = 0, bool go = true);

	//Starts annimation at desired frame
	void start(int offset = 0);
	
	//Stops animation in place
	void stop();

	//Sets frame without adjusting running status
	void set(int offset);

	bool isRunning() const;

	int getFrameNumber() const;

	void drawAnimated(sf::RenderTarget& target, sf::RenderStates states) const;

	//From sf::Drawable:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	const Animation* animation = nullptr;

	union ChronoData
	{
		ChronoData();
		std::chrono::steady_clock::time_point timer;
		int frame = 0;
	} chrono;
	bool running = false;
};

}