#include "Library.h"

namespace kiwi
{

//===== Texture =====

const sf::Texture* Library::getTexture(const Key& name) const
{
	auto ref = texturesMap.find(name);
	if (ref != texturesMap.end())
		return &(ref->second);
	return nullptr;
}

void Library::setTexture(const Key& name, const sf::Texture& src)
{
	texturesMap[name] = src;
}

void Library::setTexture(const Key& name, sf::Texture&& src)
{
	texturesMap[name] = std::move(src);
}

//loadTextureFile defined in LibraryLoad.cpp

void Library::unloadTexture(const Key& name)
{
	texturesMap.erase(name);
}

int Library::countTextures() const
{
	return texturesMap.size();
}

void Library::clearTextures()
{
	texturesMap.clear();
}

bool Library::loadTexture(const Key& name, const std::string& file)
{
	sf::Texture temp;
	if (temp.loadFromFile(file))
	{
		texturesMap[name] = std::move(temp);
		return true;
	}
	
	return false;
}

//===== Animation =====
const Animation* Library::getAnimation(const Key& name) const
{
	auto ref = animationsMap.find(name);
	if (ref != animationsMap.end())
		return &(ref->second);
	return nullptr;
}

void Library::setAnimation(const Key& name, const Animation& src)
{
	animationsMap[name] = src;
}

//loadAnimationFile defined in LibraryLoad.cpp

void Library::unloadAnimation(const Key& name)
{
	animationsMap.erase(name);
}

int Library::countAnimations() const
{
	return animationsMap.size();
}

void Library::clearAnimations()
{
	animationsMap.clear();
}

//===== Font =====

const sf::Font* Library::getFont(const Key& name) const
{
	auto ref = fontsMap.find(name);
	if (ref != fontsMap.end())
		return &(ref->second);
	return nullptr;
}

void Library::setFont(const Key& name, const sf::Font& src)
{
	fontsMap[name] = src;
}

void Library::setFont(const Key& name, sf::Font&& src)
{
	fontsMap[name] = std::move(src);
}

//loadFontFile defined in LibraryLoad.cpp

void Library::unloadFont(const Key& name)
{
	fontsMap.erase(name);
}

int Library::countFonts() const
{
	return fontsMap.size();
}

void Library::clearFonts()
{
	fontsMap.clear();
}

bool Library::loadFont(const Key& name, const std::string& file)
{
	sf::Font temp;
	if (temp.loadFromFile(file))
	{
		fontsMap[name] = std::move(temp);
		return true;
	}

	return false;
}

//===== Region =====
const Region* Library::getRegion(const Key& name) const
{
	auto ref = regionsMap.find(name);
	if (ref != regionsMap.end())
		return &(ref->second);
	return nullptr;
}

void Library::setRegion(const Key& name, const Region& src)
{
	regionsMap[name] = src;
}

void Library::setRegion(const Key& name, Region&& src)
{
	regionsMap[name] = std::move(src);
}

//loadRegionFile defined in LibraryLoad.cpp

void Library::unloadRegion(const Key& name)
{
	regionsMap.erase(name);
}

int Library::countRegions() const
{
	return regionsMap.size();
}

void Library::clearRegions()
{
	regionsMap.clear();
}

//===== Private =====

void Library::setAnimationTexture(const Key& name, const Key& textureName)
{
	if (!animationsMap.count(name))
		throw std::domain_error(std::string("\"") + name + "\" is not a valid animation name.");

	if (!textureName.size())
		animationsMap[name].texture = nullptr;
	else if (!texturesMap.count(textureName))
		throw std::domain_error(std::string("\"") + name + "\" is not a valid texture name.");
	else
		animationsMap[name].texture = getTexture(textureName);
}

}