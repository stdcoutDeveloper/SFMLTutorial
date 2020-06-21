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
					bounds_[i].SetSize(sf::Vector2f(window_size_.x, block_size_));
				else
					bounds_[i].SetSize(sf::Vector2f(block_size_, window_size_.y));
				if (i < 2)
					bounds_[i].GetBound().setPosition(0, 0);
				else
				{
					bounds_[i].GetBound().setOrigin(bounds_[i].GetBound().getSize());
					bounds_[i].GetBound().setPosition(sf::Vector2f(window_size_));
				}
			}
		}

		~World() = default;

		int GetBlockSize()
		{

		}

		void RespawnApple()
		{

		}

		void Update(Snake& player)
		{

		}

		void Render()
		{

		}

	private:
		Apple apple_;
		Bound bounds_[4];
		sf::Vector2u window_size_;
		sf::Vector2i item_;
		int block_size_ = 16;
	};
}
