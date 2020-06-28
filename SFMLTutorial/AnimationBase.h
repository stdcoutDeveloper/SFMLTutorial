#pragma once

#include <string>
#include <sstream>

namespace SFMLTutorial
{
    class SpriteSheet; // forward declaration: prevent cross-inclusions.

    class AnimationBase
    {
    public:
        AnimationBase()
        {
        }

        virtual ~AnimationBase()
        {
        }

        void Play()
        {
        }

        void Pause()
        {
        }

        void Stop()
        {
        }

        void Reset()
        {
        }

        virtual void Update(const float& deltaTime)
        {
        }

        friend std::stringstream& operator >>(std::stringstream& stream, AnimationBase& anim)
        {
            anim.ReadIn(stream);
            return stream;
        }

    private:
        typedef unsigned int Frame;

        friend class SpriteSheet;

    protected:
        virtual void FrameStep() = 0;
        virtual void CropSprite() = 0;
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
        std::string name_;
        SpriteSheet* sprite_sheet_;
    };
}
