#pragma once

#include "AnimationBase.h"
#include "SpriteSheet.h" // ??? in .cpp file

namespace SFMLTutorial
{
    class AnimationDirectional : public AnimationBase
    {
    protected:
        void FrameStep() override
        {
            // check which direction to roll frames in?
            if (frame_start_ < frame_end_)
                ++frame_current_;
            else
                --frame_current_;

            // check if the frame is out of bounds?
            if ((frame_start_ < frame_end_ && frame_current_ > frame_end_) || (frame_start_ > frame_end_ &&
                frame_current_ < frame_end_))
            {
                if (is_loop_)
                {
                    frame_current_ = frame_start_;
                    return;
                }

                frame_current_ = frame_end_;
                Pause();
            }
        }

        void CropSprite() override
        {
            // construct a rectangle based on top-left corner and size (in pixel) of sprite.
            sf::IntRect rect(sprite_sheet_->GetSpriteSize().x * frame_current_,
                             sprite_sheet_->GetSpriteSize().y * (frame_row_ + static_cast<unsigned int>(sprite_sheet_->
                                 GetDirection())), sprite_sheet_->GetSpriteSize().x, sprite_sheet_->GetSpriteSize().y);
            sprite_sheet_->CropSprite(rect);
        }

        void ReadIn(std::stringstream& stream) override
        {
            stream >> frame_start_ >> frame_end_ >> frame_row_ >> frame_time_ >> frame_action_start_ >>
                frame_action_end_;
        }
    };
}
