#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

namespace kiwi
{

class Frame : public sf::Drawable
{
public:
	Frame(const sf::Texture* srcTexture, int count, sf::PrimitiveType type = sf::TriangleFan);
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
	Frame frame(int index, const std::vector<sf::Vector2f>& outline, sf::PrimitiveType type = sf::TriangleFan) const;
};

class AnimatedBase
{
public:

	AnimatedBase() = default;
	explicit AnimatedBase(const Animation* srcAnimation, int offset = 0, bool go = true);

	~AnimatedBase() = default;

	const Animation* getAnimation() const;

	void setAnimation(const Animation* srcAnimation, int offset = 0, bool go = true);

	//Starts annimation at desired frame
	void play(int offset = 0);
	
	//Stops animation in place
	void pause();

	//Sets frame without adjusting running status
	void setFrame(int offset);

	bool isRunning() const;

	int getFrameNumber() const;

	void drawAnimated(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawAnimated(sf::RenderTarget& target, sf::RenderStates states, const std::vector<sf::Vector2f>& outline, sf::PrimitiveType type = sf::TriangleFan) const;

private:

	const Animation* animation = nullptr;

	union ChronoData
	{
		ChronoData(); //Default constructor
		std::chrono::steady_clock::time_point timer;
		int frame = 0;
	} chrono;
	bool running = false;
};

}