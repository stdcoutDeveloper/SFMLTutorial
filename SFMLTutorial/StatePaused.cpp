#include "StatePaused.h"
#include "StateManager.h"

using namespace SFMLTutorial;

StatePaused::StatePaused(StateManager* stateManager) : BaseState(stateManager)
{
}

StatePaused::~StatePaused() = default;

void StatePaused::OnCreate()
{
    SetTransparent(true);

    // setup text
    font_.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
    text_.setFont(font_);
    text_.setString(sf::String("PAUSED"));
    text_.setCharacterSize(14);
    text_.setStyle(sf::Text::Bold);

    sf::Vector2u windowSize = state_mgr_->GetSharedContext()->window_->GetWindowSize();

    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text_.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    // setup backdrop
    rect_.setSize(sf::Vector2f(windowSize));
    rect_.setPosition(0.0f, 0.0f);
    rect_.setFillColor(sf::Color(0, 0, 0, 150));

    // add callback for handling event
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->AddCallback(StateType::PAUSED, "Key_P", &StatePaused::Unpause, this);
}

void StatePaused::OnDestroy()
{
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->RemoveCallback(StateType::PAUSED, "Key_P");
}

void StatePaused::Activate()
{
}

void StatePaused::Deactivate()
{
}

void StatePaused::Update(const sf::Time& time)
{
}

void StatePaused::Draw()
{
    sf::RenderWindow& window = state_mgr_->GetSharedContext()->window_->GetRenderWindow();
    window.draw(rect_);
    window.draw(text_); // text should be drawn afterward
}

void StatePaused::Unpause(EventDetails* details)
{
    state_mgr_->SwitchTo(StateType::GAME);
}
