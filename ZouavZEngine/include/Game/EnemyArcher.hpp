#pragma once
#include "Game/Enemy.hpp"
#include "Prefab.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class EnemyArcher : public Enemy
{
   friend class cereal::access;
private:
    Prefab arrow;

public:
    EnemyArcher() = delete;
    EnemyArcher(class GameObject* _gameobject, std::string _name = "EnemyArcher");
    void Begin() override;
    void Update() override;
    void Editor() override;
    void Attack() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(arrow);
        _ar(cereal::base_class<Enemy>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<EnemyArcher>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->arrow);
        _ar(cereal::base_class<Enemy>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(EnemyArcher)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Enemy, EnemyArcher)