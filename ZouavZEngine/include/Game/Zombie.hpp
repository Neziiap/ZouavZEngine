#pragma once
#include "Game/Enemy.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Zombie : public Enemy
{
   friend class cereal::access;
private:
public:
    Zombie() = delete;
    Zombie(class GameObject* _gameobject, std::string _name = "Zombie");
    void Begin() override;
    void Update() override;
    void Editor() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cereal::base_class<Enemy>(this));
    }
    
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<Zombie>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<Enemy>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(Zombie)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Enemy, Zombie)