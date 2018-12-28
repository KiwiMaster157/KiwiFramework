#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "Animatic.h"
#include "Region.h"

namespace kiwi
{

class Library
{
public:
	using Key = std::string;

	//get____ returns NULL if not found.

	//load____File returns count inserted
	//  throws std::runtime_error if a file cannot be opened
	//  overwrites existing entities
	//  Empty lines and lines beginning with '#' are treated as comments

	//Texture
	const sf::Texture* getTexture(const Key& name) const;
	void setTexture(const Key& name, const sf::Texture& src);//insert or modify
	void setTexture(const Key& name, sf::Texture&& src);
	//Format: name=file (no spaces)
	int loadTextureFile(const std::string& file);
	void unloadTexture(const Key& name);
	int countTextures() const;
	void clearTextures();

	//Loads texture from file & inserts / replaces.
	bool loadTexture(const Key& name, const std::string& file);//loads texture from file

	//Animation
	const Animation* getAnimation(const Key& name) const;
	void setAnimation(const Key& name, const Animation& src);//insert or modify
	//Format : name=textureName (no spaces)
	//Next ln: originX originY width height columns length period looping (space delimited, looping={1,0}, period in ms)
	int loadAnimationFile(const std::string& file);
	void unloadAnimation(const Key& name);
	int countAnimations() const;
	void clearAnimations();

	//Font
	const sf::Font* getFont(const Key& name) const;
	void setFont(const Key& name, const sf::Font& src);//insert or modify
	void setFont(const Key& name, sf::Font&& src);//insert or modify
	//Format: name=file (no spaces)
	int loadFontFile(const std::string& file);
	void unloadFont(const Key& name);
	int countFonts() const;
	void clearFonts();

	//Loads font from file & inserts / replaces.
	bool loadFont(const Key& name, const std::string& file);//loads texture from file

	//Region
	const Region* getRegion(const Key& name) const;
	void setRegion(const Key& name, const Region& src);//insert or modify
	void setRegion(const Key& name, Region&& src);//insert or modify
	//Format : name=type (no spaces, type={List,Fan,Strip})
	//Next ln: x0 y0 x1 y1 ... xn yn
	//If an odd number of values are on the second line, ignores last one
	int loadRegionFile(const std::string& file);
	void unloadRegion(const Key& name);
	int countRegions() const;
	void clearRegions();

private:
	std::map<Key, sf::Texture> texturesMap;
	std::map<Key, kiwi::Animation> animationsMap;
	std::map<Key, sf::Font> fontsMap;
	std::map<Key, kiwi::Region> regionsMap;

	//if animation does not exist, throws std::domain_error
	void setAnimationTexture(const Key& name, const Key& textureName);
};

}