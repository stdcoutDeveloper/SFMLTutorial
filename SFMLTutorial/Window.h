#pragma once
#include "pch.h"
#include <string>

namespace SFMLTutorial
{
    class Window
    {
    public:
        Window()
        {
            Setup("Window", sf::Vector2u(640, 480)); // default.
        }

        Window(const std::string& title, const sf::Vector2u& size)
        {
            Setup(title, size);
        }

        ~Window()
        {
            Destroy();
        }

        /**
         * \brief Clear the window.
         */
        void ClearBeforeDraw()
        {
            window_.clear(sf::Color::Black);
        }

        void Draw(sf::Drawable& drawable)
        {
            window_.draw(drawable);
        }

        /**
         * \brief Display the changes.
         */
        void DisplayAfterDraw()
        {
            window_.display();
        }

        void Update()
        {
            sf::Event event;
            while (window_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    is_close_ = true;
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
                    ToggleFullScreen();
            }
        }

        bool IsClose() const
        {
            return is_close_;
        }

        bool IsFullScreen() const
        {
            return is_fullscreen_;
        }

        sf::Vector2u GetWindowSize() const
        {
            return window_size_;
        }

        void ToggleFullScreen()
        {
            is_fullscreen_ = !is_fullscreen_;
            Destroy();
            Create();
        }

        sf::RenderWindow& GetRenderWindow()
        {
            return window_;
        }

    private:
        sf::RenderWindow window_;
        sf::Vector2u window_size_;
        std::string window_title_;
        bool is_close_ = false, is_fullscreen_ = false;

        void Setup(const std::string title, const sf::Vector2u& size)
        {
            window_title_ = title;
            window_size_ = size;

            Create();
        }

        void Destroy()
        {
            window_.close();
        }

        void Create()
        {
            auto style = is_fullscreen_ ? sf::Style::Fullscreen : sf::Style::Default;
            window_.create({window_size_.x, window_size_.y, 32}, window_title_, style);
        }
    };
}
