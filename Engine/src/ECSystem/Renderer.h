#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"
#include "MeshSystem.h"
#include "CBufferSystem.h"



class Renderer {
public:
    Renderer(Graphics& gfx, entt::registry& registry);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;

    void Update();

private:
    Graphics& m_gfx;
    entt::registry& m_registry;
    UINT m_Stride;
    UINT m_Offset;
};
