#pragma once

#include "Window.h"
#include "Mushroom.h"
#include "World.h"
#include "Snake.h"
#include "Textbox.h"

namespace SFMLTutorial
{
    class Game
    {
    public:
        Game() : window_("Game", sf::Vector2u(800, 600)), world_(sf::Vector2u(800, 600)), snake_(world_.GetGridSize()),
                 textbox_(5, 14, 350, sf::Vector2f(16.0f, 16.0f))
        {
            textbox_.Add("Seeded random number generator with: " + std::to_string(time(nullptr)));
        }

        ~Game() = default;

        void HandleInput()
        {
            // process input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && snake_.GetPhysicalDirection() != Direction::DOWN)
                snake_.SetDirection(Direction::UP);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && snake_.GetPhysicalDirection() != Direction::UP)
                snake_.SetDirection(Direction::DOWN);
			// @formatter: off
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && snake_.GetPhysicalDirection() != Direction::RIGHT)
			// @formatter: on
                snake_.SetDirection(Direction::LEFT);
			// @formatter: off
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && snake_.GetPhysicalDirection() != Direction::LEFT)
			// @formatter: on
                snake_.SetDirection(Direction::RIGHT);
        }

        void Update()
        {
            window_.Update();

            // mush_.Update(window_.GetWindowSize().x, window_.GetWindowSize().y, time_elapsed_.asSeconds());

            float timeStep = 1.0f / snake_.GetSpeed();
            if (time_elapsed_.asSeconds() >= timeStep)
            {
                snake_.Update();
                world_.Update(snake_);
                time_elapsed_ -= sf::seconds(timeStep);

                if (snake_.IsLost())
                {
                    textbox_.Update("Game over! Score: " + std::to_string(snake_.GetScore()), true);
                    snake_.Reset();
                }
                else if (snake_.GetScore() > 0)
                {
                    textbox_.Update("You ate an apple. Score: " + std::to_string(snake_.GetScore()));
                }
            }
        }

        /**
         * \brief Draw sprites to screen.
         */
        void Render()
        {
            window_.ClearBeforeDraw();

            // window_.Draw(mush_.GetMushroom());
            world_.Render(window_.GetRenderWindow());
            snake_.Render(window_.GetRenderWindow());
            textbox_.Render(window_.GetRenderWindow());

            window_.DisplayAfterDraw();
        }

        const Window& GetWindow() const
        {
            return window_;
        }

        sf::Time& GetTimeElapsed()
        {
            return time_elapsed_;
        }

        void RestartClock()
        {
            time_elapsed_ += clock_.restart();
        }

    private:
        Window window_;
        // Mushroom mush_;
        sf::Clock clock_;
        sf::Time time_elapsed_;
        World world_;
        Snake snake_;
        Textbox textbox_;
        // static constexpr float FPS = 1 / 60.0f; // 60 frame per second.
    };
}
