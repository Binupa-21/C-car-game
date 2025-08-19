#include "AssetManager.h"
#include <iostream>

AssetManager& AssetManager::GetInstance() {
    static AssetManager instance;
    return instance;
}

sf::Texture& AssetManager::GetTexture(const std::string& filename) {
    auto& texMap = GetInstance().m_Textures;

    // Check if the texture is already loaded
    auto pairFound = texMap.find(filename);
    if (pairFound != texMap.end()) {
        return pairFound->second;
    } else {
        // Load the texture and store it in the map
        auto& texture = texMap[filename];
        if (!texture.loadFromFile(filename)) {
            std::cout << "Failed to load texture: " << filename << std::endl;
        }
        return texture;
    }
}