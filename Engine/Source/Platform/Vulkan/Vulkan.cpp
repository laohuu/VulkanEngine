#include "Vulkan.h"

namespace Engine::Utils
{
    void VulkanLoadDebugUtilsExtensions(VkInstance instance)
    {
        fpSetDebugUtilsObjectNameEXT =
            (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
        if (fpSetDebugUtilsObjectNameEXT == nullptr)
            fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
                return VK_SUCCESS;
            };

        fpCmdBeginDebugUtilsLabelEXT =
            (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
        if (fpCmdBeginDebugUtilsLabelEXT == nullptr)
            fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {};

        fpCmdEndDebugUtilsLabelEXT =
            (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
        if (fpCmdEndDebugUtilsLabelEXT == nullptr)
            fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer) {};

        fpCmdInsertDebugUtilsLabelEXT =
            (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
        if (fpCmdInsertDebugUtilsLabelEXT == nullptr)
            fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
            };
    }

    static const char* StageToString(VkPipelineStageFlagBits stage)
    {
        switch (stage)
        {
            case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT:
                return "VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT";
            case VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT:
                return "VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT";
            default:
                break;
        }
        //        ENGINE_CORE_ASSERT(false);
        return nullptr;
    }

    void RetrieveDiagnosticCheckpoints() {}

} // namespace Engine::Utils