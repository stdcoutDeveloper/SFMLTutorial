#pragma once

#include "pch.h"
#include "ResourceManager.h"

namespace SFMLTutorial
{
    class TextureManager : public ResourceManager<TextureManager, sf::Texture>
    {
    public:
        TextureManager() : ResourceManager("textures.cfg")
        {
        }

        /**
         * \brief Load resource into sf::Texture.
         * @param path: path of texture resource.
         */
        sf::Texture* Load(const std::string& path)
        {
            sf::Texture* texture = new sf::Texture();
            if (!texture->loadFromFile(Utilities::GetWorkingDirectoryA() + path))
            {
                delete texture;
                texture = nullptr;
#ifdef _DEBUG
                std::cerr << "Could not load texture: " << path << std::endl;
#endif
            }

            return texture;
        }
    };
}
