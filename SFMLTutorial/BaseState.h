#pragma once

#include "pch.h"

namespace SFMLTutorial
{
    class StateManager; // forward declaration. -> use StateManager* state_mgr_ undefined

    class BaseState
    {
    public:
        BaseState(StateManager* stateManager) : state_mgr_(stateManager)
        {
        }

        virtual ~BaseState()
        {
        }

        virtual void OnCreate() = 0;
        virtual void OnDestroy() = 0;

        virtual void Activate() = 0;
        virtual void Deactivate() = 0;

        virtual void Update(const sf::Time& time) = 0;
        virtual void Draw() = 0;

        void SetTransparent(const bool& isTransparent)
        {
            is_transparent_ = isTransparent;
        }

        bool IsTransparent() const
        {
            return is_transparent_;
        }

        void SetTranscendent(const bool& isTranscendent)
        {
            is_transcendent_ = isTranscendent;
        }

        bool IsTranscendent() const
        {
            return is_transcendent_;
        }

        StateManager* GetStateManager()
        {
            return state_mgr_;
        }

    protected:
        StateManager* state_mgr_;
        bool is_transparent_ = false, is_transcendent_ = false;

    private:
        friend class StateManager;
    };
}
