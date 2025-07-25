#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"
#include "audioinfo.hpp"

struct ResourceManager {
    std::unordered_map<std::string, Texture> m_textures = {};
    std::unordered_map<std::string, Shader> m_shaders = {};
    std::unordered_map<std::string, MusicWrapper> m_musics = {};
    std::unordered_map<std::string, Font> m_fonts = {};
    Texture m_emptyTexture = {};
    Shader m_emptyShader = {};
    MusicWrapper m_emptyMusics = {};
    Font m_emptyFont = {};

    Font& GetFont(std::string relativeFilePath, int fontSize, const char* codepoints = nullptr) {
        std::string fontKey = relativeFilePath + "_" + std::to_string(fontSize);
        if (m_fonts.find(fontKey) == m_fonts.end()) {
            LoadFontFromDisk(relativeFilePath, fontSize, fontKey, codepoints);

            if (m_fonts.find(fontKey) == m_fonts.end()) {
                return m_emptyFont;
            }
        }

        return m_fonts[fontKey];
    }

    Texture& GetTexture(std::string relativeFilePath) {
        if (m_textures.find(relativeFilePath) == m_textures.end()) {
            LoadTextureFromDisk(relativeFilePath);

            // still not found
            if (m_textures.find(relativeFilePath) == m_textures.end()) {
                return m_emptyTexture;
            }
        }

        return m_textures[relativeFilePath];
    }

    MusicWrapper& GetMusic(std::string relativeFilePath) {
        if (m_musics.find(relativeFilePath) == m_musics.end()) {
            LoadMusicFromDisk(relativeFilePath);

            if (m_musics.find(relativeFilePath) == m_musics.end()) {
                return m_emptyMusics;
            }
        }

        return m_musics[relativeFilePath];
    }

private:
    void LoadTextureFromDisk(std::string& relativeFilePath) {
        std::string pathWithPrefix = "resources/" + relativeFilePath;
        m_textures.emplace(relativeFilePath, LoadTexture(pathWithPrefix.c_str()));
    }

    void LoadMusicFromDisk(std::string& relativeFilePath) {
        std::string pathWithPrefix = "resources/" + relativeFilePath;
        m_musics.emplace(relativeFilePath, MusicWrapper{ relativeFilePath, MUSIC_STATES::STOPPED, LoadMusicStream(pathWithPrefix.c_str()) });
    }

    void LoadFontFromDisk(std::string& relativeFilePath, int fontSize, std::string& fontKey, const char* codepointText) {
        std::string pathWithPrefix = "resources/" + relativeFilePath;
        // const char* codepointText = "🡇🡅➕⮌";
        int codepointCount = 0;
        int* codepoints = LoadCodepoints(codepointText, &codepointCount);
        m_fonts.emplace(fontKey, LoadFontEx(pathWithPrefix.c_str(), fontSize, codepoints, codepointCount));
    }
};
