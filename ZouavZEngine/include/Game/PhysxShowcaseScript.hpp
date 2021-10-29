#pragma once
#include "Component/ScriptComponent.hpp"
#include "Prefab.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class PhysxShowcaseScript : public ScriptComponent
{
   friend class cereal::access;
private:
    Prefab cubePrefab;
    float timerSpawn = 0.0f;
    float spawnCooldown = 0.5f;
public:
    PhysxShowcaseScript() = delete;
    PhysxShowcaseScript(class GameObject* _gameobject, std::string _name = "PhysxShowcaseScript");
    void Begin() override;
    void Update() override;
    void Editor() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cubePrefab);
        _ar(cereal::base_class<ScriptComponent>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<PhysxShowcaseScript>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->cubePrefab);
        _ar(cereal::base_class<ScriptComponent>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(PhysxShowcaseScript)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, PhysxShowcaseScript)