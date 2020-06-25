#pragma once

#include "SharedContext.h"
#include "StateIntro.h"
#include "StateMainMenu.h" // TODO ???

namespace SFMLTutorial
{
    enum class StateType
    {
        INTRO = 1,
        MAIN_MENU,
        GAME,
        PAUSED,
        GAME_OVER,
        CREDITS
    };

    class StateManager
    {
    public:
        StateManager(SharedContext* sharedContext) : shared_context_(sharedContext)
        {
            RegisterState<StateIntro>(StateType::INTRO);
            RegisterState<StateMainMenu>(StateType::MAIN_MENU);
            // RegisterState<StateGame>(StateType::GAME);
            // RegisterState<StatePaused>(StateType::PAUSED);
        }

        ~StateManager()
        {
            for (auto& itr : states_)
            {
                itr.second->OnDestroy();
                delete itr.second;
            }
        }

        /**
         * \brief Update state with elapsed time passed in as argument.
         */
        void Update(const sf::Time& time)
        {
            if (states_.empty())
                return;

            // Check whether top state allow other to be updated?
            if (states_.back().second->IsTranscendent() && states_.size() > 1)
            {
                auto itr = states_.end();
                while (itr != states_.begin())
                {
                    if (itr != states_.end())
                    {
                        if (!itr->second->IsTranscendent())
                            break;
                    }
                    --itr;
                }

                while (itr != states_.end())
                {
                    itr->second->Update(time);
                    ++itr;
                }
            }
            else
            {
                states_.back().second->Update(time);
            }
        }

        /**
         * \brief Draw in correct order.
         * The lastest state is drawn on screen last.
         */
        void Draw()
        {
            if (states_.empty())
                return;

            if (states_.back().second->IsTransparent() && states_.size() > 1)
            {
                auto itr = states_.end();
                while (itr != states_.begin())
                {
                    if (itr != states_.end())
                    {
                        if (!itr->second->IsTransparent())
                            break;
                    }
                    --itr;
                }

                while (itr != states_.end())
                {
                    itr->second->Draw();
                    ++itr;
                }
            }
            else
            {
                states_.back().second->Draw();
            }
        }

        /**
         * \brief Remove states which are about to be removed.
         */
        void ProcessRequests()
        {
            while (state_to_remove_.begin() != state_to_remove_.end())
            {
                RemoveState(*state_to_remove_.begin());
                state_to_remove_.erase(state_to_remove_.begin());
            }
        }

        SharedContext* GetSharedContext()
        {
            return shared_context_;
        }

        /**
         * \brief Check whether manger has state?
         */
        bool IsHasState(const StateType& type)
        {
            for (auto itr = states_.begin(); itr != states_.end(); ++itr)
            {
                if (itr->first == type)
                {
                    auto removed = std::find(state_to_remove_.begin(), state_to_remove_.end(), type);
                    if (removed == state_to_remove_.end()) // is state not about to be removed?
                        return true;

                    // state is about to be removed.
                    return false;
                }
            }
            // state is not found.
            return false;
        }

        /**
         * \brief Change the current state to other state.
         * @param type: other state.
         */
        void SwitchTo(const StateType& type)
        {
            shared_context_->event_manager_->SetCurrentState(type);
            for (auto itr = states_.begin(); itr != states_.end(); ++itr)
            {
                // if found state
                if (itr->first == type)
                {
                    states_.back().second->Deactivate();
                    StateType tempType = itr->first;
                    BaseState* tempState = itr->second;
                    states_.erase(itr);
                    states_.emplace_back(tempType, tempState);
                    tempState->Activate();
                    return;
                }
            }

            // Not found state: create new one.
            if (!states_.empty())
                states_.back().second->Deactivate();

            CreateState(type);
            states_.back().second->Activate();
        }

        /**
         * \brief Add state which is about to removed into removed states list.
         */
        void Remove(const StateType& type)
        {
            state_to_remove_.push_back(type);
        }

    private:
        typedef std::vector<std::pair<StateType, BaseState*>> StateContainer;
        typedef std::vector<StateType> TypeContainer;
        typedef std::unordered_map<StateType, std::function<BaseState*(void)>> StateFactory;

        SharedContext* shared_context_;
        StateContainer states_;
        TypeContainer state_to_remove_;
        StateFactory state_factory_;

        /**
         * \brief Create a state if it is already existed.
         */
        void CreateState(const StateType& type)
        {
            auto newState = state_factory_.find(type);
            if (newState == state_factory_.end()) // not found?
                return;

            BaseState* state = newState->second();
            states_.emplace_back(type, state);
            state->OnCreate();
        }

        /**
         * \brief Remove state from list.
         */
        void RemoveState(const StateType& type)
        {
            for (auto itr = states_.begin(); itr != states_.end(); ++itr)
            {
                if (itr->first == type)
                {
                    itr->second->OnDestroy();
                    delete itr->second;
                    states_.erase(itr);
                    return;
                }
            }
        }

        /**
         * \brief Generate state automatically.
         */
        template <class T>
        void RegisterState(const StateType& type)
        {
            state_factory_[type] = [this]() -> BaseState*
            {
                return new T(this);
            };
        }
    };
}
