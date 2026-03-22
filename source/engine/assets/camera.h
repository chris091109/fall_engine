#pragma once

#include "../common.h"
#include "../vulkan_context.h"
#include "../drivers/buffer.h"
#include "../drivers/sync.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <array>

struct UBO
{
    mat4 projection_matrix;
    mat4 view_matrix;
    vec3 pos;
    f32  _pad;
};

class Camera
{
    static constexpr u32 N = Engine::Sync::MAX_FRAMES_IN_FLIGHT;

  public:
    vec3 location = {0.f, 0.f, 1.f};
    quat rotation = quat{1.0f, 0.0f, 0.0f, 0.0f};

    vec3 forward = {0.f, 0.f, -1.f};
    vec3 up      = {0.f, 1.f,  0.f};
    vec3 right   = {1.f, 0.f,  0.f};

    f32 fov  = 90.f;
    f32 near = 0.1f;
    f32 far  = 1000.f;

    void create(Engine::Vulkan_Context& ctx);
    void update(u32 frame, f32 aspect);
    void destroy();
    ~Camera() { destroy(); }

    VkDescriptorSetLayout set_layout()   const { return m_set_layout; }
    VkDescriptorSet       set(u32 frame) const { return m_sets[frame]; }

  private:
    VkDevice              m_device     = VK_NULL_HANDLE;
    VkPhysicalDevice      m_physical   = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_set_layout = VK_NULL_HANDLE;
    VkDescriptorPool      m_pool       = VK_NULL_HANDLE;

    std::array<Engine::Buffer, N> m_buffers;
    std::array<void*,          N> m_mapped = {};
    std::array<VkDescriptorSet,N> m_sets   = {};

    void m_update_vectors();
    UBO  get_UBO(f32 aspect) const;
};
