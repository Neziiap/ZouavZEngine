#include "Component/RigidBody.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "System/PhysicUtils.hpp"
#include "Component/ShapeCollision.hpp"
#include "Component/ScriptComponent.hpp"
#include "imgui.h"

using namespace physx;


RigidBody::RigidBody(GameObject* _gameObject, std::string _name)
	: Rigid(_gameObject, _name)
{
	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidDynamic(t);

	actor->userData = _gameObject;

	PhysicSystem::scene->addActor(*actor);

	AttachShape();

	if (!_gameObject->IsActive())
		InternalDeactivate();
	
	LockAxis();
}

void RigidBody::SetLinearVelocity(const class Vec3& _v)
{
	if(isActive)
		((PxRigidBody*)actor)->setLinearVelocity(PxVec3FromVec3(_v));
}

void RigidBody::AddForce(const class Vec3& _v)
{
	((PxRigidBody*)actor)->addForce(PxVec3FromVec3(_v));
}

Vec3 RigidBody::GetLinearVelocity()
{
	return Vec3FromPxVec3(((PxRigidBody*)actor)->getLinearVelocity());
}

void RigidBody::DisableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::EnableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::Editor()
{
	if (ImGui::Checkbox("Lock Rotaion X ", &lockX))
		LockAxis();

	if (ImGui::Checkbox("Lock Rotaion Y ", &lockY))
		LockAxis();

	if (ImGui::Checkbox("Lock Rotaion Z ", &lockZ))
		LockAxis();
}

void RigidBody::LockAxis()
{
	PxRigidDynamic* rd = static_cast<PxRigidDynamic*>(actor);

	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockZ);
	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockX);
	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockY);
}

template <class Archive>
static void RigidBody::load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(_construct->lockX, _construct->lockY, _construct->lockZ);
	_construct->LockAxis();
	_ar(cereal::base_class<Component>(_construct.ptr()));
}