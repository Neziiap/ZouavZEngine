#include "Component/RigidStatic.hpp"
#include "GameObject.hpp"
#include "PxRigidStatic.h"
#include "System/PhysicUtils.hpp"
#include "Component/BoxCollision.hpp"
#include "PxScene.h"
#include "System/PhysicSystem.hpp"
#include "imgui.h"

using namespace physx;

RigidStatic::RigidStatic(GameObject* _gameObject, std::string _name)
	: Rigid(_gameObject, _name)
{
	if (_gameObject == nullptr)
		return;

	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidStatic(t);

	actor->userData = _gameObject;

	AttachShape();

	PhysicSystem::scene->addActor(*actor);

	if (!_gameObject->IsActive())
		InternalDeactivate();
}

void RigidStatic::Activate()
{
	Component::Activate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidStatic::Deactivate()
{
	Component::Deactivate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void RigidStatic::InternalActivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidStatic::InternalDeactivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

RigidStatic::~RigidStatic()
{
	
}

void RigidStatic::Editor()
{

}

template <class Archive>
static void RigidStatic::load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}
