#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

namespace kiwi
{
inline namespace v1
{

struct SpriteRowInfo
{
	int start = 0;
	int height = 0;
	int colWidth = 0;
	int colCount = 0;
	sf::Time period = sf::seconds(0);
};

class Animation
{
public:
	Animation() = default;

	Animation(const Animation&) = delete;
	Animation(Animation&&) = default;

	//When reading from a file, blank lines and lines starting with # are treated as comments
	//The first meaningful line will be taken as the image file
	//Each line following consists of row info, with time in milliseconds
	//If there is an issue reading the file, returns false and leaves animation in an undefined state
	bool readFromFile(const std::filesystem::path& file);

	void reset();

	const sf::Texture* getTexture() const;

	int maxRows() const;

	//Throws if out of bounds
	const SpriteRowInfo& operator[](int index) const;

private:
	sf::Texture spriteSheet;
	std::vector<SpriteRowInfo> reelData;
};

//Prefer composition over inheritance
struct AnimatedSprite: public sf::Drawable, public sf::Transformable
{
public:

	AnimatedSprite() = default;
	AnimatedSprite(const Animation* animation, int row);

	virtual ~AnimatedSprite() = default;

	void start(int offset = 0);

	void stop();

	bool isRunning() const;

	//From sf::Drawable:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

private:

	const SpriteRowInfo& getRowInfo() const;
	void updateFrame();

	sf::Sprite toDraw;

	const Animation* animationSheet = nullptr;
	int animationRow = 0;

	int animationFrame = 0;
	sf::Clock timer;
	bool running = false;
};

}
}