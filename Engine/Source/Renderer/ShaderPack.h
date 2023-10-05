#ifndef ENGINE_SHADERPACK_H
#define ENGINE_SHADERPACK_H

#include "Shader.h"

#include "Serialization/ShaderPackFile.h"

namespace Engine
{
    class ShaderPack : public RefCounted
    {
    public:
        ShaderPack() = default;
        ShaderPack(const std::filesystem::path& path);

        bool IsLoaded() const { return m_Loaded; }
        bool Contains(std::string_view name) const;

        Ref<Shader> LoadShader(std::string_view name);

        static Ref<ShaderPack> CreateFromLibrary(Ref<ShaderLibrary> shaderLibrary, const std::filesystem::path& path);

    private:
        bool                  m_Loaded = false;
        ShaderPackFile        m_File;
        std::filesystem::path m_Path;
    };
} // namespace Engine

#endif // ENGINE_SHADERPACK_H
