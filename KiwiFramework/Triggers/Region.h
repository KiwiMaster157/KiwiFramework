#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

namespace kiwi
{

class Region
{
public:
	Region() = default;
	Region(const Region&) = default;
	Region(Region&&) = default;

	Region& operator=(const Region&) = default;
	Region& operator=(Region&&) = default;

	void clear();

	bool empty() const;
	int pointCount() const;
	int triangleCount() const;

	const std::vector<sf::Vector2f>& getPoints() const;
	sf::PrimitiveType getType() const;

	sf::Vector2f& operator[](int index);
	const sf::Vector2f& operator[](int index) const;

	Region& insertPoint(sf::Vector2f point);
	Region& insertPoint(float x, float y);
	
	void setPoints(const std::vector<sf::Vector2f>& newPoints);
	void setPoints(std::vector<sf::Vector2f>&& newPoints);
	void setType(sf::PrimitiveType newType);

	bool contains(sf::Vector2f point) const;
	bool contains(float x, float y) const;
	bool contains(int x, int y) const;

protected:

	std::vector<sf::Vector2f> boundaryPoints;
	sf::PrimitiveType type = sf::PrimitiveType::TriangleFan;
};

}