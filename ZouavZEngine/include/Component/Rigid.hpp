#pragma once

#include "Component/Component.hpp"
#include "Transform.hpp"

namespace physx
{
	class PxRigidActor;
	class PxShape;
}

class Rigid : public Component
{
protected:
	void AttachShape();
	void InternalActivate() override;
	void InternalDeactivate() override;
public:
	physx::PxRigidActor* actor = nullptr;

	Rigid() = delete;
	Rigid(class GameObject* _gameObject, std::string _name = "Rigid");
	~Rigid();

	void Activate() override;
	void Deactivate() override;

	virtual void UpdateTransform(Transform transform) {};

	virtual void OnContact(class Object* _other, physx::PxShape* _collidingShap);
	virtual void OnTrigger(class Object* _other, physx::PxShape* _collidingShape);
};