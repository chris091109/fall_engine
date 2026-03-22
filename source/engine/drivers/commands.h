#pragma once
#include "../common.h"
#include "buffer.h"
#include <vector>
#include "../vulkan_context.h"

namespace Engine {

  class Mesh;

  class Commands {
    public:
      void create(Vulkan_Context &ctx);
      void destroy();
      ~Commands() { destroy(); }

      void record(u32 frame, VkRenderPass render_pass, VkFramebuffer framebuffer,
          VkExtent2D extent, VkPipeline pipeline, VkPipelineLayout pipeline_layout,
          VkDescriptorSet cam_set, const Mesh& mesh);
      void reset(u32 frame);

      VkCommandBuffer buffer(u32 frame) const { return m_command_buffers[frame]; }
      VkCommandPool   pool()            const { return m_command_pool; }

    static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2;

private:
    VkDevice      m_device       = VK_NULL_HANDLE;
    u32 m_queue_family_index       = 0;
    VkCommandPool m_command_pool = VK_NULL_HANDLE;

    std::vector<VkCommandBuffer> m_command_buffers;
};

} // namespace Engine
