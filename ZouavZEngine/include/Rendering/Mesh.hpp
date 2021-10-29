#pragma once

#include "Resource.hpp"
#include <vector>
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Mat4.hpp"
#include <map>
#include <string>
#include <assimp/scene.h>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct BoneInfo
{
    /*offset matrix transforms vertex from model space to bone space*/
    Mat4 offset;

    /*id is index in finalBoneMatrices*/
    int id;

    bool isValid = true;
};

class Mesh : public Resource
{
private:
    unsigned int VBO, EBO = 0;
    unsigned int VAO = 0;
    size_t nbElements = 0;

public:
    int boneCounter = 0;
    std::map<std::string, BoneInfo> boneInfoMap;
    Mesh(const std::string& _name);
    Mesh(const std::string& _name, const char* path);
    ~Mesh();

    void RemoveFromResourcesManager() override;

    void InitMesh(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize);
    void ChangeSizeAndData(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize);
    void CreateCube();

    void SetVertexBoneDataToDefault(Vertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.boneIDs[i] = -1;
            vertex.weights[i] = 0.0f;
        }
    }

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
  
    unsigned int GetID() const
    {
        return VAO;
    }

    size_t GetNbElements() const
    {
        return nbElements;
    }
};