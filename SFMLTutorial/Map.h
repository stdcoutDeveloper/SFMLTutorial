#pragma once

#include "SharedContext.h"
#include "TextureManager.h"
#include "BaseState.h" // incomplete class
#include "StateManager.h" // so need to include StateManager.h
#include <math.h>

namespace SFMLTutorial
{
    enum Sheet
    {
        TILE_SIZE = 32,
        SHEET_WIDTH = 256,
        SHEET_HEIGHT = 256
    };

    typedef unsigned int TileID;

    struct TileInfo
    {
        TileInfo(SharedContext* context, const std::string& texture = "", TileID id = 0) : id_(0), is_deadly_(false),
                                                                                           context_(context)
        {
            TextureManager* textureMgr = context_->texture_mgr_;
            if (texture_.empty())
            {
                id_ = id;
                return;
            }

            if (!textureMgr->RequireResource(texture))
                return;

            texture_ = texture;
            id_ = id;
            sprite_.setTexture(*(textureMgr->GetResource(texture_)));
            sf::IntRect tileBoundaries(id_ % (SHEET_WIDTH / TILE_SIZE) * TILE_SIZE,
                                       id_ / (SHEET_HEIGHT / TILE_SIZE) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            sprite_.setTextureRect(tileBoundaries); // crop sprite.
        }

        /**
         * \brief Free the texture used for tile sheet.
         */
        ~TileInfo()
        {
            if (texture_.empty())
                return;

            context_->texture_mgr_->ReleaseResource(texture_);
        }

        sf::Sprite sprite_; // sprite represents the tile.

        TileID id_;
        std::string name_;
        sf::Vector2f friction_;
        bool is_deadly_;

        SharedContext* context_;
        std::string texture_; // texture
    };

    struct Tile
    {
        TileInfo* properties_;
        bool is_warp_; // is the tile a warp? (go to next level)
    };

    class Map
    {
    public:
        Map(SharedContext* context, BaseState* currentState) : default_tile_(context), max_map_size_(32, 32),
                                                               tile_count_(0), tile_set_count_(0), map_gravity_(512.0f),
                                                               is_load_next_map_(false), current_state_(currentState),
                                                               context_(context)
        {
            context_->game_map_ = this;
            LoadTiles("tiles.cfg");
        }

        ~Map()
        {
            PurgeMap();
            PurgeTileSet();
            context_->game_map_ = nullptr;
        }

        /**
         * \brief Get tile at specific coordinates.
         */
        Tile* GetTile(unsigned int x, unsigned int y)
        {
            auto itr = tile_map_.find(ConvertCoordinates(x, y));
            return (itr != tile_map_.end() ? itr->second : nullptr);
        }

        TileInfo* GetDefaultTile()
        {
            return &default_tile_;
        }

        float GetGravity() const
        {
            return map_gravity_;
        }

        unsigned int GetTileSize() const
        {
            return TILE_SIZE;
        }

        const sf::Vector2u& GetMapSize() const
        {
            return max_map_size_;
        }

        const sf::Vector2f& GetPlayerStart() const
        {
            return player_start_;
        }

        /**
         * \brief Load map from a path file.
         */
        void LoadMapFromConfigFile(const std::string& path)
        {
            std::ifstream ifs;
            ifs.open(Utilities::GetWorkingDirectoryA() + path, std::ifstream::in);
            if (!ifs.is_open())
            {
#ifdef _DEBUG
                std::cerr << "Could not load map file: " << path << std::endl;
#endif
                return;
            }

            // EntityManager* entityMgr = context_->entity_mgr_;
            std::string line;

            int playerId = -1;
            while (std::getline(ifs, line))
            {
                if (line[0] == '|') // ignore comment
                    continue;

                std::stringstream keyStream(line);
                std::string type;
                keyStream >> type;
                if (type == "TILE")
                {
                    int tileId = 0;
                    keyStream >> tileId;
                    if (tileId < 0)
                    {
#ifdef _DEBUG
                        std::cerr << "Bad tile id: " << tileId << std::endl;
#endif
                        continue;
                    }

                    auto itr = tile_set_.find(tileId);
                    if (itr == tile_set_.end())
                    {
#ifdef _DEBUG
                        std::cerr << "Tile id: " << tileId << "was not found in tile set." << std::endl;
#endif
                        continue;
                    }

                    sf::Vector2i tileCoordinates;
                    keyStream >> tileCoordinates.x >> tileCoordinates.y;
                    if (tileCoordinates.x > max_map_size_.x || tileCoordinates.y > max_map_size_.y)
                        // is within boundaries of map size?
                    {
#ifdef _DEBUG
                        std::cerr << "Tile is out of range: " << tileCoordinates.x << " " << tileCoordinates.y <<
                            std::endl;
#endif
                        continue;
                    }

                    Tile* tile = new Tile();
                    tile->properties_ = itr->second; // tile information
                    if (!tile_map_.emplace(ConvertCoordinates(tileCoordinates.x, tileCoordinates.y), tile).second)
                    {
#ifdef _DEBUG
                        std::cerr << "Duplicate tile: " << tileCoordinates.x << " " << tileCoordinates.y << std::endl;
#endif
                        delete tile;
                        tile = nullptr;
                        continue;
                    }

                    std::string warp;
                    keyStream >> warp;

                    tile->is_warp_ = false;
                    if (warp == "WARP")
                        tile->is_warp_ = true;
                }
                else if (type == "BACKGROUND")
                {
                    if (!background_texture_.empty())
                        continue;

                    keyStream >> background_texture_;
                    TextureManager* textureMgr = context_->texture_mgr_;

                    if (!textureMgr->RequireResource(background_texture_))
                    {
                        background_texture_.clear();
                        continue;
                    }

                    sf::Texture* texture = textureMgr->GetResource(background_texture_);
                    background_.setTexture(*texture);

                    // scale the sprite enough to fit the view space fully.
                    sf::Vector2f viewSize = current_state_->GetView().getSize();
                    sf::Vector2u textureSize = texture->getSize();
                    sf::Vector2f scaleFactors;
                    scaleFactors.x = viewSize.x / textureSize.x;
                    scaleFactors.y = viewSize.y / textureSize.y;
                    background_.setScale(scaleFactors);
                }
                else if (type == "SIZE")
                {
                    keyStream >> max_map_size_.x >> max_map_size_.y;
                }
                else if (type == "GRAVITY")
                {
                    keyStream >> map_gravity_;
                }
                else if (type == "DEFAULT_FRICTION")
                {
                    keyStream >> default_tile_.friction_.x >> default_tile_.friction_.y;
                }
                else if (type == "NEXTMAP")
                {
                    keyStream >> next_map_;
                }
            }

            ifs.close();
        }

        /**
         * \brief Keep track of when the next map should be loaded.
         */
        void LoadNextMap()
        {
            is_load_next_map_ = true;
        }

        /**
         * \brief Update the map.
         */
        void Update(float deltaTime)
        {
            if (is_load_next_map_)
            {
                PurgeMap();
                is_load_next_map_ = false;

                if (!next_map_.empty())
                    LoadMapFromConfigFile("media/maps/" + next_map_);
                else
                    current_state_->GetStateManager()->SwitchTo(StateType::GAME_OVER);
                // simulate player beating the game.

                next_map_.clear();
            }

            sf::FloatRect viewSpace = context_->window_->GetViewSpace();
            background_.setPosition(viewSpace.left, viewSpace.top); // background follows the camera.
        }

        /**
         * \brief Draw background and tiles in view space.
         * Anything is not currently within the view space should be left undrawn.
         */
        void Draw()
        {
            sf::RenderWindow& window = context_->window_->GetRenderWindow();
            window.draw(background_); // draw background first.

            sf::FloatRect viewSpace = context_->window_->GetViewSpace();
            sf::Vector2i tileBegin(floor(viewSpace.left / TILE_SIZE), floor(viewSpace.top / TILE_SIZE));
            // floor: round downward.
            sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / TILE_SIZE),
                                 ceil((viewSpace.top + viewSpace.height) / TILE_SIZE)); // ceil: round upward.

            // draw tiles.
            unsigned int count = 0;
            for (int x = tileBegin.x; x <= tileEnd.x; x++)
            {
                for (int y = tileBegin.y; y <= tileEnd.y; y++)
                {
                    if (x < 0 || y < 0) // go left or up far enough?
                        continue;

                    Tile* tile = GetTile(x, y);
                    if (!tile)
                        continue;

                    sf::Sprite& sprite = tile->properties_->sprite_;
                    sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    window.draw(sprite);
                    ++count;
                }
            }
        }

    private:
        typedef std::unordered_map<TileID, Tile*> TileMap;
        typedef std::unordered_map<TileID, TileInfo*> TileSet;

        TileSet tile_set_; // different types of tile.
        TileMap tile_map_;
        sf::Sprite background_;
        TileInfo default_tile_;
        sf::Vector2u max_map_size_;
        sf::Vector2f player_start_;
        unsigned int tile_count_;
        unsigned int tile_set_count_;
        float map_gravity_;
        std::string next_map_;
        bool is_load_next_map_;
        std::string background_texture_; // name of background loaded from file.
        BaseState* current_state_;
        SharedContext* context_;

        /**
         * \brief Convert 2D coordinates to 1D.
         */
        unsigned int ConvertCoordinates(unsigned int x, unsigned int y)
        {
            return (x * max_map_size_.x + y);
        }

        /**
         * \brief Load different types of tiles from the path file.
         */
        void LoadTiles(const std::string& path)
        {
            std::ifstream ifs;
            ifs.open(Utilities::GetWorkingDirectoryA() + path, std::ifstream::in);
            if (!ifs.is_open())
            {
#ifdef _DEBUG
                std::cerr << "Could not load tile set file: " << path << std::endl;
#endif
                return;
            }
            std::string line;
            while (std::getline(ifs, line))
            {
                if (line[0] == '|') // ignore comment
                    continue;

                std::stringstream keyStream(line);
                int tileId;
                keyStream >> tileId;
                if (tileId < 0) // is out of bounds?
                    continue;

                TileInfo* tile = new TileInfo(context_, "TileSheet", tileId);
                keyStream >> tile->name_ >> tile->friction_.x >> tile->friction_.y >> tile->is_deadly_;
                if (!tile_set_.emplace(tileId, tile).second)
                {
#ifdef _DEBUG
                    std::cerr << "Duplicate tile type: " << tile->name_ << std::endl;
#endif
                    delete tile;
                }
            }

            ifs.close();
        }

        /**
         * \brief Erase entire map.
         */
        void PurgeMap()
        {
            tile_count_ = 0;
            for (auto& itr : tile_map_)
            {
                delete itr.second;
            }

            tile_map_.clear();
            context_->entity_mgr_->Purge();

            if (background_texture_.empty())
                return;

            // free up background texture
            context_->texture_mgr_->ReleaseResource(background_texture_);
            background_texture_.clear();
        }

        /**
         * \brief Empty the container of different tile types.
         */
        void PurgeTileSet()
        {
            for (auto& itr : tile_set_)
            {
                delete itr.second;
            }

            tile_set_.clear();
            tile_set_count_ = 0;
        }
    };
}
