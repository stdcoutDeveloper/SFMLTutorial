#pragma once

#include "BaseState.h"
#include "EventManager.h"

namespace SFMLTutorial
{
    class StateMainMenu : public BaseState
    {
    public:
        StateMainMenu(StateManager* stateManager);

        ~StateMainMenu();

        void OnCreate() override;

        /**
         * \brief Remove callbacks
         */
        void OnDestroy() override;

        /**
         * \brief If state GAME exist, adjust "play" button to "resume" button.
         */
        void Activate() override;

        void Deactivate() override;

        void Update(const sf::Time& time) override;

        void Draw() override;

        /**
         * \brief Perform action when clicking on buttons of main menu.
         */
        void MouseClick(EventDetails* details);

    private:
        sf::Font font_;
        sf::Text text_; // title

        sf::Vector2f button_size_;
        sf::Vector2f button_position_;
        unsigned int button_padding_;

        sf::RectangleShape rects_[3];
        sf::Text labels_[3];
    };
}
