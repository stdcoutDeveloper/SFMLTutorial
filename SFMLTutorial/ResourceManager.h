#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Utilities.h"

namespace SFMLTutorial
{
    template <typename Derived, typename T>
    class ResourceManager
    {
    public:
        ResourceManager(const std::string& filePath)
        {
            LoadPaths(filePath);
        }

        virtual ~ResourceManager()
        {
            PurgeResources();
        }

        T* GetResource(const std::string& id)
        {
            auto resource = Find(id);
            return (resource ? resource->first : nullptr);
        }

        /**
         * \brief Retrieving one of paths to a particular resource.
         * @param id: id of resource.
         */
        std::string GetPath(const std::string& id)
        {
            auto path = paths_.find(id);
            return (path != paths_.end() ? path->second : "");
        }

        /**
         * \brief Register resource when resource is required.
         * If resource is being used, just simply increase its counter.
         * If not, create it.
         */
        bool RequireResource(const std::string& id)
        {
            // if resource is being used.
            auto resource = Find(id);
            if (resource)
            {
                ++(resource->second); // increase counter
                return true;
            }

            // if resource is not besing used anywhere.
            auto path = paths_.find(id);
            if (path == paths_.end())
                return false;

            T* res = Load(path->second);
            if (!res)
                return false;

            resources_.emplace(id, std::make_pair(res, 1));
            return true;
        }

        /**
         * \brief Unload resource when resource is no longer used anywhere.
         * If it is still being used somewhere, just simply decrease its counter.
         */
        bool ReleaseResource(const std::string& id)
        {
            auto res = Find(id);
            if (!res)
                return false;

            // if found
            auto counter = res->second;
            --counter;
            if (counter == 0) // is no longer need?
                Unload(id);

            return true;
        }

    private:
        // first string: id, second string: path
        typedef std::unordered_map<std::string, std::string> Paths;

        // T*: pointer to resource, unsigned int: counter how many places are using this resource.
        typedef std::pair<T*, unsigned int> ResourceCounter;

        // string: id
        typedef std::unordered_map<std::string, ResourceCounter> Resources;

        Resources resources_;
        Paths paths_;

        /**
         * \brief Load paths from a file.
         * @param filePath: path of file to load data.
         */
        void LoadPaths(const std::string& filePath)
        {
            std::ifstream pathsIfs;
            pathsIfs.open(Utilities::GetWorkingDirectoryA() + filePath, std::ifstream::in);
            if (pathsIfs.is_open())
            {
                std::string line;
                while (std::getline(pathsIfs, line)) // read lines from input stream.
                {
                    std::stringstream keyStream(line); // operate on string.
                    std::string pathName;
                    std::string path;
                    keyStream >> pathName;
                    keyStream >> path;

                    paths_.emplace(pathName, path);
                }

                pathsIfs.close();
                return;
            }

#ifdef _DEBUG
            std::cerr << "Could not load the path file: " << filePath << std::endl;
#endif
        }

        /**
         * \brief Purge and de-allocate all resources.
         */
        void PurgeResources()
        {
            while (resources_.begin() != resources_.end())
            {
                delete (resources_.begin()->second.first); // delete T*
                resources_.erase(resources_.begin());
            }
        }

        /**
         * \brief Find the resource by id.
         * @param id: id of resource.
         */
        ResourceCounter* Find(const std::string& id)
        {
            auto itr = resources_.find(id);
            return (itr != resources_.end() ? &itr->second : nullptr);
        }

        /**
         * \brief Use Load() method of derived class.
         */
        T* Load(const std::string& path)
        {
            // avoid runtime polymorphism, using CRTP.
            return static_cast<T*>(this)->Load(path); // downcast.
        }

        /**
         * \brief Unload resource when it is not used anywhere.
         */
        bool Unload(const std::string& id)
        {
            auto itr = resources_.find(id);
            if (itr == resources_.end())
                return false;

            delete itr->second.first; // free allocated memory.
            resources_.erase(itr);
            return true;
        }
    };
}
