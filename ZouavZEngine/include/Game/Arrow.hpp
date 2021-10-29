#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Arrow : public ScriptComponent
{
   friend class cereal::access;
private:

public:
    Vec3 direction{ 1.0f, 0.0f, 0.0f };
    float speed{ 1.0f };
    int damage{ 1 };
    float lifeTime{ 5.0f };

    Arrow() = delete;
    Arrow(class GameObject* _gameobject, std::string _name = "Arrow");
    void Begin() override;
    void Update() override;
    void Editor() override;
    void Initiate(const Vec3& _direction, float _damage);
    void OnTrigger(class Object* _other, class ShapeCollision* _triggerShape) override;
    void OnAddComponent();

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cereal::base_class<ScriptComponent>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<Arrow>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<ScriptComponent>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(Arrow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, Arrow)