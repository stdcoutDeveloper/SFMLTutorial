#pragma once
#include "pch.h"

namespace SFMLTutorial
{
    class Mushroom
    {
    public:
        Mushroom() : move_speed_(400.0f, 400.0f) // 400 pixel per second.
        {
            InitMushroom();
        }

        void Update(unsigned int widthWindow, unsigned int heightWindow, float time)
        {
            Bounce(widthWindow, heightWindow, time);
        }

        sf::Sprite& GetMushroom()
        {
            return mushroom_;
        }

    private:
        const char* path_ = R"(..\Res\images\Mushroom.png)";
        sf::Vector2u size_;
        sf::Texture mushroom_texture_;
        sf::Sprite mushroom_;
        sf::Vector2f move_speed_;

        void InitMushroom()
        {
            mushroom_texture_.loadFromFile(path_);
            mushroom_.setTexture(mushroom_texture_);
            // mushroom_.setColor(sf::Color(255, 0, 0, 255)); // Red tint.

            size_ = mushroom_texture_.getSize();
            mushroom_.setOrigin(size_.x / 2.0f, size_.y / 2.0f);
        }

        /**
         * \brief Bounce mushroom around windows.
         */
        void Bounce(unsigned int widthWindow, unsigned int heightWindow, float time)
        {
            if ((mushroom_.getPosition().x + size_.x / 2 > widthWindow && move_speed_.x > 0)
                || (mushroom_.getPosition().x - size_.x / 2 < 0 && move_speed_.x < 0))
                move_speed_.x = -move_speed_.x; // reverse the direction on X axis.

            if ((mushroom_.getPosition().y + size_.y / 2 > heightWindow && move_speed_.y > 0)
                || (mushroom_.getPosition().y - size_.y / 2 < 0 && move_speed_.y < 0))
                move_speed_.y = -move_speed_.y; // reverse the direction on Y axis.

            mushroom_.setPosition(mushroom_.getPosition() + move_speed_ * time);
        }
    };
}
