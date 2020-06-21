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

	private:
		sf::RectangleShape bound_shape_;
		int element_size_ = 16;
	};
}
