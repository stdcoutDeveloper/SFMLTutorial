#pragma once

#include "pch.h"
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>

namespace SFMLTutorial
{
    /**
     * \brief Type of event.
     */
    enum class EventType
    {
		// @formatter:off
        KEY_DOWN = sf::Event::KeyPressed, //5
        KEY_UP = sf::Event::KeyReleased,
        MOUSE_BUTTON_DOWN = sf::Event::MouseButtonPressed, //9
        MOUSE_BUTTON_UP = sf::Event::MouseButtonReleased,
        MOUSE_WHEEL = sf::Event::MouseWheelMoved,
        WINDOW_RESIZED = sf::Event::Resized,
        GAINED_FOCUS = sf::Event::GainedFocus,
        LOST_FOCUS = sf::Event::LostFocus,
        MOUSE_ENTERED = sf::Event::MouseEntered,
        MOUSE_LEFT = sf::Event::MouseLeft,
        CLOSED = sf::Event::Closed,
        TEXT_ENTERED = sf::Event::TextEntered,
        KEYBOARD = sf::Event::Count + 1,  //24
        MOUSE,
        JOYSTICK
        // @formatter:on
    };

    /**
     * \brief Binding the code (which key) to a event type.
     */
    struct EventInfo
    {
        EventInfo() : code_of_key_pressed_(0)
        {
        }

        EventInfo(int event) : code_of_key_pressed_(event)
        {
        }

        union
        {
            // Code of the key that has been pressed (F5 code is 89)
            int code_of_key_pressed_;
        };
    };

    /**
     * \brief Event details.
     */
    struct EventDetails
    {
        EventDetails(const std::string& bindName) : name_(bindName)
        {
            Clear();
        }

        void Clear()
        {
            size_ = sf::Vector2i(0, 0);
            text_entered_ = 0;
            mouse_ = sf::Vector2i(0, 0);
            mouse_wheel_delta_ = 0;
            keycode_ = -1;
        }

        std::string name_;
        sf::Vector2i size_;
        sf::Uint32 text_entered_;
        sf::Vector2i mouse_;
        int mouse_wheel_delta_;
        int keycode_;
    };

    /**
     * \brief Bind name event with event details.
     */
    struct Binding
    {
        Binding(const std::string& name) : name_(name), count_events_happening_(0), details_(name)
        {
        }

        void BindEvent(EventType type, EventInfo info = EventInfo())
        {
            // insert new elements at the end of vector, new element is constructed using arguments.
            events_.emplace_back(type, info);
        }

        typedef std::vector<std::pair<EventType, EventInfo>> Events;

        Events events_;
        std::string name_;
        int count_events_happening_;
        EventDetails details_;
    };

    class EventManager
    {
    public:
        EventManager()
        {
            LoadBindings();
        }

        ~EventManager()
        {
            for (auto& itr : bindings_)
            {
                delete itr.second;
                itr.second = nullptr;
            }
        }

        bool AddBinding(Binding* binding)
        {
            if (bindings_.find(binding->name_) != bindings_.end()) // binding name has already existed?
                return false;

            return bindings_.emplace(binding->name_, binding).second;
        }

        bool RemoveBinding(std::string name)
        {
            auto itr = bindings_.find(name);
            if (itr == bindings_.end()) // is name not existed?
                return false;

            delete itr->second;
            bindings_.erase(itr);
            return true;
        }

        void SetFocus(const bool& focus)
        {
            is_window_focused_ = focus;
        }

        template <class T>
        bool AddCallback(const std::string& name, void (T::*func)(EventDetails*), T* instance)
        {
            auto temp = std::bind(func, instance, std::placeholders::_1);
            return callbacks_.emplace(name, temp).second; // second is std::function...
        }

        void RemoveCallback(const std::string& name)
        {
            callbacks_.erase(name);
        }

        void HandleEvent(sf::Event& event)
        {
            for (auto& bindingItr : bindings_)
            {
                Binding* bind = bindingItr.second;
                for (auto& eventItr : bind->events_)
                {
                    EventType sfmlEvent = static_cast<EventType>(event.type);
                    if (eventItr.first != sfmlEvent)
                        continue;

                    // check if it's a keyboard event
                    if (sfmlEvent == EventType::KEY_DOWN || sfmlEvent == EventType::KEY_UP) // is matching type?
                    {
                        if (eventItr.second.code_of_key_pressed_ == event.key.code) // is matching code?
                        {
                            if (bind->details_.keycode_ != -1)
                                bind->details_.keycode_ = eventItr.second.code_of_key_pressed_;

                            (bind->count_events_happening_)++;
                            break;
                        }
                    }
                    else if (sfmlEvent == EventType::MOUSE_BUTTON_DOWN || sfmlEvent == EventType::MOUSE_BUTTON_UP)
                        // mouse event?
                    {
                        if (eventItr.second.code_of_key_pressed_ == event.mouseButton.button)
                        {
                            bind->details_.mouse_.x = event.mouseButton.x;
                            bind->details_.mouse_.y = event.mouseButton.y;

                            if (bind->details_.keycode_ != -1)
                                bind->details_.keycode_ = eventItr.second.code_of_key_pressed_;

                            (bind->count_events_happening_)++;
                            break;
                        }
                    }
                    else
                    {
                        if (sfmlEvent == EventType::MOUSE_WHEEL)
                        {
                            bind->details_.mouse_wheel_delta_ = event.mouseWheel.delta;
                        }
                        else if (sfmlEvent == EventType::WINDOW_RESIZED)
                        {
                            bind->details_.size_.x = event.size.width;
                            bind->details_.size_.y = event.size.height;
                        }
                        else if (sfmlEvent == EventType::TEXT_ENTERED)
                        {
                            bind->details_.text_entered_ = event.text.unicode;
                        }

                        (bind->count_events_happening_)++;
                    }
                }
            }
        }

        void Update()
        {
            if (!is_window_focused_)
                return;

            for (auto& bindingItr : bindings_)
            {
                Binding* bind = bindingItr.second;
                for (auto& eventItr : bind->events_)
                {
                    switch (eventItr.first)
                    {
                    case EventType::KEYBOARD:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(eventItr.second.code_of_key_pressed_)))
                        {
                            if (bind->details_.keycode_ != -1)
                                bind->details_.keycode_ = eventItr.second.code_of_key_pressed_;

                            (bind->count_events_happening_)++;
                        }
                        break;

                    case EventType::MOUSE:
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Button(eventItr.second.code_of_key_pressed_)))
                        {
                            if (bind->details_.keycode_ != -1)
                                bind->details_.keycode_ = eventItr.second.code_of_key_pressed_;

                            (bind->count_events_happening_)++;
                        }
                        break;

                    case EventType::JOYSTICK:
                        // Expansion in the future.
                        break;

                    default:
                        break;
                    }
                }

                // is matching number of events in the event container and events that are "on"?
                if (bind->events_.size() == bind->count_events_happening_)
                {
                    auto callItr = callbacks_.find(bind->name_);
                    if (callItr != callbacks_.end()) // if found -> call function accordingly.
                        callItr->second(&(bind->details_));
                }

                // reset
                bind->count_events_happening_ = 0;
                bind->details_.Clear();
            }
        }

        sf::Vector2i GetMousePosition(sf::RenderWindow* window = nullptr)
        {
            return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
        }

    private:
        typedef std::unordered_map<std::string, Binding*> Bindings;
        typedef std::unordered_map<std::string, std::function<void(EventDetails*)>> Callbacks;

        Bindings bindings_;
        Callbacks callbacks_;
        bool is_window_focused_ = true;

        /**
         * \brief Load binding from a file.
         */
        void LoadBindings()
        {
            std::string delimiter = ":";

            std::ifstream bindings;
            bindings.open(R"(..\Res\configs\keys.cfg)");
            if (!bindings.is_open())
                return;

            std::string line;
            while (std::getline(bindings, line)) // read by line.
            {
                std::stringstream keyStream(line);
                std::string callbackName;
                keyStream >> callbackName; // get callback name.
                Binding* bind = new Binding(callbackName);
                while (!keyStream.eof()) // reach to end of line?
                {
                    std::string keyValue;
                    keyStream >> keyValue;
                    int start = 0;
                    int end = keyValue.find(delimiter);
                    if (end == std::string::npos) // if not found ":"
                    {
                        delete bind;
                        bind = nullptr;
                        break;
                    }
                    EventType type = EventType(std::stoi(keyValue.substr(start, end - start)));
                    int code = std::stoi(keyValue.substr(end + delimiter.length(),
                                                         keyValue.find(delimiter, end + delimiter.length())));
                    EventInfo eventInfo;
                    eventInfo.code_of_key_pressed_ = code;

                    bind->BindEvent(type, eventInfo);
                }

                if (!AddBinding(bind))
                    delete bind;

                bind = nullptr;
            }

            bindings.close();
        }
    };
}
