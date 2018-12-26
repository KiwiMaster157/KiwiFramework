#include "Trigger.h"
#include "Manager.h"
#include "Agent.h"
#include "Animatic.h"

#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(720, 640), "Animation test", sf::Style::Default);
	
	sf::Texture texture;
	texture.loadFromFile("TestAnimation.png");

	kiwi::Animation animation;
	animation.texture = &texture;
	animation.count = 8;
	animation.looping = false;
	animation.size = sf::Vector2i(32, 32);
	animation.columns = 4;
	animation.origin = sf::Vector2i(0, 0);
	animation.period = std::chrono::milliseconds(1000);

	kiwi::AnimatedSprite sprite(&animation);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
			}
		}

		window.clear();

		window.draw(sprite);

		window.display();
	}

	return 0;
}
