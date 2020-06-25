#pragma once

#include "BaseState.h"
#include "EventManager.h"

namespace SFMLTutorial
{
    class StateIntro : public BaseState
    {
    public:
        StateIntro(StateManager* stateManager);

        ~StateIntro();

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
         * \brief Transition when hit the Spacebar after a certain interval.
         */
        void Continue(EventDetails* details);

    private:
        sf::Texture intro_texture_;
        sf::Sprite intro_sprite_;
        sf::Font font_;
        sf::Text text_;
        float time_passed_;
    };
}
