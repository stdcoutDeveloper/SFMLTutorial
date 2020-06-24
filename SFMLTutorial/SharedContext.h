#pragma once

#include "Window.h"

namespace SFMLTutorial
{
    struct SharedContext
    {
        SharedContext() : window_(nullptr), event_manager_(nullptr)
        {
        }

        Window* window_;
        EventManager* event_manager_;
    };
}
