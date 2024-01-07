#pragma once

namespace XHuang
{

struct GraphicsAPI { };

struct Vulkan : public GraphicsAPI { };

template <typename GraphicsAPI_Type> struct GraphicsContext {};
template <typename GraphicsAPI_Type> struct GraphicsCommandBuffer {};

template <typename GraphicsAPI_Type> struct GraphicsRenderPass {};

} // namespace XHuang