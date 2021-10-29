#include <vector>
#include <iostream>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "System/Debug.hpp"

#include "Maths/Vec2.hpp"
#include "Maths/Vec3.hpp"

#include "System/ResourcesManager.hpp"

#include "Rendering/Mesh.hpp"

Mesh::Mesh(const std::string& _name)
	: Resource(_name, "NoPath")
{
}

Mesh::Mesh(const std::string& _name, const char* _path)
	: Resource(_name, _path)
{
    Assimp::Importer importer;
		
	const aiScene* scene = importer.ReadFile(_path,	aiProcess_Triangulate | 
													aiProcess_FindInvalidData | 
													aiProcess_ValidateDataStructure );
	if (!scene)
	{
        Debug::LogWarning(std::string("Mesh load failed!: ").append(_path));
		return;
	}

    std::vector<Vertex> vertices;
	std::vector<int> indices;

	const aiMesh* mesh = scene->mMeshes[0];
	int test = scene->mNumMeshes;
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		Vertex vert;

		SetVertexBoneDataToDefault(vert);
		vert.pos = Vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
		vert.normal = mesh->HasNormals() ? Vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z) : Vec3();
		vert.texCoord = mesh->HasTextureCoords(0) ? Vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y) : Vec2();

		vertices.push_back(vert);
	}

	ExtractBoneWeightForVertices(vertices, mesh, scene);

 	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void Mesh::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCounter;
			newBoneInfo.offset = Mat4::ConvertAssimpMatrixToMat4(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCounter;
			boneCounter++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void Mesh::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < 4; i++)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.boneIDs[i] = boneID;
			vertex.weights[i] = weight;
			break;
		}
	}
}

void Mesh::InitMesh(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize)
{
	nbElements = indicesSize;

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	
	//For animation
	// ids
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (const GLvoid*)(8 * sizeof(int)));
	glEnableVertexAttribArray(3);

	// weights
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(const GLvoid*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
}

void Mesh::ChangeSizeAndData(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize)
{
	nbElements = indicesSize;
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::RemoveFromResourcesManager()
{
	ResourcesManager::RemoveResourceMesh(name);
}

void Mesh::CreateCube()
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Vertex vert;

	vert = {Vec3(-0.5f, -0.5f, -0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 0.f)};
	vertices.push_back(vert);

	vert = {Vec3(0.5f, -0.5f, -0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 0.f)};
	vertices.push_back(vert);

	vert = {Vec3(0.5f, 0.5f, -0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 1.f)};
	vertices.push_back(vert);

	vert = {Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 1.f)};
	vertices.push_back(vert);
	
	vert = { Vec3(-0.5f, -0.5f, 0.5f),
		Vec3(0.f, 0.f, 1.f),
		Vec2(0.f, 0.f) };
	vertices.push_back(vert);

	vert = { Vec3(0.5f, -0.5f, 0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 0.f) };
	vertices.push_back(vert);

	vert = { Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 1.f) };
	vertices.push_back(vert);

	vert = { Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 1.f) };
	vertices.push_back(vert);

	// forward
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	// backward
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);
	// up
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(6);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(6);
	// down
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);
	// left
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(2);
	// right
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(3);

	InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}
