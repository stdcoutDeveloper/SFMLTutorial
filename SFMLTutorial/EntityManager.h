#pragma once

namespace SFMLTutorial
{
    struct SharedContext; // forward declaration

    class EntityManager
    {
    public:
        void Purge()
        {
        }

        SharedContext* GetContext()
        {
            return context_;
        }

    private:
        SharedContext* context_;
    };
}
