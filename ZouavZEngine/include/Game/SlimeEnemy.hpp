#pragma once
#include "Game/Enemy.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class SlimeEnemy : public Enemy
{
   friend class cereal::access;
private:
    int dividedCount = 0;
    int maxDivision = 2;
public:
    SlimeEnemy() = delete;
    SlimeEnemy(class GameObject* _gameobject, std::string _name = "SlimeEnemy");
    void Begin() override;
    void Update() override;
    void Editor() override;

    void OnDeath() override;

    template <class Archive>
    void serialize(Archive & _ar) 
    {
        _ar(cereal::base_class<Enemy>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<SlimeEnemy>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<Enemy>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(SlimeEnemy)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Enemy, SlimeEnemy)