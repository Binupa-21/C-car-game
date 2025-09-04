#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AssetManager {
private:
    std::map<std::string, sf::Texture> m_Textures;
    std::map<std::string, sf::SoundBuffer> m_SoundBuffers;
    AssetManager() = default;
public:
    static AssetManager& GetInstance();
    static sf::Texture& GetTexture(const std::string& filename);
    static sf::SoundBuffer& GetSoundBuffer(const std::string& filename);
};
