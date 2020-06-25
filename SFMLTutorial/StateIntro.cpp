#include "StateIntro.h"
#include "StateManager.h"

using namespace SFMLTutorial;

StateIntro::StateIntro(StateManager* stateManager) : BaseState(stateManager)
{
}

StateIntro::~StateIntro() = default;

void StateIntro::OnCreate()
{
    time_passed_ = 0.0f;

    // get current window size
    sf::Vector2u windowSize = state_mgr_->GetSharedContext()->window_->GetRenderWindow().getSize();

    intro_texture_.loadFromFile(R"(..\Res\images\intro.png)");
    intro_sprite_.setTexture(intro_texture_);
    intro_sprite_.setOrigin(intro_texture_.getSize().x / 2.0f, intro_texture_.getSize().y / 2.0f);
    // centre of image.
    intro_sprite_.setPosition(windowSize.x / 2.0f, 0.0f);

    font_.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
    text_.setFont(font_);
    text_.setString("Press SPACE to continue...");
    text_.setCharacterSize(15);
    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    // centre of text.
    text_.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->AddCallback(StateType::INTRO, "Intro_Continue", &StateIntro::Continue, this);
}

/**
 * \brief Remove callbacks
 */
void StateIntro::OnDestroy()
{
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->RemoveCallback(StateType::INTRO, "Intro_Continue");
}

void StateIntro::Activate()
{
}

void StateIntro::Deactivate()
{
}

/**
 * \brief Move the sprite
 */
void StateIntro::Update(const sf::Time& time)
{
    if (time_passed_ < 5.0f)
    {
        time_passed_ += time.asSeconds();
        intro_sprite_.setPosition(intro_sprite_.getPosition().x,
                                  intro_sprite_.getPosition().y + 48 * time.asSeconds());
        // move 48 pixel per second.
    }
}

void StateIntro::Draw()
{
    sf::RenderWindow& window = state_mgr_->GetSharedContext()->window_->GetRenderWindow();
    window.draw(intro_sprite_);

    if (time_passed_ >= 5.0f)
        window.draw(text_);
}

/**
 * \brief Transition when hit the Spacebar after a certain interval.
 */
void StateIntro::Continue(EventDetails* details)
{
    if (time_passed_ >= 5.0f)
    {
        state_mgr_->SwitchTo(StateType::MAIN_MENU);
        state_mgr_->Remove(StateType::INTRO);
    }
}
