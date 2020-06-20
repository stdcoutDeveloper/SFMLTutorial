#pragma once
#include "pch.h"

namespace SFMLTutorial
{
    class Mushroom
    {
    public:
        Mushroom() : increment_(0.4f, 0.4f)
        {
            InitMushroom();
        }

		/**
		 * \brief Bounce mushroom around windows.
		 */
        void Bounce(unsigned int widthWindow, unsigned int heighWindow)
        {
            if ((mushroom_.getPosition().x + size_.x / 2 > widthWindow && increment_.x > 0)
                || (mushroom_.getPosition().x - size_.x / 2 < 0 && increment_.x < 0))
                increment_.x = -increment_.x; // reverse the direction on X axis.

            if ((mushroom_.getPosition().y + size_.y / 2 > heighWindow && increment_.y > 0)
                || (mushroom_.getPosition().y - size_.y / 2 < 0 && increment_.y < 0))
                increment_.y = -increment_.y; // reverse the direction on Y axis.

            mushroom_.setPosition(mushroom_.getPosition() + increment_);
        }

        const sf::Sprite& GetMushroom() const
        {
            return mushroom_;
        }

    private:
        const char* path_ = R"(..\Res\images\Mushroom.png)";
        sf::Vector2u size_;
        sf::Texture mushroom_texture_;
        sf::Sprite mushroom_;
        sf::Vector2f increment_;

        void InitMushroom()
        {
            mushroom_texture_.loadFromFile(path_);
            mushroom_.setTexture(mushroom_texture_);
			// mushroom_.setColor(sf::Color(255, 0, 0, 255)); // Red tint.

            size_ = mushroom_texture_.getSize();
            mushroom_.setOrigin(size_.x / 2.0f, size_.y / 2.0f);
        }
    };
}
