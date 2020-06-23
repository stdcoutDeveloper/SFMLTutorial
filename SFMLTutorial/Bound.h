#pragma once

#include "pch.h"

namespace SFMLTutorial
{
    class Bound
    {
    public:
        Bound()
        {
            bound_shape_.setFillColor(sf::Color(150, 0, 0));
        }

        sf::RectangleShape& GetBound()
        {
            return bound_shape_;
        }

        void SetSize(sf::Vector2f size)
        {
            bound_shape_.setSize(size);
        }

        void SetOrigin(const sf::Vector2f& origin)
        {
            bound_shape_.setOrigin(origin);
        }

        void SetPosition(const sf::Vector2f& position)
        {
            bound_shape_.setPosition(position);
        }

        void SetPosition(float x, float y)
        {
            bound_shape_.setPosition(x, y);
        }

        const sf::Vector2f& GetSize() const
        {
            return bound_shape_.getSize();
        }

    private:
        sf::RectangleShape bound_shape_;
        int element_size_ = 16; // size of a square block.
    };
}
