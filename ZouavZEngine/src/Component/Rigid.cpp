#include "Component/Rigid.hpp"
#include "Object.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidActorExt.h"
#include "extensions/PxRigidBodyExt.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "System/PhysicUtils.hpp"
#include "Component/ShapeCollision.hpp"

using namespace physx;

Rigid::Rigid(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	
}

void Rigid::Activate()
{
	Component::Activate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void Rigid::Deactivate()
{
	Component::Deactivate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void Rigid::InternalActivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void Rigid::InternalDeactivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void Rigid::AttachShape()
{
	std::vector<ShapeCollision*> collisions = GetGameObject().GetComponents<ShapeCollision>();

	for (ShapeCollision* collision : collisions)
	{
		if (collision && !collision->isAttach)
		{
			collision->isAttach = true;

			collision->shape = physx::PxRigidActorExt::createExclusiveShape(*actor, *collision->geometry, *collision->material);
			if (!collision->shape)
				Debug::LogError("Attach to shape failed !");
			else
				collision->UpdateScale(this);
		}
	}
}

void Rigid::OnContact(Object* _other, physx::PxShape* _collidingShape)
{
	if (!gameObject)
		return;

	std::vector<ScriptComponent*> scripts = GetGameObject().GetComponents<ScriptComponent>();

	ShapeCollision* collision = static_cast<ShapeCollision*>(_collidingShape->userData);

	for (ScriptComponent* script : scripts)
		script->OnContact(_other, collision);
}

void Rigid::OnTrigger(Object* _other, physx::PxShape* _collidingShape)
{
	if (!gameObject)
		return;

	std::vector<ScriptComponent*> scripts = GetGameObject().GetComponents<ScriptComponent>();

	ShapeCollision* collision = static_cast<ShapeCollision*>(_collidingShape->userData);

	for (ScriptComponent* script : scripts)
		script->OnTrigger(_other, collision);
}

Rigid::~Rigid()
{
	if (!PhysicSystem::scene || !GetGameObject().IsActive())
		return;

	if (actor)
	{
		PhysicSystem::scene->removeActor(*actor, false);
		actor = nullptr;
	}
}