#include "Rendering/AnimResource.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/ResourcesManager.hpp"

AnimResource::AnimResource(const std::string& _name, std::string& _path, Mesh* _mesh)
	:Resource(_name, _path.c_str())
{
	bones.clear();
	rootNode.children.clear();
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		Debug::LogWarning(std::string("Animation load failed!: ").append(_path));
		return;
	}

	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	mesh = _mesh;

	if (!mesh)
		return;

	std::map<std::string, BoneInfo> boneInfoMap = mesh->boneInfoMap;

	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(boneInfoMap, animation);

	rootNode.bone = FindBone(rootNode.name.c_str());
	if (boneInfoMap.find(rootNode.name) != boneInfoMap.end())
	{
		rootNode.boneInfo = boneInfoMap.find(rootNode.name)->second;
		rootNode.boneInfo.isValid = true;
	}
	else
		rootNode.boneInfo.isValid = false;

	for (auto& child : rootNode.children)
		AssignBoneToNode(boneInfoMap, &child);

	Debug::Log("Animation resource : " + _name + " loaded");
}

void AnimResource::AssignBoneToNode(std::map<std::string, BoneInfo>& _boneInfoMap, AssimpNodeData* _node)
{
	_node->bone = FindBone(_node->name.c_str());
	if (_boneInfoMap.find(_node->name) != _boneInfoMap.end())
	{
		_node->boneInfo = _boneInfoMap.find(_node->name)->second;
		_node->boneInfo.isValid = true;
	}
	else
		_node->boneInfo.isValid = false;
	for (auto& child : _node->children)
		AssignBoneToNode(_boneInfoMap, &child);
}

void AnimResource::UpdateAnimationResources(AssimpNodeData* _rootNode, Mesh* _mesh)
{
	//no need to reread bone if mesh already set
	if (mesh == _mesh)
		return;

	bones.clear();
	rootNode.children.clear();
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene)
	{
		Debug::LogWarning(std::string("Animation load failed!: ").append(path));
		return;
	}
	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	mesh = _mesh;

	std::map<std::string, BoneInfo> boneInfoMap = mesh->boneInfoMap;

	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(boneInfoMap, animation);

	rootNode.bone = FindBone(rootNode.name.c_str());
	if (boneInfoMap.find(rootNode.name) != boneInfoMap.end())
	{
		rootNode.boneInfo = boneInfoMap.find(rootNode.name)->second;
		rootNode.boneInfo.isValid = true;
	}
	else
		rootNode.boneInfo.isValid = false;

	for (auto& child : rootNode.children)
		AssignBoneToNode(boneInfoMap, &child);

	_rootNode = &rootNode;
}

void AnimResource::RemoveFromResourcesManager()
{

}

void AnimResource::UpdateAnimation(std::vector<float>* finalBonesMatrices, AssimpNodeData* _rootNode, float _deltaTime, bool _loop, float& _currentTime, bool& _animationFinish)
{
	if (!_animationFinish)
	{
		_currentTime += tickPerSecond * animationSpeed * _deltaTime;

		if (_currentTime >= duration)
		{
			_currentTime = 0.0f;
			_animationFinish = true;
		}
	}
	if (_animationFinish && _loop)
		_animationFinish = false;

	if(!_animationFinish)
		CalculateBoneTransform(finalBonesMatrices, &rootNode, Mat4::identity, _currentTime);
}

void AnimResource::ReadMissingBones(std::map<std::string, BoneInfo>& _boneInfoMap, const aiAnimation* animation)
{
	bones.clear();
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		auto channel = animation->mChannels[i];

		std::string boneName = channel->mNodeName.data;

		if (_boneInfoMap.find(boneName) == _boneInfoMap.end())
		{
			_boneInfoMap[boneName].id = mesh->boneCounter;
			mesh->boneCounter++;
		}
		bones.emplace_back(Bone{ channel->mNodeName.data,
			_boneInfoMap[channel->mNodeName.data].id, channel });
	}

	mesh->boneInfoMap = _boneInfoMap;
}

void AnimResource::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	dest.children.clear();

	dest.name = src->mName.data;
	dest.transformation = Mat4::ConvertAssimpMatrixToMat4(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.emplace_back(newData);
	}
}

Bone* AnimResource::FindBone(const char* _boneName)
{
	for (auto& bone : bones)
	{
		if (strcmp(bone.name.c_str(), _boneName) == 0)
			return &bone;
	}
	return nullptr;
}

void AnimResource::CalculateBoneTransform(std::vector<float>* finalBonesMatrices, const AssimpNodeData* _node, Mat4 _parentTransform, float _currentTime)
{
	const char* nodeName = _node->name.c_str();
	Mat4 nodeTransform = _node->transformation;

	if (_node->bone)
	{
		_node->bone->Update(_currentTime);
		nodeTransform = _node->bone->localTransform;
	}

	Mat4 globalTransformation = _parentTransform * nodeTransform;

	if (_node->boneInfo.isValid)
	{
		int index = _node->boneInfo.id * 16;
		Mat4 mat = globalTransformation * _node->boneInfo.offset;

		(*finalBonesMatrices)[index] = mat.matrix[0];
		(*finalBonesMatrices)[index + 1] = mat.matrix[1];
		(*finalBonesMatrices)[index + 2] = mat.matrix[2];
		(*finalBonesMatrices)[index + 3] = mat.matrix[3];
		(*finalBonesMatrices)[index + 4] = mat.matrix[4];
		(*finalBonesMatrices)[index + 5] = mat.matrix[5];
		(*finalBonesMatrices)[index + 6] = mat.matrix[6];
		(*finalBonesMatrices)[index + 7] = mat.matrix[7];
		(*finalBonesMatrices)[index + 8] = mat.matrix[8];
		(*finalBonesMatrices)[index + 9] = mat.matrix[9];
		(*finalBonesMatrices)[index + 10] = mat.matrix[10];
		(*finalBonesMatrices)[index + 11] = mat.matrix[11];
		(*finalBonesMatrices)[index + 12] = mat.matrix[12];
		(*finalBonesMatrices)[index + 13] = mat.matrix[13];
		(*finalBonesMatrices)[index + 14] = mat.matrix[14];
		(*finalBonesMatrices)[index + 15] = mat.matrix[15];
	}

	for (int i = 0; i < _node->childrenCount; i++)
		CalculateBoneTransform(finalBonesMatrices, &_node->children[i], globalTransformation, _currentTime);
}
