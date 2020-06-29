#pragma once

#include "SharedContext.h"
#include "TextureManager.h"
#include "BaseState.h"

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
        }

        float GetGravity() const
        {
        }

        unsigned int GetTileSize() const
        {
        }

        const sf::Vector2u& GetMapSize() const
        {
        }

        const sf::Vector2f& GetPlayerStart() const
        {
        }

        void LoadMap(const std::string& path)
        {
        }

        void LoadNext()
        {
        }

        void Update(float deltaTime)
        {
        }

        void Draw()
        {
        }

    private:
        typedef std::unordered_map<TileID, Tile*> TileMap;
        typedef std::unordered_map<TileID, TileInfo*> TileSet;

        TileSet tile_set_;
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
        std::string background_texture_;
        BaseState* current_state_;
        SharedContext* context_;

        /**
         * \brief Convert 2D coordinates to 1D.
         */
        unsigned int ConvertCoordinates(unsigned int x, unsigned int y)
        {
        }

        /**
         * \brief Load different types of tiles from the path file.
         */
        void LoadTiles(const std::string& path)
        {
        }

        void PurgeMap()
        {
        }

        void PurgeTileSet()
        {
        }
    };
}
