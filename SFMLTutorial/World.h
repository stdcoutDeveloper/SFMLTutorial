#pragma once
#include "Snake.h"
#include "Bound.h"
#include "Apple.h"

namespace SFMLTutorial
{
    class World
    {
    public:
        World(sf::Vector2u windowSize) : window_size_(windowSize)
        {
            RespawnApple();

            for (int i = 0; i < 4; i++)
            {
                if ((i + 1) % 2 == 0)
                    bounds_[i].SetSize(sf::Vector2f(static_cast<float>(window_size_.x),
                                                    static_cast<float>(grid_size_)));
                else
                    bounds_[i].SetSize(sf::Vector2f(static_cast<float>(grid_size_),
                                                    static_cast<float>(window_size_.y)));

                if (i < 2) // top-left corner?
                {
                    bounds_[i].SetPosition(0, 0);
                }
                else
                {
                    bounds_[i].SetOrigin(bounds_[i].GetSize()); // change origin to bottom-right corner.
                    bounds_[i].SetPosition(sf::Vector2f(window_size_));
                }
            }
        }

        ~World() = default;

        int GetGridSize()
        {
            return grid_size_;
        }

        void RespawnApple()
        {
            int maxX = window_size_.x / grid_size_ - 2;
            int maxY = window_size_.y / grid_size_ - 2;
            apple_position_ = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
            // x in the range 1 to maxX, y in the range 1 to maxY
            apple_.SetPosition(static_cast<float>(apple_position_.x * grid_size_),
                               static_cast<float>(apple_position_.y * grid_size_));
        }

        void Update(Snake& player)
        {
            if (player.GetPosition() == apple_position_)
            {
                player.Extend();
                player.IncreaseScore();
                RespawnApple();
            }

            int gridSizeX = window_size_.x / grid_size_;
            int gridSizeY = window_size_.y / grid_size_;
            if (player.GetPosition().x <= 0 || player.GetPosition().y <= 0 || player.GetPosition().x >= gridSizeX - 1
                || player.GetPosition().y >= gridSizeY - 1)
                player.Lose();
        }

        void Render(sf::RenderWindow& window)
        {
            // draw bounds and apple.
            for (int i = 0; i < 4; i++)
            {
                window.draw(bounds_[i].GetBound());
            }

            window.draw(apple_.GetApple());
        }

    private:
        Apple apple_;
        Bound bounds_[4]; // 4 bounds: left, top, right, bottom.
        sf::Vector2u window_size_;
        sf::Vector2i apple_position_;
        int grid_size_ = 16; // size of a square block (a grid). 
    };
}
