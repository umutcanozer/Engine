#pragma once
#include <vector>
#include "Core/Graphics.h"
#include "entt.hpp"

struct GridVertex
{
	DirectX::XMFLOAT3 position;
};

struct GridComponent {
	std::vector<GridVertex> vertices;
	std::vector<unsigned short> indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};

class Grid
{
public:
	Grid(Graphics& gfx, entt::registry& registry);
	Grid(const Grid&) = delete;
	Grid& operator=(const Grid&) = delete;
	~Grid() = default;

	void Init();

private:
	void CreateGridBuffers(GridComponent& gridComponent);

private:
	Graphics& m_gfx;
	entt::registry& m_registry;
};

