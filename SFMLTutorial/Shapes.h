#pragma once

#include "pch.h"

namespace SFMLTutorial
{
    class Shapes
    {
    public:
        Shapes() : rect_(sf::Vector2f(128.0f, 128.0f))
        {
            InitRectangle();
        }

        const sf::RectangleShape& GetRectangle() const
        {
            return rect_;
        }

    private:
        sf::RectangleShape rect_;

        void InitRectangle()
        {
            rect_.setFillColor(sf::Color::Red);
            rect_.setPosition(320, 240);
            // set origin is centre of rectangle instead of top left corner (0, 0).
            rect_.setOrigin(rect_.getSize().x / 2, rect_.getSize().y / 2);
        }
    };
}
