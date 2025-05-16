#pragma once
#include <chrono>
#include "System/System.h"
#include "System/CameraController.h"
#include "Core/Graphics.h"
#include "ECSystem/Renderer.h"
#include "ECSystem/BehaviourSystem.h"
#include "Core/Grid.h"
#include "Core/SkyBox.h"

class Scene
{
public:
	Scene(Graphics& gfx, System& system);
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene() = default;

	void Init();
	void Update();

private:
	Graphics& m_graphics;
	System& m_system;

	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<CBufferSystem> m_cbufferSystem;
	std::unique_ptr<MeshSystem> m_meshSystem;
	std::unique_ptr<CameraSystem> m_cameraSystem;
	std::unique_ptr<TransformSystem> m_transformSystem;
	std::unique_ptr<CameraController> m_cameraController;
	std::unique_ptr<BehaviourSystem> m_behaviourSystem;
	std::unique_ptr<Skybox> m_skybox;

	std::unique_ptr<Grid> m_grid;

	entt::registry m_registry;

	entt::entity m_camera;
	entt::entity m_gridEntity;
	entt::entity m_skyboxEntity;
};