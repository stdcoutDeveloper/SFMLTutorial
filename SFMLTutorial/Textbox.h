#pragma once
#include "pch.h"
#include <string>

namespace SFMLTutorial
{
    class Textbox
    {
    public:
        Textbox()
        {
            Setup(5, 9, 200, sf::Vector2f(0.0f, 0.0f));
        }

        Textbox(int numberOfLinesVisible, int characterSize, int width, sf::Vector2f screenPosition)
        {
            Setup(numberOfLinesVisible, characterSize, width, screenPosition);
        }

        ~Textbox()
        {
            Clear();
        }

        void Add(std::string message)
        {
            messages_.push_back(message);
            if (messages_.size() < 6)
                return;
            messages_.erase(messages_.begin()); // remove first element in vector.
        }

        void Render(sf::RenderWindow& window)
        {
            std::string content;
            for (auto& itr : messages_)
            {
                content.append(itr + "\n");
            }

            if (!content.empty())
            {
                content_.setString(content);
                window.draw(backdrop_);
                window.draw(content_);
            }
        }

        /**
         * \brief Update content of message box if it has new message.
         */
        void Update(std::string str, bool isNewRound = false)
        {
            if (isNewRound)
            {
                Clear();
            }

            if (!IsContainMessage(str)) // is new message?
            {
                Add(str);
            }
        }

    private:
        typedef std::vector<std::string> MessageContainer;
        MessageContainer messages_;
        int number_of_lines_visible_;
        sf::RectangleShape backdrop_;
        sf::Font font_;
        sf::Text content_;

        void Setup(int numberOfLinesVisible, int characterSize, int width, sf::Vector2f screenPosition)
        {
            number_of_lines_visible_ = numberOfLinesVisible;

            // setup content
            sf::Vector2f offset(2.0f, 2.0f);
            font_.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
            content_.setFont(font_);
            content_.setString("");
            content_.setCharacterSize(characterSize);
            content_.setFillColor(sf::Color::White);
            content_.setPosition(screenPosition + offset);

            // setup backdrop
            backdrop_.setSize(sf::Vector2f(static_cast<float>(width), number_of_lines_visible_ * characterSize * 1.2f));
            backdrop_.setFillColor(sf::Color(90, 90, 90, 90));
            backdrop_.setPosition(screenPosition);
        }

        void Clear()
        {
            messages_.clear();
        }

        /**
         * \brief Check whether message box contains string
         */
        bool IsContainMessage(std::string str)
        {
            if (messages_.empty())
                return false;

            for (auto& itr : messages_)
            {
                if (itr.find(str) != std::string::npos) // found string?
                    return true;
            }

            return false;
        }
    };
}
