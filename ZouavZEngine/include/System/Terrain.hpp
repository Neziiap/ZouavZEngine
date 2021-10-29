#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Prefab.hpp"
#include "System/ResourcesManager.hpp"
#include "Object.hpp"

#include <FastNoiseLite.h>

#include <unordered_map>
#include "cereal/types/vector.hpp"
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

#define MAX_NOISE_COUNT 4
#define MAX_COLOR_COUNT 8 // need to change the TerrainShader.vs define too // TODO change only one
#define MAX_GENGO_COUNT 10

struct GeneratedGameObjectParam
{
	Prefab prefab;
	int ratio = 1;
	bool isOrientedByNormal = true;
};

struct NoiseParam
{
	FastNoiseLite::NoiseType noiseType = FastNoiseLite::NoiseType::NoiseType_Perlin;
	FastNoiseLite::FractalType fractalType = FastNoiseLite::FractalType::FractalType_None;
	int octaves = 1;
	float frequency = 0.01f;
	float lacunarity = 2.0f;
	float gain = 0.5f;
	float weightedStrength = 0.0f;
	float pingPongStength = 2.0f;
};

namespace physx
{
	class PxRigidStatic;
	class PxMaterial;
	class PxShape;
	class PxRigidActor;
}	

struct ChunkCreateArg
{
	std::vector<physx::PxMaterial*>& material;
	std::vector<GeneratedGameObjectParam>& toGeneratePrefabs;
	int nbGOPerChunk;
	int totalRatio;

	Vec2 pos;

	int size;
	int vertexCount;

	int seed;

	std::vector<NoiseParam> noiseParams;

	float minHeight;
	float maxHeight;
	float heightIntensity;
};

class Terrain;

class Chunk : public Object
{
	friend Terrain;
private:

	std::vector<class GameObject*> generatedGameObjects;

	Mesh mesh{"chunkMesh"};

	physx::PxShape* shape = nullptr;

	Vec2 pos{};

	physx::PxRigidActor* actor = nullptr;

	float yScalePrecision = 128.0f;

	int size = 0;
	int vertexCount = 0;

	bool isGenerated = false;
public:

	Chunk() : Object("Chunk", "Ground") {}
	Chunk(const Chunk&) = delete;
	~Chunk();

	float CalculateHeigt(ChunkCreateArg _cca, float _x, float _z);
	void Generate(ChunkCreateArg _cca, bool _reGenerate);

	const Mesh& GetMesh() const { return mesh; }

	Vec2 GetPos() const { return pos; }
	int GetSize() const { return size; }
	Vec2 GetWorldPos() const { return pos * (float)size; }
};

class Terrain
{
	friend class Scene;
private:
	static Terrain* terrain;

	std::unordered_map<std::string, std::unique_ptr<Chunk>> chunks;

	std::vector<physx::PxMaterial*> material;

	std::shared_ptr<Shader> shader;
	
	bool isGenerated = false;
	bool isActivated = false;

public:

	// infinite variable
	
	class GameObject* actualizer = nullptr;

	float chunkDistanceRadius = 256;

	// chunk variable

	int chunkSize = 128;
	int chunkVertexCount = 16;

	int seed = 0;

	int noiseCount = 0;
	int noiseID = 0;
	std::vector<NoiseParam> noiseParams;

	float minHeight = -10.0f;
	float maxHeight = 30.0f;
	float heightIntensity = 30.0f;

	// texture and color variable

	int colorCount = 0;
	int colorID = 0;
	std::vector<Vec3> colors;
	std::vector<float> colorHeight;
	std::vector<float> colorBlend;
	std::vector<float> colorStrength;
	std::vector<float> textureScale;
	std::vector<std::shared_ptr<Texture>> textureID;

	// Generated Gameobject variable
	
	std::vector<GeneratedGameObjectParam> GenGOParams;
	int totalRatio = 1;
	int nbGOPerChunk = 1;
	int GenGOCount = 0;
	int GenGOID = 0;

	// editor variable

	bool alwaysActualize = true;

	Terrain();

	bool IsGenerated() { return isGenerated; }

	void Generate(class GameObject* _actualizer = nullptr);
	void Actualise();

	void Update();
	void Draw(const class Camera& _camera) const;

	void DisplayOptionWindow();

	void AddNoiseLayer();
	void DeleteCurrentNoiseLayer();

	void AddColorLayer();
	void DeleteCurrentColorLayer();

	void AddGenGO();
	void DeleteCurrentGenGO();

	void ComputeTotalRatio();

	void Clear();

	static int GetChunkSize() { return terrain->chunkSize; }

	inline static Vec2 GetChunkPosFromWorldPos(const Vec3& _pos) 
	{ 
		return Vec2{ (float)((int)(_pos.x / (float)terrain->chunkSize - (_pos.x < 0.f ? 1.f : 0.f))), (float)((int)(_pos.z / (float)terrain->chunkSize - (_pos.z < 0.f ? 1.f : 0.f))) };
	};

	template <class Archive>
	void load(Archive& _ar)
	{
		int _noiseCount = 0;
		int _colorCount = 0;
		int _GenGOCount = 0;
		_ar(isActivated, chunkDistanceRadius, chunkSize, chunkVertexCount, seed, _noiseCount, noiseID,
			minHeight, maxHeight, heightIntensity, _colorCount, colorID, alwaysActualize, _GenGOCount);

		int _noiseType;
		int _fractalType;
		for (int i = 0; i < _noiseCount; ++i)
		{
			if (i != 0)
				AddNoiseLayer();

			_ar(_noiseType, _fractalType,
				noiseParams[i].octaves, noiseParams[i].frequency, noiseParams[i].lacunarity,
				noiseParams[i].gain, noiseParams[i].weightedStrength, noiseParams[i].pingPongStength);

			noiseParams[i].noiseType = (FastNoiseLite::NoiseType)_noiseType;
			noiseParams[i].fractalType = (FastNoiseLite::FractalType)_fractalType;
		}
		std::string _textureName;
		for (int i = 0; i < _colorCount; ++i)
		{
			if (i != 0)
				AddColorLayer();
			_ar(colors[i].x, colors[i].y, colors[i].z, colorHeight[i], colorBlend[i], colorStrength[i], textureScale[i], _textureName);
			textureID[i] = _textureName.compare("nullptr") == 0 ? nullptr : *ResourcesManager::GetResource<Texture>(_textureName);
		}

		_ar(nbGOPerChunk);
		for (int i = 0; i < _GenGOCount; ++i)
		{
			if (i != 0)
				AddGenGO();
			GenGOParams[i].prefab.load(_ar);
			_ar(GenGOParams[i].ratio, GenGOParams[i].isOrientedByNormal);
		}
		ComputeTotalRatio();
	}
	
	template <class Archive>
	void save(Archive& _ar)
	{
		_ar(isActivated, chunkDistanceRadius, chunkSize, chunkVertexCount, seed, noiseCount, noiseID,
			minHeight, maxHeight, heightIntensity, colorCount, colorID, alwaysActualize, GenGOCount);

		for (int i = 0; i < noiseCount; ++i)
			_ar((int)noiseParams[i].noiseType, (int)noiseParams[i].fractalType,
				noiseParams[i].octaves, noiseParams[i].frequency, noiseParams[i].lacunarity,
				noiseParams[i].gain, noiseParams[i].weightedStrength, noiseParams[i].pingPongStength);

		std::string null = "nullptr";

		for (int i = 0; i < colorCount; ++i)
			_ar(colors[i].x, colors[i].y, colors[i].z, colorHeight[i], colorBlend[i], colorStrength[i], textureScale[i], textureID[i] == nullptr ? null : textureID[i]->GetName());

		_ar(nbGOPerChunk);
		for (int i = 0; i < GenGOCount; ++i)
		{
			GenGOParams[i].prefab.save(_ar);
			_ar(GenGOParams[i].ratio, GenGOParams[i].isOrientedByNormal);
		}
	}
};
