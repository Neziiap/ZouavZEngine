#pragma once

#include "Component/ScriptComponent.hpp"
#include "Maths/Vec4.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class ICharacter : public ScriptComponent
{
protected:
    friend class cereal::access;

    class Material* material = nullptr;
    class FontComponent* lifeFont = nullptr;
	Vec4 baseColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    Vec4 damageColor{ 1.0f, 0.0f, 0.0f, 1.0f };
    float invulnerabilityFrame = 1.0f;
    float invulnerabilityTimer = 0.0f;
    float beforeAttack = 0.1f;
    float timerBeforeAttack = 0.0f;
    int life = 5;
    int maxLife = 5;
    int attackDamage = 1;
    int level = 1;
    bool asTakenDamage = false;
    float attackDuration = 0.5f;
    float attackCooldown = 1.0f;
    float timerAttackDuration = 0.0f;
    float timerAttackCooldown = 0.0f;
    class BoxCollision* attackCollision;
    class RigidBody* rb{ nullptr };
    class AudioBroadcaster* audioBroadcaster;
    class Animation* animation;
    std::string deathAnimName;
    std::string attackAnimName;
    std::string walkAnimName;
    std::string idleAnimName;

public:
    ICharacter() = delete;
    ICharacter(class GameObject* _gameobject, std::string _name = "ICharacter");
    void Begin() override;
    void Update() override;
    void Editor() override;

    void OnAddComponent() override;

    void OnTrigger(class Object* _other, class ShapeCollision* _triggerShape) override;

    int GetLevel() { return level; };
    int GetLife() const { return life; }
    bool IsAlive() const { return life > 0; }
    bool IsAttacking() const;
    bool Damage(int _damage);
    void NeedToAttack();
    virtual void Attack();
    virtual void StopAttack();
    virtual void OnDeath() {};

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(damageColor, invulnerabilityFrame, life, maxLife, attackDamage, level, attackDuration, attackCooldown, beforeAttack);
        _ar(cereal::base_class<Component>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<ICharacter>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->damageColor, _construct->invulnerabilityFrame, _construct->life, _construct->maxLife, _construct->attackDamage, _construct->level, _construct->attackDuration, _construct->attackCooldown, _construct->beforeAttack);
        _ar(cereal::base_class<Component>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(ICharacter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, ICharacter)