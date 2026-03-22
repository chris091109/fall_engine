#pragma once

#include <cstdint>
#include <cstddef>

// Unsigned integers
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Signed integers
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Floating point
using f32 = float;
using f64 = double;

using usize = size_t;

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using mat4 = glm::mat4;
using quat = glm::quat;
