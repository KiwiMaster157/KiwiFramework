#include "Trigger.h"
#include "Manager.h"
#include "Agent.h"
#include "Button.h"
#include "Library.h"

#include <iostream>

kiwi::Button makeButton(const std::string& name, kiwi::Library& lib, const kiwi::Library::Key& animation, const kiwi::Library::Key& region)
{
	return kiwi::Button(name, lib.getAnimation(animation), lib.getRegion(region));
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& rhs)
{
	os << rhs.x << ", " << rhs.y;
	return os;
}

int main()
{
	try
	{
		sf::RenderWindow window(sf::VideoMode(720, 640), "Animation test", sf::Style::Default);

		kiwi::Library lib;

		lib.loadTextureFile("textureTest.txt");
		lib.loadAnimationFile("animationTest.txt");
		lib.loadRegionFile("regionTest.txt");

		kiwi::Button sprite = makeButton("testButton", lib, "test2", "test2");
		sf::Vector2f point{ 0, 0 };

		sprite.scale(10, 10);
		sprite.move(100, 100);
		sprite.attach(&point);

		while (window.isOpen())
		{
			kiwi::ButtonEvent be;
			sf::Event e;
			while (window.pollEvent(e))
			{
				switch (e.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				default:
					if (sprite.trigger(e, be))
					{
						switch (be.type)
						{
						case kiwi::ButtonEvent::Press:
							std::cout << "Clicked" << std::endl;
							point += sf::Vector2f(10, 10);
							break;
						case kiwi::ButtonEvent::Release:
							std::cout << "Unclicked" << std::endl;
						case kiwi::ButtonEvent::Reset:
							point -= sf::Vector2f(10, 10);
							break;
						case kiwi::ButtonEvent::HoverBegin:
							std::cout << "Hover Begin" << std::endl;
							break;
						case kiwi::ButtonEvent::HoverMove:
							std::cout << (be.btns.any() ? "Drag" : "Hover") << std::endl;
							break;
						case kiwi::ButtonEvent::HoverEnd:
							std::cout << "Hover End" << std::endl;
							break;
						}
					}
				}
			}

			window.clear();

			window.draw(sprite);

			window.display();
		}
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << std::endl;
		std::cin.ignore();
	}

	return 0;
}
