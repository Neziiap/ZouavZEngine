#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"
#include "Prefab.hpp"
#include "Maths/Vec2.hpp"

class EnemyManager : public ScriptComponent
{
   friend class cereal::access;
private:
    static std::vector<class Enemy*> enemies;
    int nbEnemiesPrefab = 0;
    std::vector<Prefab> enemiesPrefab;
    std::vector<GameObject*> arenaObject;
    Prefab spawner;
    Prefab cornerPillar;
    
    GameObject* player = nullptr;
    class Player* playerComp = nullptr;
    Vec2 playerChunkPos;

    Vec2 currentChunkWorldPos;
    int chunkSize = 0;

    bool isSpawningDone = false;
    bool asDoneChunk = false;

    float toSpawnTime = 10.0f;
    float toFinishSpawn = 20.0f;
    float timerToSpawn = 0.0f;
    float timerToFinish = 0.0f;

public:
    EnemyManager() = delete;
    EnemyManager(class GameObject* _gameobject, std::string _name = "EnemyManager");

    void Begin() override;
    void Update() override;
    void Editor() override;

    static void AddEnemy(class Enemy* _enemy);
    static void RemoveEnemy(class Enemy* _enemy);

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(nbEnemiesPrefab);
        for (int i = 0; i < nbEnemiesPrefab; ++i)
            enemiesPrefab[i].save(_ar);
        spawner.save(_ar);
        cornerPillar.save(_ar);
        _ar(cereal::base_class<ScriptComponent>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<EnemyManager>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->nbEnemiesPrefab);
        for (int i = 0; i < _construct->nbEnemiesPrefab; ++i)
        {
            _construct->enemiesPrefab.emplace_back(Prefab{});
            _construct->enemiesPrefab[i].load(_ar);
        }
        _construct->spawner.load(_ar);
        _construct->cornerPillar.load(_ar);
        _ar(cereal::base_class<ScriptComponent>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(EnemyManager)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, EnemyManager)