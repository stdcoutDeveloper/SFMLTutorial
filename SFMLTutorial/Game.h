#pragma once
#include "Window.h"
#include "Mushroom.h"

namespace SFMLTutorial
{
    class Game
    {
    public:
        Game() : window_("Game", sf::Vector2u(800, 600))
        {
        }

        ~Game() = default;

        void HandleInput()
        {
            // TODO: process input
        }

        void Update()
        {
            window_.Update();
            mush_.Update(window_.GetWindowSize().x, window_.GetWindowSize().y, time_elapsed_.asSeconds());
        }

        /**
         * \brief Draw sprites to screen.
         */
        void Render()
        {
            window_.ClearBeforeDraw();
            window_.Draw(mush_.GetMushroom());
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
            time_elapsed_ = clock_.restart();
        }

    private:
        Window window_;
        Mushroom mush_;
        sf::Clock clock_;
        sf::Time time_elapsed_;
        static constexpr float FPS = 1 / 60.0f; // 60 frame per second.
    };
}
