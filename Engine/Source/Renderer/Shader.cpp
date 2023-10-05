#include "Shader.h"

#include "Platform/Vulkan/VulkanShader.h"

#include "Renderer/ShaderPack.h"

namespace Engine
{
    Ref<Shader> Shader::Create(const std::string& filepath, bool forceCompile, bool disableOptimization)
    {
        Ref<Shader> result = Ref<VulkanShader>::Create(filepath, forceCompile, disableOptimization);
        return result;
    }

    Ref<Shader> Shader::CreateFromString(const std::string& source)
    {
        Ref<Shader> result = nullptr;
        return result;
    }

    ShaderLibrary::ShaderLibrary() = default;

    ShaderLibrary::~ShaderLibrary() = default;

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name      = shader->GetName();
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Load(std::string_view path, bool forceCompile, bool disableOptimization)
    {
        Ref<Shader> shader;
        if (!forceCompile && m_ShaderPack)
        {
            if (m_ShaderPack->Contains(path))
                shader = m_ShaderPack->LoadShader(path);
        }
        else
        {
            // Try compile from source
            // Unavailable at runtime
        }

        auto& name      = shader->GetName();
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Load(std::string_view name, const std::string& path)
    {
        m_Shaders[std::string(name)] = Shader::Create(path);
    }

    void ShaderLibrary::LoadShaderPack(const std::filesystem::path& path)
    {
        m_ShaderPack = Ref<ShaderPack>::Create(path);
        if (!m_ShaderPack->IsLoaded())
        {
            m_ShaderPack = nullptr;
        }
    }

    const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const { return m_Shaders.at(name); }

    ShaderUniform::ShaderUniform(std::string             name,
                                 const ShaderUniformType type,
                                 const uint32_t          size,
                                 const uint32_t          offset) :
        m_Name(std::move(name)),
        m_Type(type), m_Size(size), m_Offset(offset)
    {}

    constexpr std::string_view ShaderUniform::UniformTypeToString(const ShaderUniformType type)
    {
        if (type == ShaderUniformType::Bool)
        {
            return std::string_view("Boolean");
        }
        else if (type == ShaderUniformType::Int)
        {
            return std::string_view("Int");
        }
        else if (type == ShaderUniformType::Float)
        {
            return std::string_view("Float");
        }

        return std::string_view("None");
    }
} // namespace Engine