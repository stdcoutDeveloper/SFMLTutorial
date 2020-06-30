#pragma once

#include "Window.h"
#include "TextureManager.h"
#include "EntityManager.h"

namespace SFMLTutorial
{
    class Map; // forward declaration, not include Map.h here.

    struct SharedContext
    {
        SharedContext() : window_(nullptr), event_manager_(nullptr), texture_mgr_(nullptr), game_map_(nullptr),
                          entity_mgr_(nullptr)
        {
        }

        Window* window_;
        EventManager* event_manager_;
        TextureManager* texture_mgr_;
        Map* game_map_;
        EntityManager* entity_mgr_;
    };
}
