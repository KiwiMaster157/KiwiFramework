#include "Region.h"
#include <string>

namespace kiwi
{

static bool triangleContains(const std::array<sf::Vector2f, 3>& triangle, sf::Vector2f point);

//Used in triangleContains calculations
static float det(sf::Vector2f u, sf::Vector2f v);


void Region::clear()
{
	boundaryPoints.clear();
	type = sf::PrimitiveType::TriangleFan;
}

bool Region::empty() const
{
	return boundaryPoints.empty();
}

int Region::pointCount() const
{
	return boundaryPoints.size();
}

int Region::triangleCount() const
{
	if (type == sf::PrimitiveType::Triangles)
		return static_cast<int>(pointCount() / 3);
	return pointCount() < 2 ? 0 : pointCount() - 2;
}

const std::vector<sf::Vector2f>& Region::getPoints() const
{
	return boundaryPoints;
}

sf::PrimitiveType Region::getType() const
{
	return type;
}

sf::Vector2f& Region::operator[](int index)
{
	return boundaryPoints[(index + pointCount()) % pointCount()];
}

const sf::Vector2f& Region::operator[](int index) const
{
	return boundaryPoints[(index + pointCount()) % pointCount()];
}

Region& Region::insertPoint(sf::Vector2f point)
{
	boundaryPoints.push_back(point);
	return *this;
}

Region& Region::insertPoint(float x, float y)
{
	return insertPoint(sf::Vector2f(x, y));
}

void Region::setPoints(const std::vector<sf::Vector2f>& newPoints)
{
	boundaryPoints = newPoints;
}

void Region::setPoints(std::vector<sf::Vector2f>&& newPoints)
{
	boundaryPoints = std::move(newPoints);
}

void Region::setType(sf::PrimitiveType newType)
{
	type = newType;
}

bool Region::contains(sf::Vector2f point) const
{
	std::array<sf::Vector2f, 3> triangle;

	for (int i = 0; i < triangleCount(); i++)
	{
		switch (type)
		{
		case sf::Triangles:
			triangle[0] = boundaryPoints[3 * i];
			triangle[1] = boundaryPoints[3 * i + 1];
			triangle[2] = boundaryPoints[3 * i + 2];
			break;
		case sf::TriangleFan:
			triangle[0] = boundaryPoints[0];
			triangle[1] = boundaryPoints[i + 1];
			triangle[2] = boundaryPoints[i + 2];
			break;
		case sf::TriangleStrip:
			triangle[0] = boundaryPoints[i];
			triangle[1] = boundaryPoints[i + 1];
			triangle[2] = boundaryPoints[i + 2];
			break;
		default:
			throw std::logic_error(std::string("Unexpected sf::PrimitiveType = ") + std::to_string(type));
		}

		if (triangleContains(triangle, point))
			return true;
	}
	return false;
}

bool Region::contains(float x, float y) const
{
	return contains(sf::Vector2f(x, y));
}

bool Region::contains(int x, int y) const
{
	return contains(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
}

//Non-member function
/*
http://mathworld.wolfram.com/TriangleInterior.html
*/
static bool triangleContains(const std::array<sf::Vector2f, 3>& triangle, sf::Vector2f point)
{
	sf::Vector2f v1 = triangle[1] - triangle[0];
	sf::Vector2f v2 = triangle[2] - triangle[0];

	float denominator = det(v1, v2);

	float a = (det(point, v2) - det(triangle[0], v2)) / denominator;
	float b = -(det(point, v1) - det(triangle[0], v1)) / denominator;

	return a > 0 && b > 0 && a + b < 1;
}

static float det(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.y - u.y * v.x;
}


}