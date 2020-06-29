#pragma once

#include <string>
#include <sstream>

namespace SFMLTutorial
{
    class SpriteSheet; // forward declaration: prevent cross-inclusions.

    class AnimationBase
    {
    public:
        AnimationBase() : frame_current_(0), frame_start_(0), frame_end_(0), frame_row_(0), frame_action_start_(-1),
                          frame_action_end_(-1), frame_time_(0.0f), elapsed_time_(0.0f), is_loop_(false),
                          is_playing_(false)
        {
        }

        virtual ~AnimationBase()
        {
        }

        void SetSpriteSheet(SpriteSheet* spriteSheet)
        {
            sprite_sheet_ = spriteSheet;
        }

        void SetLooping(bool isLoop)
        {
            is_loop_ = isLoop;
        }

        void SetName(const std::string& name)
        {
            name_ = name;
        }

        /**
         * \brief Add support for types of animation that can play backwards in the future.
         */
        void SetFrame(const unsigned int& frame)
        {
            if ((frame >= frame_start_ && frame <= frame_end_) || (frame >= frame_end_ && frame <= frame_start_)) // ???
                frame_current_ = frame;
        }

        /**
         * \brief Check whether animation is currently able to perform its custom behavior.
         */
        bool IsInAction()
        {
            if (frame_action_start_ == -1 || frame_action_end_ == -1) // -1: action is always performed.
                return true;

            return (frame_current_ >= frame_action_start_ && frame_current_ <= frame_action_end_);
        }

        void Play()
        {
            is_playing_ = true;
        }

        void Pause()
        {
            is_playing_ = false;
        }

        void Stop()
        {
            is_playing_ = false;
            Reset();
        }

        /**
         * \brief Reset animation
         */
        void Reset()
        {
            frame_current_ = frame_start_;
            elapsed_time_ = 0.0f;
            CropSprite(); // crop sprite because of the frame change.
        }

        /**
         * \brief 
         * @param deltaTime: the elapsed time between frames.
         */
        virtual void Update(const float& deltaTime)
        {
            if (!is_playing_)
                return;

            elapsed_time_ += deltaTime;
            if (elapsed_time_ < frame_time_)
                return;

            FrameStep();
            CropSprite();
            elapsed_time_ = 0.0f;
        }

        /**
         * \brief overload operator >> to ease animation loading from file.
         */
        friend std::stringstream& operator >>(std::stringstream& stream, AnimationBase& anim)
        {
            anim.ReadIn(stream);
            return stream;
        }

    private:
        typedef unsigned int Frame;

        friend class SpriteSheet;

    protected:
        /**
         * \brief Update portion that is unique to different types of animation.
         */
        virtual void FrameStep() = 0;

        /**
         * \brief Obtain sprite from a sprite sheet.
         */
        virtual void CropSprite() = 0;

        /**
         * \brief Loading data from files.
         */
        virtual void ReadIn(std::stringstream& stream) = 0;

        Frame frame_current_;
        Frame frame_start_;
        Frame frame_end_;
        Frame frame_row_;
        int frame_action_start_; // frame when a specific action begins
        int frame_action_end_; // frame when a specific action ends
        float frame_time_; // amount of time each frame takes to finish
        float elapsed_time_;
        bool is_loop_;
        bool is_playing_;
        std::string name_; // animation name
        SpriteSheet* sprite_sheet_;
    };
}
