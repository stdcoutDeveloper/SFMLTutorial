#pragma once

#include "pch.h"
#include "Direction.h"
#include "TextureManager.h"
#include "AnimationBase.h"
#include <string>
#include <unordered_map>

namespace SFMLTutorial
{
    class SpriteSheet
    {
    public:
        SpriteSheet(TextureManager* textureMgr) : sprite_scale_(1.0f, 1.0f), direction_(Direction::RIGHT),
                                                  animation_current_(nullptr), texture_mgr_(textureMgr)
        {
        }

        ~SpriteSheet()
        {
            ReleaseSheet();
        }

        /**
         * \brief Crop sprite from texture.
         * @param rect: top-left corner coordinates are local to the texture
         * and the size of the rectangle.
         */
        void CropSprite(const sf::IntRect& rect)
        {
            sprite_.setTextureRect(rect);
        }

        bool LoadSheet(const std::string& file)
        {
        }

        /**
         * \brief Release resources it was using.
         */
        void ReleaseSheet()
        {
            texture_mgr_->ReleaseResource(texture_);
            animation_current_ = nullptr;
            while (animations_.begin() != animations_.end())
            {
                delete animations_.begin()->second;
                animations_.erase(animations_.begin());
            }
        }

        AnimationBase* GetCurrentAnimation()
        {
            return animation_current_;
        }

        void SetSpriteSize(const sf::Vector2i& size)
        {
            sprite_size_ = size;
            sprite_.setOrigin(sprite_size_.x / 2.0f, sprite_size_.y * 1.0f);
        }

        void SetSpritePosition(const sf::Vector2f& position)
        {
            sprite_.setPosition(position);
        }

        void SetDirection(const Direction& direction)
        {
            if (direction_ == direction)
                return;

            direction_ = direction;
            animation_current_->CropSprite(); // TODO ???
        }

        /**
         * \brief 
         * @param name: name of animation
         * @param isPlay: whether play this animation instantly?
         * @param isLoop: whether it's looped or not?
         */
        bool SetAnimation(const std::string& name, const bool& isPlay = false, const bool& isLoop = false)
        {
            auto itr = animations_.find(name);
            if (itr == animations_.end())
                return false;

            if (itr->second == animation_current_)
                return false;

            if (animation_current_)
                animation_current_->Stop();

            animation_current_ = itr->second;
            animation_current_->SetLooping(isLoop);
            if (isPlay)
                animation_current_->Play();

            animation_current_->CropSprite();
            return true;
        }

        void Update(const float& deltaTime)
        {
            animation_current_->Update(deltaTime);
        }

        void Draw(sf::RenderWindow* window)
        {
            window->draw(sprite_);
        }

    private:
        typedef std::unordered_map<std::string, AnimationBase*> Animations;

        std::string texture_;
        sf::Sprite sprite_;
        sf::Vector2i sprite_size_;
        sf::Vector2f sprite_scale_;
        Direction direction_;
        std::string animation_type_;
        Animations animations_;
        AnimationBase* animation_current_;
        TextureManager* texture_mgr_;
    };
}
