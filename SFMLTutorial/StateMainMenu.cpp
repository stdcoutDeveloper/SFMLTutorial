#include "StateMainMenu.h"
#include "StateManager.h"

using namespace SFMLTutorial;

StateMainMenu::StateMainMenu(StateManager* stateManager) : BaseState(stateManager)
{
}

StateMainMenu::~StateMainMenu() = default;

void StateMainMenu::OnCreate()
{
    font_.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
    text_.setFont(font_);
    text_.setString(sf::String("MAIN MENU"));
    text_.setCharacterSize(18);

    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text_.setPosition(static_cast<float>(400), static_cast<float>(100));

    button_size_ = sf::Vector2f(300.0f, 32.0f);
    button_position_ = sf::Vector2f(400.0f, 200.0f);
    button_padding_ = 4; // 4 pixel.

    std::string strs[3] = {"PLAY", "CREDITS", "EXIT"};
    for (int i = 0; i < 3; i++)
    {
        // buttons
        sf::Vector2f buttonPosition(button_position_.x, button_position_.y + i * (button_size_.y + button_padding_));
        rects_[i].setSize(button_size_);
        rects_[i].setFillColor(sf::Color::Red);

        rects_[i].setOrigin(button_size_.x / 2.0f, button_size_.y / 2.0f);
        rects_[i].setPosition(buttonPosition);

        // labels
        labels_[i].setFont(font_);
        labels_[i].setString(sf::String(strs[i]));
        labels_[i].setCharacterSize(12);

        sf::FloatRect rect = labels_[i].getLocalBounds();
        labels_[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        labels_[i].setPosition(buttonPosition);
    }

    // TODO ???
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->AddCallback(StateType::MAIN_MENU, "Mouse_Left", &StateMainMenu::MouseClick, this);
}

/**
 * \brief Remove callbacks
 */
void StateMainMenu::OnDestroy()
{
    EventManager* eventMgr = state_mgr_->GetSharedContext()->event_manager_;
    eventMgr->RemoveCallback(StateType::MAIN_MENU, "Mouse_Left");
}

/**
 * \brief If state GAME exist, adjust "play" button to "resume" button.
 */
void StateMainMenu::Activate()
{
    if (state_mgr_->IsHasState(StateType::GAME) && labels_[0].getString() == "PLAY")
    {
        labels_[0].setString(sf::String("RESUME"));
        sf::FloatRect rect = labels_[0].getLocalBounds();
        labels_[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    }
}

void StateMainMenu::Deactivate()
{
}

void StateMainMenu::Update(const sf::Time& time)
{
}

void StateMainMenu::Draw()
{
    sf::RenderWindow& window = state_mgr_->GetSharedContext()->window_->GetRenderWindow();
    window.draw(text_); // title
    for (int i = 0; i < 3; i++)
    {
        window.draw(rects_[i]); // buttons
        window.draw(labels_[i]); // labels
    }
}

/**
 * \brief Perform action when clicking on buttons of main menu.
 */
void StateMainMenu::MouseClick(EventDetails* details)
{
    sf::Vector2i mousePosition = details->mouse_;
    float halfX = button_size_.x / 2.0f;
    float halfY = button_size_.y / 2.0f;
    for (int i = 0; i < 3; i++)
    {
        if (mousePosition.x >= rects_[i].getPosition().x - halfX && mousePosition.x <= rects_[i].getPosition().x + halfX
            && mousePosition.y >= rects_[i].getPosition().y - halfY && mousePosition.y <= rects_[i].getPosition().y +
            halfY)
        {
            if (i == 0)
            {
                state_mgr_->SwitchTo(StateType::GAME); // play
            }
            else if (i == 1)
            {
                // TODO: Credits state.
                // state_mgr_->SwitchTo(StateType::CREDITS);
            }
            else if (i == 2)
            {
                state_mgr_->GetSharedContext()->window_->Close(); // exit
            }
        }
    }
}
