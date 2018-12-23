#include "Animatic.h"
#include <fstream>
#include <sstream>

namespace kiwi
{
inline namespace v1
{

//=====Animation=====

bool Animation::readFromFile(const std::filesystem::path& file)
{
	std::ifstream fin(file);
	std::string line;
	bool needFile = true;

	if (!fin.is_open())
		return false;

	reelData.clear();

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')//line is comment
			continue;
		if (needFile)
		{
			if (!spriteSheet.loadFromFile(line))
				return false;
			needFile = false;
		}
		else
		{
			reelData.push_back(SpriteRowInfo());
			SpriteRowInfo& last = reelData.back();
			std::stringstream lineData;
			lineData << line;
			lineData >> last.start >> last.height >> last.colWidth >> last.colWidth;
			int millisec;
			lineData >> millisec;
			last.period = sf::milliseconds(millisec);
			if (!lineData)
				return false;
		}
	}

	return !reelData.empty();
}

void Animation::reset()
{
	spriteSheet = sf::Texture();
	reelData.clear();
}

const sf::Texture* Animation::getTexture() const
{
	return &spriteSheet;
}

int Animation::maxRows() const
{
	return reelData.size();
}

const SpriteRowInfo& Animation::operator[](int index) const
{
	return reelData[index];
}

//=====AnimatedSprite=====

AnimatedSprite::AnimatedSprite(const Animation* animation, int row)
{
	animationSheet = animation;
	animationRow = row;
	toDraw.setTexture(*animation->getTexture());
	updateFrame();
}

//default arg: offset = 0
void AnimatedSprite::start(int offset)
{
	int cols = getRowInfo().colCount;
	animationFrame = (offset + cols) % cols;
	running = true;
	timer.restart();
}

void AnimatedSprite::stop()
{
	running = false;
}

bool AnimatedSprite::isRunning() const
{
	return running;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	const_cast<AnimatedSprite&>(*this).updateFrame();
	states.transform *= getTransform();
	target.draw(toDraw, states);
}

const SpriteRowInfo& AnimatedSprite::getRowInfo() const
{
	return (*animationSheet)[animationRow];
}

void AnimatedSprite::updateFrame()
{
	if (isRunning() && timer.getElapsedTime() > getRowInfo().period)
	{
		animationFrame++;
		animationFrame %= getRowInfo().colCount;

		toDraw.setTextureRect(sf::IntRect(
			getRowInfo().colWidth * animationFrame, //left
			getRowInfo().start,    //top
			getRowInfo().colWidth, //width
			getRowInfo().height)); //height
	}
}

}
}