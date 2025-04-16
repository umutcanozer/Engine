#include "MeshManager.h"
using namespace DirectX;
std::shared_ptr<MeshAsset> MeshManager::LoadModel(const std::string& path, Graphics& gfx)
{
	std::ifstream test(path);
	if (!test.is_open()) {
		std::cerr << "C++ da okuyamýyor bu dosyayý: " << path << std::endl;
	}
	else {
		std::cerr << "C++ dosyayý açabiliyor." << std::endl;
	}
    if (m_loadedMeshes.contains(path)) {
        return m_loadedMeshes[path];
    }

	/*
	* scene->mNumMeshes = number of meshes in the file
	* scene->mMeshes = array of meshes
	* scene->mMaterials = array of materials
	* scene->mRootNode = root node of the scene graph
	*/

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_ImproveCacheLocality
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
	}

	aiMesh* mesh = scene->mMeshes[0];
	std::shared_ptr<MeshAsset> meshAsset = std::make_shared<MeshAsset>();
	meshAsset->vertices.reserve(mesh->mNumVertices);
	meshAsset->indices.reserve(mesh->mNumFaces * 3);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		if (mesh->HasVertexColors(0)) {
			vertex.color.x = mesh->mColors[0][i].r;
			vertex.color.y = mesh->mColors[0][i].g;
			vertex.color.z = mesh->mColors[0][i].b;
			vertex.color.w = mesh->mColors[0][i].a;
		}
		else {
			vertex.color = { 1.f, 1.f, 1.f, 1.f };
		}

		meshAsset->vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			meshAsset->indices.push_back(face.mIndices[j]);
		}
	}



	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.ByteWidth = (UINT)(sizeof(Vertex) * meshAsset->vertices.size());
	vbDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = meshAsset->vertices.data();

	gfx.GetDevice()->CreateBuffer(&vbDesc, &vbData, &meshAsset->vertexBuffer);

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.ByteWidth = (UINT)(sizeof(unsigned short) * meshAsset->indices.size());
	ibDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = meshAsset->indices.data();

	gfx.GetDevice()->CreateBuffer(&ibDesc, &ibData, &meshAsset->indexBuffer);

	m_loadedMeshes[path] = meshAsset;
    return meshAsset;
}
