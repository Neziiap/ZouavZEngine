#pragma once
#include "Game/ICharacter.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Enemy : public ICharacter
{
   friend class cereal::access;
protected:
    GameObject* player = nullptr;
    float detectionDistance = 25.0f;
    float distanceToAttack = 2.0f;
    float distanceToStop = 1.5f;
    float speed = 2.0f;

    float decayTimer = 0.0f;
    float timeToDecay = 10.f;

public:
    Enemy() = delete;
    Enemy(class GameObject* _gameobject, std::string _name = "Enemy");
    void Begin() override;
    void Update() override;
    void Editor() override;
    
    void OnDeath() override;
    int xpGiven = 3;

    void UpdateLevel(int _level);

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(detectionDistance, distanceToAttack, distanceToStop, speed, timeToDecay);
        _ar(cereal::base_class<ICharacter>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<Enemy>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->detectionDistance, _construct->distanceToAttack, _construct->distanceToStop, _construct->speed, _construct->timeToDecay);
        _ar(cereal::base_class<ICharacter>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(Enemy)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ICharacter, Enemy)