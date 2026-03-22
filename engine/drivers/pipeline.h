#pragma once
#include "../common.h"
#include <fstream>
#include <print>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "buffer.h"
#include "../vulkan_context.h"
#include "swapchain.h"

namespace Engine {

  class Pipeline {
    public:
      void create(Vulkan_Context &ctx, Swapchain &swapchain, VkRenderPass render_pass,
                  VkDescriptorSetLayout cam_set_layout);
      void destroy();
      ~Pipeline() { destroy(); }

      VkPipeline       handle()          const { return m_pipeline; }
      VkPipelineLayout layout()          const { return m_pipeline_layout; }

    private:
      Vulkan_Context   *m_ctx = nullptr;
      Swapchain *m_swapchain = nullptr;
      VkDevice m_device = VK_NULL_HANDLE;
      VkPipeline       m_pipeline        = VK_NULL_HANDLE;
      VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;

      VkShaderModule createShaderModule(const std::vector<char>& code);
      static std::vector<char> readFile(const std::string& path);
  };
} 
