#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <iterator>

namespace SFMLTutorial
{
    class EntityBase; // ???
    enum class EntityType;
    struct SharedContext; // forward declaration

    class EntityManager
    {
    public:
        EntityManager(SharedContext* context, unsigned int maxEntities) : context_(context), id_counter_(0),
                                                                          max_entities_(maxEntities)
        {
            LoadEnemyTypesFromFile("EnemyList.list");
            //RegisterEntity<Player>(EntityType::PLAYER);
            //RegisterEntity<Enemy>(EntityType::ENEMY);
        }

        ~EntityManager()
        {
            Purge();
        }

        /**
         * \brief 
         * @return -1: If failed, id counter of entity if success.
         */
        int AddEntity(const EntityType& type, const std::string& name = "")
        {
            auto itr = entity_factory_.find(type);
            if (itr == entity_factory_.end())
                return -1;

            EntityBase* entity = (itr->second)(); // call std::function(): new T(this)
            entity->id_ = id_counter_;
            if (!name.empty())
                entity->name_ = name;

            entities_.emplace(id_counter_, entity);

            if (type == EntityType::ENEMY)
            {
                auto enemyItr = enemy_types_.find(name);
                if (enemyItr != enemy_types_.end())
                {
                    /*Enemy* enemy = dynamic_cast<Enemy*>(entity);
                    if (enemy)
                        enemy->Load(enemyItr->second);*/
                }
            }

            ++id_counter_;
            return (id_counter_ - 1);
        }

        EntityBase* FindEntityById(unsigned int id)
        {
            auto itr = entities_.find(id);
            return (itr != entities_.end() ? itr->second : nullptr);
        }

        EntityBase* FindEntityByName(const std::string& name)
        {
            for (auto& itr : entities_)
            {
                if (itr.second->name_ == name)
                    return itr.second;
            }
            return nullptr;
        }

        void RemoveEntity(unsigned int id)
        {
            entities_to_remove_.emplace_back(id);
        }

        /**
         * \brief Update all entities.
         */
        void Update(float deltaTime)
        {
            for (auto& itr : entities_)
            {
                itr.second->Update(deltaTime);
            }
            CheckEntityCollision();
            ProcessRemovals();
        }

        /**
         * \brief Draw all entities.
         */
        void Draw()
        {
            sf::RenderWindow& window = context_->window_->GetRenderWindow();
            sf::FloatRect viewSpace = context_->window_->GetViewSpace();

            for (auto& itr : entities_)
            {
                if (!viewSpace.intersects(itr.second->collision_bounding_box_))
                    continue;

                itr.second->Draw(&window);
            }
        }

        /**
         * \brief Release, de-allocate all resources.
         */
        void Purge()
        {
            for (auto& itr : entities_)
            {
                delete itr.second;
            }
            entities_.clear();

            id_counter_ = 0;
        }

        SharedContext* GetContext()
        {
            return context_;
        }

    private:
        typedef std::unordered_map<unsigned int, EntityBase*> EntityContainer;
        typedef std::unordered_map<EntityType, std::function<EntityBase*(void)>> EntityFactory;
        // second argument: the character file path
        typedef std::unordered_map<std::string, std::string> EnemyTypes;

        EntityContainer entities_;
        EnemyTypes enemy_types_;
        EntityFactory entity_factory_;

        SharedContext* context_;
        unsigned int id_counter_; // highest ID was given to an entity 
        unsigned int max_entities_;

        std::vector<unsigned int> entities_to_remove_;

        /**
         * \brief Insert lambda functions into the entity factory container.
         */
        template <class T>
        void RegisterEntity(const EntityType& type)
        {
            entity_factory_[type] = [this]()-> EntityBase*
            {
                return new T(this);
            };
        }

        /**
         * \brief Resolve collisions between entities.
         */
        void ProcessRemovals()
        {
            while (entities_to_remove_.begin() != entities_to_remove_.end())
            {
                unsigned int id = entities_to_remove_.back(); // last element
                auto itr = entities_.find(id);
                if (itr != entities_.end())
                {
                    delete itr->second;
                    entities_.erase(itr);
                }
                entities_to_remove_.pop_back(); // remove the last element
            }
        }

        /**
         * \brief Load pair of enemy names and their character definition files.
         */
        void LoadEnemyTypesFromFile(const std::string& name)
        {
            std::ifstream ifs;
            ifs.open(Utilities::GetWorkingDirectoryA() + "media/Characters/" + name);
            if (!ifs.is_open())
                return;

            std::string line;
            while (std::getline(ifs, line))
            {
                if (line[0] == '|')
                    continue;

                std::stringstream keyStream(line);
                std::string name;
                std::string charFile;
                keyStream >> name >> charFile;
                enemy_types_.emplace(name, charFile);
            }

            ifs.close();
        }

        /**
         * \brief Check for collisions between entities.
         */
        void CheckEntityCollision()
        {
            if (entities_.empty())
                return;

            // check every entity against every other entity.
            for (auto itr = entities_.begin(); std::next(itr) != entities_.end(); ++itr)
            {
                for (auto nextItr = std::next(itr); nextItr != entities_.end(); ++nextItr)
                {
                    if (itr->first == nextItr->first) // same id?
                        continue;

                    if (itr->second->collision_bounding_box_.intersects(nextItr->second->collision_bounding_box_))
                        // collision?
                    {
                        // handling the collision.
                        itr->second->HandleCollisionWithOtherEntity(nextItr->second, false);
                        nextItr->second->HandleCollisionWithOtherEntity(itr->second, false);
                    }

                    EntityType firstType = itr->second->type_;
                    EntityType secondType = nextItr->second->type_;
                    if (firstType == EntityType::PLAYER || firstType == EntityType::ENEMY)
                    {
                        /*Character* firstCharacter = dynamic_cast<Character*>(itr->second);
                        if (firstCharacter)
                        {
                            if (firstCharacter->attack_box_.intersects(nextItr->second->collision_bounding_box_))
                                firstCharacter->HandleCollisionWithOtherEntity(nextItr->second, true);
                        }*/
                    }

                    if (secondType == EntityType::PLAYER || secondType == EntityType::ENEMY)
                    {
                        /*Character* secondCharacter = dynamic_cast<Character*>(nextItr->second);
                        if (secondCharacter)
                        {
                            if (secondCharacter->attack_box_.intersects(itr->second->collision_bounding_box_))
                                secondCharacter->HandleCollisionWithOtherEntity(itr->second, true);
                        }*/
                    }
                }
            }
        }
    };
}
