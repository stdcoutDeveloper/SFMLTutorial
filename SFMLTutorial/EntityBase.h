#pragma once

#include "pch.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "EntityManager.h" // incomplete class???
#include "SharedContext.h" // incomplete class???
#include "Map.h" // incomplete class???

namespace SFMLTutorial
{
	// @formatter:off
    enum class EntityType
    {
        BASE, // abstract class
        ENEMY,
        PLAYER
    };
    // @formatter:on

    /**
     * \brief Possible states an entity can have.
     */
    enum class EntityState
    {
        IDLE,
        WALKING,
        JUMPING,
        ATTACKING,
        HURT,
        DYING
    };

    class EntityManager; // forward declaration
    struct TileInfo;

    /**
     * \brief Hold the collision information.
     */
    struct CollisionElement
    {
        CollisionElement(float area, TileInfo* info, const sf::FloatRect& tileBounds) : area_(area), tile_(info),
                                                                                        tile_bounds_(tileBounds)
        {
        }

        float area_; // area of collision
        TileInfo* tile_; // information of tile colliding with entity
        sf::FloatRect tile_bounds_; // boundary information of a tile the entity's colliding with
    };

    /**
     * \brief Compare two collisions based on area size.
     * @return true: if area size is bigger.
     */
    inline bool SortDescendingCollisionsByArea(const CollisionElement& firstElement,
                                               const CollisionElement& secondElement)
    {
        return (firstElement.area_ > secondElement.area_);
    }

    /**
     * \brief Abstract class provides functionalities that any entity should have.
     */
    class EntityBase
    {
    private:
        typedef std::vector<CollisionElement> Collisions;

        friend class EntityManager; // EntityManager could use private and protected method of EntityBase

    public:
        EntityBase(EntityManager* entityMgr) : name_("BaseEntity"), type_(EntityType::BASE), id_(0),
                                               reference_tile_(nullptr), state_(EntityState::IDLE),
                                               is_colliding_on_x_(false), is_colliding_on_y_(false),
                                               entity_mgr_(entityMgr)
        {
        }

        virtual ~EntityBase()
        {
        }

        void SetCurrentPosition(const float& x, const float& y)
        {
            current_position_ = sf::Vector2f(x, y);
            UpdateBoundingBoxPosition();
        }

        void SetCurrentPosition(const sf::Vector2f& position)
        {
            current_position_ = position;
            UpdateBoundingBoxPosition();
        }

        void SetCollisionBoxSize(const float& x, const float& y)
        {
            collision_box_size_ = sf::Vector2f(x, y);
            UpdateBoundingBoxPosition();
        }

        void SetState(const EntityState& state)
        {
            if (state_ == EntityState::DYING) // is dying?
                return;

            state_ = state;
        }

        /**
         * \brief Adjust entity's position in the world.
         */
        void Move(float x, float y)
        {
            old_position_ = current_position_;
            current_position_ += sf::Vector2f(x, y);

            sf::Vector2u mapSize = entity_mgr_->GetContext()->game_map_->GetMapSize();
            if (current_position_.x < 0)
            {
                current_position_.x = 0;
            }
            else if (current_position_.x > (mapSize.x + 1) * TILE_SIZE) // ???
            {
                current_position_.x = (mapSize.x + 1) * TILE_SIZE;
            }

            if (current_position_.y < 0)
            {
                current_position_.y = 0;
            }
            else if (current_position_.y > (mapSize.y + 1) * TILE_SIZE)
            {
                current_position_.y = (mapSize.y + 1) * TILE_SIZE;
                SetState(EntityState::DYING);
            }

            UpdateBoundingBoxPosition(); // reflect the changes to the position.
        }

        void AddVelocity(float x, float y)
        {
            velocity_ += sf::Vector2f(x, y);

            if (abs(velocity_.x) > max_velocity_.x)
            {
                if (velocity_.x < 0)
                    velocity_.x = -max_velocity_.x;
                else
                    velocity_.x = max_velocity_.x;
            }

            if (abs(velocity_.y) > max_velocity_.y)
            {
                if (velocity_.y < 0)
                    velocity_.y = -max_velocity_.y;
                else
                    velocity_.y = max_velocity_.y;
            }
        }

        void AddAccelerate(float x, float y)
        {
            acceleration_ += sf::Vector2f(x, y);
        }

        void SetAcceleration(float x, float y)
        {
            acceleration_ = sf::Vector2f(x, y);
        }

        /**
         * \brief Manipulate velocity.
         */
        void ApplyFriction(float x, float y)
        {
            if (velocity_.x != 0)
            {
                if (abs(velocity_.x) - abs(x) < 0)
                {
                    velocity_.x = 0;
                }
                else
                {
                    // friction direction is reversed in moving direction.
                    if (velocity_.x < 0)
                        velocity_.x += x;
                    else
                        velocity_.x -= x;
                }
            }

            if (velocity_.y != 0)
            {
                if (abs(velocity_.y) - abs(y) < 0)
                {
                    velocity_.y = 0;
                }
                else
                {
                    if (velocity_.y < 0)
                        velocity_.y += y;
                    else
                        velocity_.y -= y;
                }
            }
        }

        virtual void Update(float deltaTime)
        {
            // add gravity and velocity by acceleration.
            Map* map = entity_mgr_->GetContext()->game_map_;
            float gravity = map->GetGravity();
            AddAccelerate(0, gravity);
            AddVelocity(acceleration_.x * deltaTime, acceleration_.y * deltaTime);
            SetAcceleration(0.0f, 0.0f);

            // get friction coefficient
            sf::Vector2f frictionValue;
            if (reference_tile_) // obtain friction from a tile the entity's standing on?
            {
                frictionValue = reference_tile_->friction_;
                if (reference_tile_->is_deadly_)
                    SetState(EntityState::DYING);
            }
            else if (map->GetDefaultTile()) // is entity in mid-air?
            {
                frictionValue = map->GetDefaultTile()->friction_;
            }
            else
            {
                frictionValue = friction_;
            }

            float frictionX = speed_.x * friction_.x * deltaTime;
            float frictionY = speed_.y * friction_.y * deltaTime;
            ApplyFriction(frictionX, frictionY);

            // the change in position.
            sf::Vector2f deltaPosition = velocity_ * deltaTime;
            Move(deltaPosition.x, deltaPosition.y);

            is_colliding_on_x_ = false;
            is_colliding_on_y_ = false;
            CheckCollisions();
            ResolveCollisions();
        }

        virtual void Draw(sf::RenderWindow* window) = 0;

    protected:
        std::string name_;
        EntityType type_;
        unsigned int id_; // entity id in the entity manager.
        sf::Vector2f current_position_;
        sf::Vector2f old_position_; // position before entity moved.
        sf::Vector2f velocity_;
        sf::Vector2f max_velocity_;
        sf::Vector2f speed_;
        sf::Vector2f acceleration_;
        sf::Vector2f friction_;
        TileInfo* reference_tile_;
        sf::Vector2f collision_box_size_;
        sf::FloatRect collision_bounding_box_;
        EntityState state_;
        bool is_colliding_on_x_;
        bool is_colliding_on_y_;
        Collisions collisions_;
        EntityManager* entity_mgr_;

        /**
         * \brief Update position of bounding box.
         */
        void UpdateBoundingBoxPosition()
        {
            // the origin of the bounding box is top left corner
            // the entity's position set to (width / 2, height).
            collision_bounding_box_ = sf::FloatRect(current_position_.x - collision_box_size_.x / 2,
                                                    current_position_.y - collision_box_size_.y, collision_box_size_.x,
                                                    collision_box_size_.y);
        }

        /**
         * \brief Dectect collisions
         */
        void CheckCollisions()
        {
            // get tile size
            Map* gameMap = entity_mgr_->GetContext()->game_map_;
            unsigned int tileSize = gameMap->GetTileSize();

            // calculate range of tile coordinates
            int fromX = floor(collision_bounding_box_.left / tileSize);
            int toX = floor((collision_bounding_box_.left + collision_bounding_box_.width) / tileSize);
            int fromY = floor(collision_bounding_box_.top / tileSize);
            int toY = floor((collision_bounding_box_.top + collision_bounding_box_.height) / tileSize);

            for (int x = fromX; x <= toX; x++)
            {
                for (int y = fromY; y <= toY; y++)
                {
                    Tile* tile = gameMap->GetTile(x, y);
                    if (!tile)
                        continue;

                    // get collision information
                    sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
                    // collision bounds of tile
                    sf::FloatRect intersection;
                    collision_bounding_box_.intersects(tileBounds, intersection);
                    float area = intersection.width * intersection.height;

                    CollisionElement element(area, tile->properties_, tileBounds);
                    collisions_.emplace_back(element);

                    if (tile->is_warp_ && type_ == EntityType::PLAYER)
                        gameMap->LoadNextMap();
                }
            }
        }

        void ResolveCollisions()
        {
            if (!collisions_.empty())
            {
                std::sort(collisions_.begin(), collisions_.end(), SortDescendingCollisionsByArea);
                // refactor using lambda expression here???

                // get tile size
                Map* gameMap = entity_mgr_->GetContext()->game_map_;
                unsigned int tileSize = gameMap->GetTileSize();

                for (auto& collisionItr : collisions_)
                {
                    if (!collision_bounding_box_.intersects(collisionItr.tile_bounds_))
                        continue;

                    // calculate distances from the center of the entity's bounding box to the center of the tile's bounding box
                    float xDiff = (collision_bounding_box_.left + collision_bounding_box_.width / 2) - (collisionItr
                                                                                                        .tile_bounds_.
                                                                                                        left +
                        collisionItr.tile_bounds_.width / 2);
                    float yDiff = (collision_bounding_box_.top + collision_bounding_box_.height / 2) - (collisionItr
                                                                                                        .tile_bounds_.
                                                                                                        top +
                        collisionItr.tile_bounds_.height / 2);

                    float resolve = 0.0f;
                    // which axis the resolution takes place?
                    if (abs(xDiff) > abs(yDiff))
                    {
                        // which side of tile the entity is on?
                        if (xDiff > 0) // right side?
                            resolve = collisionItr.tile_bounds_.left + tileSize - collision_bounding_box_.left; // ???
                        else
                            resolve = -((collision_bounding_box_.left + collision_bounding_box_.width) - collisionItr
                                                                                                         .tile_bounds_.
                                                                                                         left); // ???

                        Move(resolve, 0);
                        velocity_.x = 0;
                        is_colliding_on_x_ = true;
                    }
                    else
                    {
                        if (yDiff > 0)
                            resolve = collisionItr.tile_bounds_.top + tileSize - collision_bounding_box_.top;
                        else
                            resolve = -((collision_bounding_box_.top + collision_bounding_box_.height) - collisionItr
                                                                                                         .tile_bounds_.
                                                                                                         top);

                        Move(0, resolve);
                        velocity_.y = 0;
                        if (is_colliding_on_y_)
                            continue;

                        reference_tile_ = collisionItr.tile_;
                        is_colliding_on_y_ = true;
                    }
                }

                collisions_.clear();
            }

            if (!is_colliding_on_y_)
                reference_tile_ = nullptr;
        }

        /**
         * \brief Handle the collision.
         * @param collider: entity that is collided with.
         * @param isAttack: false is a normal collision, true: attack collision.
         */
        virtual void HandleCollisionWithOtherEntity(EntityBase* collider, bool isAttack) = 0;
    };
}
