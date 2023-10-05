#include "VulkanShader.h"

#include <spirv-tools/libspirv.h>
#include <spirv_cross/spirv_glsl.hpp>

#include <shaderc/shaderc.hpp>

namespace Engine
{
    namespace Utils
    {
        static const char* GetCacheDirectory() { return "Resources/Cache/Shader/Vulkan"; }

        static void CreateCacheDirectoryIfNeeded()
        {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }

        static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
        {
            switch (type.basetype)
            {
                case spirv_cross::SPIRType::Boolean:
                    return ShaderUniformType::Bool;
                case spirv_cross::SPIRType::Int:
                    if (type.vecsize == 1)
                        return ShaderUniformType::Int;
                    if (type.vecsize == 2)
                        return ShaderUniformType::IVec2;
                    if (type.vecsize == 3)
                        return ShaderUniformType::IVec3;
                    if (type.vecsize == 4)
                        return ShaderUniformType::IVec4;

                case spirv_cross::SPIRType::UInt:
                    return ShaderUniformType::UInt;
                case spirv_cross::SPIRType::Float:
                    if (type.columns == 3)
                        return ShaderUniformType::Mat3;
                    if (type.columns == 4)
                        return ShaderUniformType::Mat4;

                    if (type.vecsize == 1)
                        return ShaderUniformType::Float;
                    if (type.vecsize == 2)
                        return ShaderUniformType::Vec2;
                    if (type.vecsize == 3)
                        return ShaderUniformType::Vec3;
                    if (type.vecsize == 4)
                        return ShaderUniformType::Vec4;
                default:
                    break;
            }
            return ShaderUniformType::None;
        }

        int SkipBOM(std::istream& in)
        {
            char test[4] = {0};
            in.seekg(0, std::ios::beg);
            in.read(test, 3);
            if (strcmp(test, "\xEF\xBB\xBF") == 0)
            {
                in.seekg(3, std::ios::beg);
                return 3;
            }
            in.seekg(0, std::ios::beg);
            return 0;
        }

        // Returns an empty string when failing.
        std::string ReadFileAndSkipBOM(const std::filesystem::path& filepath)
        {
            std::string   result;
            std::ifstream in(filepath, std::ios::in | std::ios::binary);
            if (in)
            {
                in.seekg(0, std::ios::end);
                auto      fileSize     = in.tellg();
                const int skippedChars = SkipBOM(in);

                fileSize -= skippedChars - 1;
                result.resize(fileSize);
                in.read(result.data() + 1, fileSize);
                // Add a dummy tab to beginning of file.
                result[0] = '\t';
            }
            in.close();
            return result;
        }
    } // namespace Utils

    namespace ShaderUtils
    {
        static SourceLang ShaderLangFromExtension(const std::string_view type)
        {
            if (type == ".glsl")
                return SourceLang::GLSL;
            if (type == ".hlsl")
                return SourceLang::HLSL;

            return SourceLang::NONE;
        }
    } // namespace ShaderUtils

    VulkanShader::VulkanShader(const std::string& path, bool forceCompile, bool disableOptimization) :
        m_AssetPath(path), m_DisableOptimization(disableOptimization)
    {
        // TODO: This should be more "general"
        size_t found = path.find_last_of("/\\");
        m_Name       = found != std::string::npos ? path.substr(found + 1) : path;
        found        = m_Name.find_last_of('.');
        m_Name       = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

        Reload(forceCompile);
    }

    VulkanShader::~VulkanShader() {}

    void VulkanShader::Release() {}

    void VulkanShader::RT_Reload(bool forceCompile)
    {
        m_Language = ShaderUtils::ShaderLangFromExtension(m_AssetPath.extension().string());

        m_ShaderSource.clear();
        m_StagesMetadata.clear();
        m_SPIRVDebugData.clear();
        m_SPIRVData.clear();

        Utils::CreateCacheDirectoryIfNeeded();
        const std::string source = Utils::ReadFileAndSkipBOM(m_AssetPath);

        m_ShaderSource = PreProcess(source);
    }

    void VulkanShader::Reload(bool forceCompile) { RT_Reload(forceCompile); }

    void VulkanShader::Load(const std::string& source, bool forceCompile)
    {
        m_ShaderSource = PreProcess(source);
        Utils::CreateCacheDirectoryIfNeeded();

        std::array<std::vector<uint32_t>, 2>                vulkanBinaries;
        std::unordered_map<uint32_t, std::vector<uint32_t>> shaderData;
        //        CompileOrGetVulkanBinary(shaderData, forceCompile);
    }

    std::map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcess(const std::string& source)
    {
        switch (m_Language)
        {
            case ShaderUtils::SourceLang::GLSL:
                return PreProcessGLSL(source);
            case ShaderUtils::SourceLang::HLSL:
                return PreProcessHLSL(source);
            default:
                break;
        }
        return {};
    }

    std::map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcessGLSL(const std::string& source) { return {}; }

    std::map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcessHLSL(const std::string& source) { return {}; }

    size_t VulkanShader::GetHash() const { return hash_value(m_AssetPath); }

    void VulkanShader::LoadAndCreateShaders(const std::map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData)
    {
        m_ShaderData = shaderData;

        m_PipelineShaderStageCreateInfos.clear();
        std::string moduleName;
        for (auto [stage, data] : shaderData)
        {
            VkShaderModuleCreateInfo moduleCreateInfo {};

            moduleCreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            moduleCreateInfo.codeSize = data.size() * sizeof(uint32_t);
            moduleCreateInfo.pCode    = data.data();

            VkShaderModule shaderModule;
            //            VKUtils::SetDebugUtilsObjectName(device,
            //                                             VK_OBJECT_TYPE_SHADER_MODULE,
            //                                             fmt::format("{}:{}", m_Name,
            //                                             ShaderUtils::ShaderStageToString(stage)), shaderModule);

            VkPipelineShaderStageCreateInfo& shaderStage = m_PipelineShaderStageCreateInfos.emplace_back();
            shaderStage.sType                            = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStage.stage                            = stage;
            shaderStage.module                           = shaderModule;
            shaderStage.pName                            = "main";
        }
    }

    void VulkanShader::CreateDescriptors() {}

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32_t set)
    {
        ShaderMaterialDescriptorSet result;

        // TODO: remove
        result.Pool = nullptr;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount          = 1;
        allocInfo.pSetLayouts                 = &m_DescriptorSetLayouts[set];
        return result;
    }

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set)
    {
        ShaderMaterialDescriptorSet result;

        // TODO: Move this to the centralized renderer
        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext                      = nullptr;
        descriptorPoolInfo.poolSizeCount              = (uint32_t)m_TypeCounts.at(set).size();
        descriptorPoolInfo.pPoolSizes                 = m_TypeCounts.at(set).data();
        descriptorPoolInfo.maxSets                    = 1;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool              = result.Pool;
        allocInfo.descriptorSetCount          = 1;
        allocInfo.pSetLayouts                 = &m_DescriptorSetLayouts[set];

        result.DescriptorSets.emplace_back();
        return result;
    }

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set, uint32_t numberOfSets)
    {
        ShaderMaterialDescriptorSet result;

        return result;
    }

    const VkWriteDescriptorSet* VulkanShader::GetDescriptorSet(const std::string& name, uint32_t set) const
    {
        return nullptr;
    }

    std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
    {
        std::vector<VkDescriptorSetLayout> result;
        result.reserve(m_DescriptorSetLayouts.size());
        for (auto& layout : m_DescriptorSetLayouts)
            result.emplace_back(layout);

        return result;
    }

    const std::unordered_map<std::string, ShaderResourceDeclaration>& VulkanShader::GetResources() const
    {
        return m_ReflectionData.Resources;
    }

    void VulkanShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback) {}

    bool VulkanShader::TryReadReflectionData(StreamReader* serializer)
    {
        uint32_t shaderDescriptorSetCount;
        serializer->ReadRaw<uint32_t>(shaderDescriptorSetCount);

        return true;
    }

    void VulkanShader::SerializeReflectionData(StreamWriter* serializer) {}

    void VulkanShader::SetReflectionData(const ReflectionData& reflectionData) { m_ReflectionData = reflectionData; }
} // namespace Engine