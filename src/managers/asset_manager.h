#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

#include "../assets/asset.h"

namespace AssetManager {
    inline std::unordered_map<std::string, std::shared_ptr<Asset>> assets;

    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path, Args&&... args) {
        if (const auto it = assets.find(path); it != assets.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        auto asset = std::make_shared<T>(path, std::forward<Args>(args)...);

        if (asset->load()) {
            std::cout << path << " loaded asset " << path << " from disk " << std::endl;;
            assets.insert(std::make_pair(path, asset));
            return asset;
        }

        return nullptr;
    }
}
