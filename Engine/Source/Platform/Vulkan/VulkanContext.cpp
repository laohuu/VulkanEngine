#include "VulkanContext.h"

#include "Vulkan.h"

#include <imgui.h>

namespace Engine
{
    static bool s_Validation = false;

    static bool CheckDriverAPIVersionSupport(uint32_t minimumSupportedVersion)
    {
        uint32_t instanceVersion;
        vkEnumerateInstanceVersion(&instanceVersion);

        if (instanceVersion < minimumSupportedVersion)
        {
            //            ENGINE_CORE_FATAL("Incompatible Vulkan driver version!");
            //            ENGINE_CORE_FATAL("  You have {}.{}.{}", VK_API_VERSION_MAJOR(instanceVersion),
            //            VK_API_VERSION_MINOR(instanceVersion), VK_API_VERSION_PATCH(instanceVersion));
            //            ENGINE_CORE_FATAL("  You need at least {}.{}.{}",
            //            VK_API_VERSION_MAJOR(minimumSupportedVersion), VK_API_VERSION_MINOR(minimumSupportedVersion),
            //            VK_API_VERSION_PATCH(minimumSupportedVersion));

            return false;
        }

        return true;
    }

    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
    {
        for (const VkExtensionProperties& p : properties)
            if (strcmp(p.extensionName, extension) == 0)
                return true;
        return false;
    }

    VulkanContext::VulkanContext() = default;
    VulkanContext::~VulkanContext()
    {
        // Its too late to destroy the device here, because Destroy() asks for the context (which we're in the middle of
        // destructing) Device is destroyed in WindowsWindow::Shutdown()
        // m_Device->Destroy();

        vkDestroyInstance(s_VulkanInstance, nullptr);
        s_VulkanInstance = nullptr;
    }

    void VulkanContext::Init()
    {

        ImVector<const char*> instance_extensions;
        uint32_t              extensions_count = 0;
        const char**          glfw_extensions  = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++)
            instance_extensions.push_back(glfw_extensions[i]);

        // Create Vulkan Instance
        {
            VkInstanceCreateInfo create_info = {};
            create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            // Enumerate available extensions
            uint32_t                        properties_count;
            ImVector<VkExtensionProperties> properties;
            vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
            properties.resize(properties_count);
            VK_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data));

            // Enable required extensions
            if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
                instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
            {
                instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
#endif

            // Enabling validation layers
#ifdef IMGUI_VULKAN_DEBUG_REPORT
            const char* layers[]            = {"VK_LAYER_KHRONOS_validation"};
            create_info.enabledLayerCount   = 1;
            create_info.ppEnabledLayerNames = layers;
            instance_extensions.push_back("VK_EXT_debug_report");
#endif

            // Create Vulkan Instance
            create_info.enabledExtensionCount   = (uint32_t)instance_extensions.Size;
            create_info.ppEnabledExtensionNames = instance_extensions.Data;
            VK_CHECK_RESULT(vkCreateInstance(&create_info, s_Allocator, &s_VulkanInstance));

            // Setup the debug report callback
#ifdef IMGUI_VULKAN_DEBUG_REPORT
            auto vkCreateDebugReportCallbackEXT =
                (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
            IM_ASSERT(vkCreateDebugReportCallbackEXT != nullptr);
            VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
            debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            debug_report_ci.pfnCallback = debug_report;
            debug_report_ci.pUserData   = nullptr;
            err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
            check_vk_result(err);
#endif
        }

        // Select Physical Device (GPU)
        m_PhysicalDevice = VulkanPhysicalDevice::Select();

        VkPhysicalDeviceFeatures enabledFeatures;
        memset(&enabledFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
        enabledFeatures.samplerAnisotropy       = true;
        enabledFeatures.wideLines               = true;
        enabledFeatures.fillModeNonSolid        = true;
        enabledFeatures.independentBlend        = true;
        enabledFeatures.pipelineStatisticsQuery = true;
        m_Device                                = Ref<VulkanDevice>::Create(m_PhysicalDevice, enabledFeatures);
    }
} // namespace Engine