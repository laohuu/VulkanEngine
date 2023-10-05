#ifndef ENGINE_VULKANSHADER_H
#define ENGINE_VULKANSHADER_H

#include "Renderer/Shader.h"
#include "ShaderCompiler/ShaderPreprocessor.h"

#include <vulkan/vulkan.h>

namespace Engine
{
    struct StageData
    {
        std::unordered_set<IncludeData> Headers;
        uint32_t                        HashValue = 0;
        bool                            operator==(const StageData& other) const noexcept
        {
            return this->Headers == other.Headers && this->HashValue == other.HashValue;
        }
        bool operator!=(const StageData& other) const noexcept { return !(*this == other); }
    };

    class VulkanShader : public Shader
    {
    public:
        struct ReflectionData
        {
            std::unordered_map<std::string, ShaderResourceDeclaration> Resources;
            std::unordered_map<std::string, ShaderBuffer>              ConstantBuffers;
        };

    public:
        VulkanShader() = default;
        VulkanShader(const std::string& path, bool forceCompile, bool disableOptimization);
        virtual ~VulkanShader();
        void Release();

        void Reload(bool forceCompile = false) override;
        void RT_Reload(bool forceCompile) override;

        virtual size_t GetHash() const override;
        void           SetMacro(const std::string& name, const std::string& value) override {}

        virtual const std::string&                                   GetName() const override { return m_Name; }
        virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const override
        {
            return m_ReflectionData.ConstantBuffers;
        }
        virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const override;
        virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;

        bool TryReadReflectionData(StreamReader* serializer);

        void SerializeReflectionData(StreamWriter* serializer);

        void SetReflectionData(const ReflectionData& reflectionData);

        // Vulkan-specific
        const std::vector<VkPipelineShaderStageCreateInfo>& GetPipelineShaderStageCreateInfos() const
        {
            return m_PipelineShaderStageCreateInfos;
        }

        VkDescriptorSet       GetDescriptorSet() { return m_DescriptorSet; }
        VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) { return m_DescriptorSetLayouts.at(set); }
        std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();

        struct ShaderMaterialDescriptorSet
        {
            VkDescriptorPool             Pool = nullptr;
            std::vector<VkDescriptorSet> DescriptorSets;
        };

        ShaderMaterialDescriptorSet AllocateDescriptorSet(uint32_t set = 0);
        ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set = 0);
        ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set, uint32_t numberOfSets);
        const VkWriteDescriptorSet* GetDescriptorSet(const std::string& name, uint32_t set = 0) const;

    private:
        void Load(const std::string& source, bool forceCompile);

        std::map<VkShaderStageFlagBits, std::string> PreProcess(const std::string& source);
        std::map<VkShaderStageFlagBits, std::string> PreProcessGLSL(const std::string& source);
        std::map<VkShaderStageFlagBits, std::string> PreProcessHLSL(const std::string& source);

        void LoadAndCreateShaders(const std::map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData);
        void CreateDescriptors();

    private:
        std::vector<VkPipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;

        std::filesystem::path m_AssetPath;
        std::string           m_Name;
        bool                  m_DisableOptimization = false;

        std::map<VkShaderStageFlagBits, std::string>           m_ShaderSource;
        std::map<VkShaderStageFlagBits, std::vector<uint32_t>> m_SPIRVDebugData, m_SPIRVData;

        ShaderUtils::SourceLang m_Language;

        std::map<VkShaderStageFlagBits, StageData> m_StagesMetadata;

        std::map<VkShaderStageFlagBits, std::vector<uint32_t>> m_ShaderData;
        ReflectionData                                         m_ReflectionData;

        std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
        VkDescriptorSet                    m_DescriptorSet;
        // VkDescriptorPool m_DescriptorPool = nullptr;

        std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> m_TypeCounts;

    private:
        friend class ShaderCache;
        friend class ShaderPack;
        friend class VulkanShaderCompiler;
    };
} // namespace Engine

#endif // ENGINE_VULKANSHADER_H
