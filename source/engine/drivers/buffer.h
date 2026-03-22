#pragma once

#include "../common.h"
#include "../vulkan_context.h"
#include "commands.h"
#include <print>
#include <cstring>

namespace Engine {
  class Commands;

struct Vertex {
    vec3 pos;
    vec3 color;
    vec2 uv;

    static VkVertexInputBindingDescription                  getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

u32 findMemoryType(VkPhysicalDevice physical, u32 typeFilter,
                   VkMemoryPropertyFlags properties);

class Buffer {
public:
    void create(VkPhysicalDevice physical, VkDevice device,
                VkDeviceSize size, VkBufferUsageFlags usage,
                VkMemoryPropertyFlags properties);
    void destroy();
    ~Buffer() { destroy(); }

    VkBuffer       handle() const { return m_handle; }
    VkDeviceMemory memory() const { return m_memory; }
    VkDeviceSize   size()   const { return m_size; }

private:
    VkDevice       m_device = VK_NULL_HANDLE;
    VkBuffer       m_handle = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkDeviceSize   m_size   = 0;
};

class Mesh {
public:
    void create(Vulkan_Context &ctx, Commands &commands, 
        const std::vector<Vertex>& vertices,
        const std::vector<u32>& indices);
    void destroy();
    ~Mesh() { destroy(); }

    const Buffer& vertex_buffer() const { return m_vertex_buffer; }
    const Buffer& index_buffer()  const { return m_index_buffer; }
    u32           index_count()   const { return m_index_count; }
    u32           vertex_count()  const { return m_vertex_count; }

private:
    VkPhysicalDevice m_physical = VK_NULL_HANDLE;
    VkDevice         m_device   = VK_NULL_HANDLE;

    Buffer m_vertex_buffer;
    Buffer m_index_buffer;
    u32    m_index_count  = 0;
    u32    m_vertex_count = 0;

    void copy(VkDevice device, VkCommandPool pool, VkQueue queue,
              VkBuffer src, VkBuffer dst, VkDeviceSize size);
};

} // namespace Engine
