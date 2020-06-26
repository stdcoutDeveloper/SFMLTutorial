#include "StateGame.h"
#include "StateManager.h"

using namespace SFMLTutorial;

StateGame::StateGame(StateManager* stateManager) : BaseState(stateManager)
{
}

StateGame::~StateGame() = default;

/**
 * \brief Resource allocation.
 */
void StateGame::OnCreate()
{
    texture_.loadFromFile(R"(..\Res\images\Mushroom.png)");
    sprite_.setTexture(texture_);
    sprite_.setPosition(0.0f, 0.0f);
    increment_ = sf::Vector2f(400.0f, 400.0f);

    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->AddCallback(StateType::GAME, "Key_Escape", &StateGame::MainMenu, this);
    eventMgr->AddCallback(StateType::GAME, "Key_P", &StateGame::Pause, this);
}

/**
 * \brief Remove callbacks
 */
void StateGame::OnDestroy()
{
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->RemoveCallback(StateType::GAME, "Key_Escape");
    eventMgr->RemoveCallback(StateType::GAME, "Key_P");
}

void StateGame::Activate()
{
}

void StateGame::Deactivate()
{
}

/**
 * \brief Move the sprite
 */
void StateGame::Update(const sf::Time& time)
{
    sf::Vector2u windowSize = state_mgr_->GetSharedContext()->window_->GetWindowSize();
    sf::Vector2u textureSize = texture_.getSize();

    if ((sprite_.getPosition().x > windowSize.x - textureSize.x && increment_.x > 0) || (sprite_.getPosition().x < 0 &&
        increment_.x < 0))
        increment_.x = -increment_.x;

    if ((sprite_.getPosition().y > windowSize.y - textureSize.y && increment_.y > 0) || (sprite_.getPosition().y < 0 &&
        increment_.y < 0))
        increment_.y = -increment_.y;

    sprite_.setPosition(sprite_.getPosition() + increment_ * time.asSeconds());
}

void StateGame::Draw()
{
    sf::RenderWindow& window = state_mgr_->GetSharedContext()->window_->GetRenderWindow();
    window.draw(sprite_);
}

/**
 * \brief Switching states.
 */
void StateGame::MainMenu(EventDetails* details)
{
    state_mgr_->SwitchTo(StateType::MAIN_MENU);
}

void StateGame::Pause(EventDetails* details)
{
    state_mgr_->SwitchTo(StateType::PAUSED);
}
