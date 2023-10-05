#ifndef ENGINE_SHADERPREPROCESSOR_H
#define ENGINE_SHADERPREPROCESSOR_H

#include "Renderer/Shader.h"

#include <vulkan/vulkan.h>

namespace Engine
{
    struct IncludeData
    {
        std::filesystem::path IncludedFilePath {};
        size_t                IncludeDepth {};
        bool                  IsRelative {false};
        bool                  IsGuarded {false};
        uint32_t              HashValue {};

        VkShaderStageFlagBits IncludedStage {};

        inline bool operator==(const IncludeData& other) const noexcept
        {
            return this->IncludedFilePath == other.IncludedFilePath && this->HashValue == other.HashValue;
        }
    };
} // namespace Engine

namespace std
{
    template<>
    struct hash<Engine::IncludeData>
    {
        size_t operator()(const Engine::IncludeData& data) const noexcept
        {
            return std::filesystem::hash_value(data.IncludedFilePath) ^ data.HashValue;
        }
    };
} // namespace std

namespace Engine
{
    class ShaderPreprocessor
    {};
} // namespace Engine

#endif // ENGINE_SHADERPREPROCESSOR_H
