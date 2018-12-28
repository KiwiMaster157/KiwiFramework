#include "Library.h"
#include <fstream>
#include <sstream>

namespace kiwi
{

int Library::loadTextureFile(const std::string& file)
{
	std::ifstream fin;

	fin.open(file);

	if (!fin)
		throw std::runtime_error(std::string("No such file exists: ") + file);

	int count = 0;
	std::string line;

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')
			continue; //Line is comment
		count++;
		int eqSign = line.find("=");
		if (eqSign == std::string::npos)
			throw std::logic_error(std::string("Syntax error: expected '=' in ") + file);
		if (!texturesMap[line.substr(0, eqSign)].loadFromFile(line.substr(eqSign + 1)))
			throw std::runtime_error(std::string("No such file exists: ") + line.substr(eqSign + 1));
	}

	return count;
}

int Library::loadAnimationFile(const std::string& file)
{
	std::ifstream fin;

	fin.open(file);

	if (!fin)
		throw std::runtime_error(std::string("No such file exists: ") + file);

	//true =  name & texture
	//false = details
	bool lineType = true;
	int count = 0;
	std::string line;
	std::string name;

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')
			continue; //Line is comment
		if (lineType)
		{
			count++;
			int eqSign = line.find("=");
			if (eqSign == std::string::npos)
				throw std::logic_error(std::string("Syntax error: expected '=' in ") + file);
			name = line.substr(0, eqSign);
			animationsMap[name];
			setAnimationTexture(name, line.substr(eqSign + 1));
		}
		else
		{
			Animation& ref = animationsMap[name];
			int timeInMS;
			std::stringstream lineStream;
			lineStream << line;
			lineStream >> ref.origin.x >> ref.origin.y
				>> ref.size.x >> ref.size.y >> ref.columns
				>> ref.count >> timeInMS >> ref.looping;
			ref.period = std::chrono::milliseconds(timeInMS);
			if(!lineStream)
				throw std::logic_error(std::string("Syntax error: bad arguments in ") + file);
		}
		lineType = !lineType;
	}

	return count;
}

int Library::loadFontFile(const std::string& file)
{
	std::ifstream fin;

	fin.open(file);

	if (!fin)
		throw std::runtime_error(std::string("No such file exists: ") + file);

	int count = 0;
	std::string line;

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')
			continue; //Line is comment
		count++;
		int eqSign = line.find("=");
		if (eqSign == std::string::npos)
			throw std::logic_error(std::string("Syntax error: expected '=' in ") + file);
		if (!fontsMap[line.substr(0, eqSign)].loadFromFile(line.substr(eqSign + 1)))
			throw std::runtime_error(std::string("No such file exists: ") + line.substr(eqSign + 1));
	}

	return count;
}

int Library::loadRegionFile(const std::string& file)
{
	std::ifstream fin;

	fin.open(file);

	if (!fin)
		throw std::runtime_error(std::string("No such file exists: ") + file);

	//true =  name & texture
	//false = details
	bool lineType = true;
	int count = 0;
	std::string line;
	std::string name;

	while (std::getline(fin, line))
	{
		if (!line.length() || line[0] == '#')
			continue; //Line is comment
		if (lineType)
		{
			count++;
			int eqSign = line.find("=");
			if (eqSign == std::string::npos)
				throw std::logic_error(std::string("Syntax error: expected '=' in ") + file);
			name = line.substr(0, eqSign);
			std::string type = line.substr(eqSign + 1);
			if (type == "List")
				regionsMap[name].setType(sf::Triangles);
			else if (type == "Fan")
				regionsMap[name].setType(sf::TriangleFan);
			else if (type == "Strip")
				regionsMap[name].setType(sf::TriangleStrip);
			else
					throw std::logic_error(type + " is not a valid region type.");
		}
		else
		{
			Region& ref = regionsMap[name];
			std::stringstream lineStream;
			lineStream << line;
			float x, y;
			while (lineStream >> x >> y)
				ref.insertPoint(x, y);
		}
		lineType = !lineType;
	}

	return count;
}

}