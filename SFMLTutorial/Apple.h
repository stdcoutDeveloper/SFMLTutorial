#pragma once

#include "pch.h"

namespace SFMLTutorial
{
    class Apple
    {
    public:
        Apple()
        {
            apple_shape_.setFillColor(sf::Color::Red);
            apple_shape_.setRadius(size_ / 2.0f);
        }

        sf::CircleShape& GetApple()
        {
            return apple_shape_;
        }

        void SetPosition(float x, float y)
        {
            apple_shape_.setPosition(x, y);
        }

    private:
        sf::CircleShape apple_shape_;
        sf::Vector2i position_;
        int size_ = 16;
    };
}
