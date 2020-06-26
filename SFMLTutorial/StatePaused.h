#pragma once

#include "BaseState.h"
#include "EventManager.h"

namespace SFMLTutorial
{
    class StatePaused : public BaseState
    {
    public:
        StatePaused(StateManager* stateManager);

        ~StatePaused();

        void OnCreate() override;

        void OnDestroy() override;

        void Activate() override;

        void Deactivate() override;

        void Update(const sf::Time& time) override;

        void Draw() override;

        void Unpause(EventDetails* details);

    private:
        sf::Font font_;
        sf::Text text_;
        sf::RectangleShape rect_; // backdrop
    };
}
