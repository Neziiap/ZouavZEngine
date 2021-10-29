#pragma once

#include "Resource.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Bone.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include <assimp/scene.h>
#include "Rendering/Shader.hpp"
#include "Rendering/Camera.hpp"
#include <memory>

struct AssimpNodeData
{
    Bone* bone;
    std::vector<AssimpNodeData> children;
    BoneInfo boneInfo;
    Mat4 transformation;
    std::string name;
    int childrenCount;
};

class AnimResource: public Resource
{
private:
    std::vector<Bone> bones;
    Mesh* mesh;

    void ReadMissingBones(std::map<std::string, BoneInfo>& _boneInfoMap, const aiAnimation* animation);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    Bone* FindBone(const char* _boneName);
    void CalculateBoneTransform(std::vector<float>*  finalBonesMatrices, const AssimpNodeData* _node, Mat4 _parentTransform, float _currentTime);
    void AssignBoneToNode(std::map<std::string, BoneInfo>& _boneInfoMap, AssimpNodeData* _node);

public:
    AssimpNodeData rootNode;
    float duration;
    int tickPerSecond;
    float animationSpeed = 0.2f;
    bool loop = false;
    bool goToIdle = true;

    AnimResource(const std::string& _name, std::string& _path, Mesh* _mesh = nullptr);
    ~AnimResource() = default;

    void UpdateAnimationResources(AssimpNodeData* _rootNode, Mesh* _mesh = nullptr);

    void RemoveFromResourcesManager() override;

    void UpdateAnimation(std::vector<float>* finalBonesMatrices, AssimpNodeData* _rootNode, float _deltaTime, bool _loop, float& _currentTime, bool& _animationFinish);
};

