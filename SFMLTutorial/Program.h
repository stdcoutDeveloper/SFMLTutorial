#pragma once
#include "Shapes.h"
#include "Mushroom.h"

namespace SFMLTutorial
{
    class Program
    {
    public:
        Program() : window_(sf::VideoMode(640, 480), "SFML Tutorial")
        {
        }

        void Start()
        {
            // Shapes rect;
            Mushroom mush;

            while (window_.isOpen())
            {
                sf::Event event;
                while (window_.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed) // close window button clicked?
                        window_.close();
                }
                window_.clear(sf::Color(16, 16, 16, 255));

                // draw
                // window_.draw(rect.GetRectangle());
                mush.Bounce(window_.getSize().x, window_.getSize().y);
                window_.draw(mush.GetMushroom());

                window_.display();
            }
        }

    private:
        sf::RenderWindow window_;
    };
}
