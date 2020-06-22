#pragma once
#include "pch.h"
#include <vector>

namespace SFMLTutorial
{
    /**
     * \brief SnakeSegment represents for a segment of the snake.
     */
    struct SnakeSegment
    {
        SnakeSegment(int x, int y) : position_(x, y)
        {
        }

        sf::Vector2i position_;
    };

    enum class Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Snake
    {
    public:
        Snake(int blockSize) : graphics_size_(blockSize)
        {
            body_rect_.setSize(sf::Vector2f(static_cast<float>(graphics_size_ - 1),
                                            static_cast<float>(graphics_size_ - 1)));
            Reset();
        }

        ~Snake() = default;

        void SetDirection(Direction dir)
        {
            direction_ = dir;
        }

        Direction GetDirection()
        {
            return direction_;
        }

        int GetSpeed()
        {
            return speed_;
        }

        sf::Vector2i GetPosition()
        {
            return (!snake_body_.empty() ? snake_body_.front().position_ : sf::Vector2i(1, 1));
        }

        int GetLives()
        {
            return lives_;
        }

        int GetScore()
        {
            return score_;
        }

        void IncreaseScore()
        {
            score_ += 10;
        }

        bool IsLost()
        {
            return is_lost_;
        }

        void Lose()
        {
            is_lost_ = true;
        }

        void ToggleLost()
        {
            is_lost_ = !is_lost_;
        }

        void Extend()
        {
            if (snake_body_.empty())
                return;

            SnakeSegment& tailHead = snake_body_[snake_body_.size() - 1];
            if (snake_body_.size() > 1)
            {
                // extend to tail of snake.
                SnakeSegment& tailBone = snake_body_[snake_body_.size() - 2];
                if (tailHead.position_.x == tailBone.position_.x)
                {
                    if (tailHead.position_.y > tailBone.position_.y)
                        snake_body_.push_back(SnakeSegment(tailHead.position_.x, tailHead.position_.y + 1));
                    else
                        snake_body_.push_back(SnakeSegment(tailHead.position_.x, tailHead.position_.y - 1));
                }
                else if (tailHead.position_.y == tailBone.position_.y)
                {
                    if (tailHead.position_.x > tailBone.position_.x)
                        snake_body_.push_back(SnakeSegment(tailHead.position_.x + 1, tailHead.position_.y));
                    else
                        snake_body_.push_back(SnakeSegment(tailHead.position_.x - 1, tailHead.position_.y));
                }
            }
            else
            {
                // extended direction reverses to moving direction.
                if (direction_ == Direction::UP)
                    snake_body_.push_back(SnakeSegment(tailHead.position_.x, tailHead.position_.y + 1));
                else if (direction_ == Direction::DOWN)
                    snake_body_.push_back(SnakeSegment(tailHead.position_.x, tailHead.position_.y - 1));
                else if (direction_ == Direction::LEFT)
                    snake_body_.push_back(SnakeSegment(tailHead.position_.x + 1, tailHead.position_.y));
                else if (direction_ == Direction::RIGHT)
                    snake_body_.push_back(SnakeSegment(tailHead.position_.x - 1, tailHead.position_.y));
            }
        }

        /**
         * \brief Set snake into starting position.
         */
        void Reset()
        {
            snake_body_.clear();

            snake_body_.push_back(SnakeSegment(5, 7)); // head
            snake_body_.push_back(SnakeSegment(5, 6));
            snake_body_.push_back(SnakeSegment(5, 5)); // tail

            SetDirection(Direction::NONE);
            speed_ = 15;
            lives_ = 3;
            score_ = 0;
            is_lost_ = false;
        }

        void Move()
        {
            // move the body (exclude the head).
            for (int i = snake_body_.size() - 1; i > 0; i--)
            {
                snake_body_[i].position_ = snake_body_[i - 1].position_;
            }
            // move the head.
            if (direction_ == Direction::LEFT)
                --snake_body_[0].position_.x;
            if (direction_ == Direction::RIGHT)
                ++snake_body_[0].position_.x;
            if (direction_ == Direction::UP)
                --snake_body_[0].position_.y;
            if (direction_ == Direction::DOWN)
                ++snake_body_[0].position_.y;
        }

        void Update()
        {
            if (snake_body_.empty() || direction_ == Direction::NONE)
                return;

            Move();
            CheckCollision();
        }

        // @param cutSegmentNumbers: number of segments to cut.
        void Cut(int cutSegmentNumbers)
        {
            for (int i = 0; i < cutSegmentNumbers; i++)
            {
                snake_body_.pop_back(); // remove last element in vector.
            }

            lives_--;
            if (lives_ == 0)
                Lose();
        }

        void Render(sf::RenderWindow& window)
        {
            if (snake_body_.empty())
                return;

            // draw head of snake.
            auto head = snake_body_.begin();
            body_rect_.setFillColor(sf::Color::Yellow);
            body_rect_.setPosition(static_cast<float>(head->position_.x * graphics_size_),
                                   static_cast<float>(head->position_.y * graphics_size_));
            window.draw(body_rect_);

            // draw body of snake.
            body_rect_.setFillColor(sf::Color::Green);
            for (auto itr = snake_body_.begin() + 1; itr != snake_body_.end(); ++itr)
            {
                body_rect_.setPosition(static_cast<float>(itr->position_.x * graphics_size_),
                                       static_cast<float>(itr->position_.y * graphics_size_));
                window.draw(body_rect_);
            }
        }

        Direction GetPhysicalDirection()
        {
            if (snake_body_.size() <= 1)
                return Direction::NONE;

            SnakeSegment& head = snake_body_[0];
            SnakeSegment& neck = snake_body_[1];
            if (head.position_.x == neck.position_.x)
                return (head.position_.y > neck.position_.y ? Direction::DOWN : Direction::UP);
            if (head.position_.y == neck.position_.y)
                return (head.position_.x > neck.position_.x ? Direction::RIGHT : Direction::LEFT);

            return Direction::NONE;
        }

    private:
        typedef std::vector<SnakeSegment> SnakeContainer;
        SnakeContainer snake_body_;
        Direction direction_;
        sf::RectangleShape body_rect_;
        int graphics_size_, speed_, lives_, score_;
        bool is_lost_;

        /**
         * \brief Check if snake collides itself.
         */
        void CheckCollision()
        {
            if (snake_body_.size() < 5)
                return;
            SnakeSegment& head = snake_body_.front();
            for (auto itr = snake_body_.begin() + 1; itr != snake_body_.end(); ++itr)
            {
                if (itr->position_ == head.position_)
                {
                    int cutSegmentNumbers = snake_body_.end() - itr;
                    Cut(cutSegmentNumbers);
                    break;
                }
            }
        }
    };
}
