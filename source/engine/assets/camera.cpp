#include "camera.h"
#include <cstdio>
#include <stdexcept>
#include <cstring>

static constexpr u32 N = Engine::Sync::MAX_FRAMES_IN_FLIGHT;

void Camera::create(Engine::Vulkan_Context& ctx)
{
  // getting refrances
    m_device   = ctx.get_device();
    m_physical = ctx.get_physical_device();

    // Define the Discriptor and makeing vulkam use it
    VkDescriptorSetLayoutBinding binding{};
    binding.binding         = 0;
    binding.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.descriptorCount = 1;
    binding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings    = &binding;

    if (vkCreateDescriptorSetLayout(m_device, &layout_info, nullptr, &m_set_layout) != VK_SUCCESS)
        throw std::runtime_error("camera: failed to create descriptor set layout");

    // discriptor pool setup
    VkDescriptorPoolSize pool_size{};
    pool_size.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = N;

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.maxSets       = N;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes    = &pool_size;

    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_pool) != VK_SUCCESS)
        throw std::runtime_error("camera: failed to create descriptor pool");

    // allocate descriptor
    std::array<VkDescriptorSetLayout, N> layouts;
    layouts.fill(m_set_layout);

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool     = m_pool;
    alloc_info.descriptorSetCount = N;
    alloc_info.pSetLayouts        = layouts.data();

    if (vkAllocateDescriptorSets(m_device, &alloc_info, m_sets.data()) != VK_SUCCESS)
        throw std::runtime_error("camera: failed to allocate descriptor sets");

    // buffers, map, writting descriptors
    for (u32 i = 0; i < N; i++)
    {
        m_buffers[i].create(m_physical, m_device,
            sizeof(UBO),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        vkMapMemory(m_device, m_buffers[i].memory(), 0, sizeof(UBO), 0, &m_mapped[i]);

        VkDescriptorBufferInfo buf_info{};
        buf_info.buffer = m_buffers[i].handle();
        buf_info.offset = 0;
        buf_info.range  = sizeof(UBO);

        VkWriteDescriptorSet write{};
        write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet          = m_sets[i];
        write.dstBinding      = 0;
        write.descriptorCount = 1;
        write.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo     = &buf_info;

        vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
        std::printf("[camera] created\n");
    }
}

void Camera::update(u32 frame, f32 aspect)
{
    m_update_vectors();
    UBO ubo = get_UBO(aspect);
    memcpy(m_mapped[frame], &ubo, sizeof(UBO));
}

void Camera::destroy()
{
    if (m_device == VK_NULL_HANDLE) return;

    for (u32 i = 0; i < N; i++)
        m_buffers[i].destroy();

    vkDestroyDescriptorPool(m_device, m_pool, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_set_layout, nullptr);

    m_pool       = VK_NULL_HANDLE;
    m_set_layout = VK_NULL_HANDLE;
    m_device     = VK_NULL_HANDLE;
}

UBO Camera::get_UBO(f32 aspect) const
{
    UBO out{};
    // setting projection matrix
    out.projection_matrix        = glm::perspective(glm::radians(fov), aspect, near, far);
    // flip the y axis
    out.projection_matrix[1][1] *= -1.f;
    out.view_matrix              = glm::lookAt(location, location + forward, up);
    out.pos                      = location;
    return out;
}

void Camera::m_update_vectors()
{
    forward = rotation * vec3(0.f, 0.f, -1.f);
    up      = rotation * vec3(0.f, 1.f,  0.f);
    right   = rotation * vec3(1.f, 0.f,  0.f);
}
