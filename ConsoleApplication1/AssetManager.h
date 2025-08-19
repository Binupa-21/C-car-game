#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
public:
    static AssetManager& GetInstance();
    static sf::Texture& GetTexture(const std::string& filename);

private:
    AssetManager() = default;
    std::map<std::string, sf::Texture> m_Textures;
};
