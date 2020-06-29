#pragma once

#include "pch.h"
#include "Direction.h"
#include "TextureManager.h"
//#include "AnimationBase.h" // ???
//#include "AnimationDirectional.h" // ???
#include <string>
#include <unordered_map>

namespace SFMLTutorial
{
    class AnimationBase; // forward declaration to avoid circular dependency???

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
            std::ifstream ifs;
            ifs.open(Utilities::GetWorkingDirectoryA() + file, std::ifstream::in);
            if (ifs.is_open())
            {
                ReleaseSheet(); // Release current sheet resources.
                std::string line;
                while (std::getline(ifs, line))
                {
                    if (line[0] == '|')
                        continue;

                    std::stringstream keyStream(line);
                    std::string type;
                    keyStream >> type;

                    if (type == "Texture")
                    {
                        if (!texture_.empty())
                        {
#ifdef _DEBUG
                            std::cerr << "Duplicate texture entries in: " << file << std::endl;
#endif
                            continue;
                        }

                        std::string texture;
                        keyStream >> texture;
                        if (!texture_mgr_->RequireResource(texture))
                        {
#ifdef _DEBUG
                            std::cerr << "Could not setup the texture: " << texture << std::endl;
#endif
                            continue;
                        }

                        texture_ = texture;
                        sprite_.setTexture(*(texture_mgr_->GetResource(texture)));
                    }
                    else if (type == "Size")
                    {
                        keyStream >> sprite_size_.x >> sprite_size_.y;
                        SetSpriteSize(sprite_size_);
                    }
                    else if (type == "Scale")
                    {
                        keyStream >> sprite_scale_.x >> sprite_scale_.y;
                        sprite_.setScale(sprite_scale_);
                    }
                    else if (type == "AnimationType")
                    {
                        keyStream >> animation_type_;
                    }
                    else if (type == "Animation")
                    {
                        std::string name;
                        keyStream >> name;

                        if (animations_.find(name) != animations_.end())
                        {
#ifdef _DEBUG
                            std::cerr << "Duplicate animation: " << name << " in " << file << std::endl;
#endif
                            continue;
                        }

                        AnimationBase* anim = nullptr;
                        if (animation_type_ == "Directional")
                        {
                            anim = new AnimationDirectional();
                        }
                        else
                        {
#ifdef _DEBUG
                            std::cerr << "Unknown animation type: " << animation_type_ << std::endl;
#endif
                            continue;
                        }

                        keyStream >> *anim; // overload operator >>.
                        anim->SetSpriteSheet(this);
                        anim->SetName(name);
                        anim->Reset();

                        animations_.emplace(name, anim);
                        if (animation_current_) // whether current animation has been assigned a value yet?
                            continue;

                        animation_current_ = anim;
                        animation_current_->Play();
                    }
                }

                ifs.close();
                return true;
            }

#ifdef _DEBUG
            std::cerr << "Could not loading spritesheet: " << file << std::endl;
#endif
            return false;
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

        sf::Vector2i GetSpriteSize() const
        {
            return sprite_size_;
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

        Direction GetDirection() const
        {
            return direction_;
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
