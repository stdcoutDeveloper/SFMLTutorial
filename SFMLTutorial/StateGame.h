#pragma once

#include "BaseState.h"
#include "EventManager.h"

namespace SFMLTutorial
{
    class StateGame : public BaseState
    {
    public:
        StateGame(StateManager* stateManager);

        ~StateGame();

        /**
         * \brief Resource allocation.
         */
        void OnCreate() override;

        /**
         * \brief Remove callbacks
         */
        void OnDestroy() override;
        void Activate() override;

        void Deactivate() override;

        /**
         * \brief Move the sprite
         */
        void Update(const sf::Time& time) override;

        void Draw() override;

        /**
         * \brief Switching states.
         */
        void MainMenu(EventDetails* details);

        void Pause(EventDetails* details);

    private:
        sf::Texture texture_;
        sf::Sprite sprite_;
        sf::Vector2f increment_;
    };
}
