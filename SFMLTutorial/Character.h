#pragma once

#include "EntityBase.h"
#include "SpriteSheet.h"

namespace SFMLTutorial
{
    class EntityManager; // forward declaration

    class Character : public EntityBase
    {
    public:
        Character(EntityManager* entityMgr) : EntityBase(entityMgr),
                                              sprite_sheet_(entity_mgr_->GetContext()->texture_mgr_),
                                              jump_velocity_(250.0f), hit_points_(5)
        {
            name_ = "Character";
        }

        virtual ~Character()
        {
        }

        void Move(const Direction& dir)
        {
            if (state_ == EntityState::DYING)
                return;

            sprite_sheet_.SetDirection(dir);

            if (dir == Direction::LEFT)
                AddAccelerate(-speed_.x, 0.0f);
            else
                AddAccelerate(speed_.x, 0.0f);

            if (state_ == EntityState::IDLE)
                SetState(EntityState::WALKING);
        }

        void Jump()
        {
            if (state_ == EntityState::DYING || state_ == EntityState::JUMPING || state_ == EntityState::HURT)
                return;

            SetState(EntityState::JUMPING);
            AddVelocity(0.0f, -jump_velocity_);
        }

        void Attack()
        {
        }

        void GetHurt(const int& damage)
        {
        }

        void LoadCharacterFromFile(const std::string& path)
        {
        }

        void HandleCollisionWithOtherEntity(EntityBase* collider, bool isAttack) override = 0;

        void Update(float deltaTime) override
        {
        }

        void Draw(sf::RenderWindow* window) override
        {
        }

    protected:
        SpriteSheet sprite_sheet_;
        float jump_velocity_;
        int hit_points_; // how many times an entity can be hit before it dies.
        sf::FloatRect attack_box_;
        sf::Vector2f attack_box_offset_; // ???

        void UpdateAttackBox()
        {
        }

        void Animate()
        {
        }

    private:
        friend class EntityManager;
    };
}
