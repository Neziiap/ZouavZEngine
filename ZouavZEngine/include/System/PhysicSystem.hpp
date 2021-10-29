#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"
#include "PxSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "System/Debug.hpp"
#include "Terrain.hpp"

#include "Object.hpp"
#include "GameObject.hpp"
#include "Component/Rigid.hpp"
#include "Component/RigidStatic.hpp"
#include "Component/ScriptComponent.hpp"
#include "Component/ShapeCollision.hpp"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	class PxPvdSceneClient;
	class PxRigidActor;
	class PxPvdTransport;
	class PxRigidDynamic;
	class PxRigidStatic;
}

class PhysicEventCallback : public physx::PxSimulationEventCallback
{
public:
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) override { Debug::Log("AWAKE !"); PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override { std::cout << "SLEEP !" << std::endl;  PX_UNUSED(actors); PX_UNUSED(count); }
	
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override 
	{ 
		for (physx::PxU32 i = 0; i < count; ++i)
		{
			if (pairs[i].flags & physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER || pairs[i].flags & physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)
				return;

			if (!static_cast<ShapeCollision*>(pairs[i].triggerShape->userData)->IsActive())
				return;

			if (pairs[i].triggerShape->userData && pairs[i].triggerActor->userData)
			{
				Object* otherActor = static_cast<Object*>(pairs[i].otherActor->userData);
				GameObject* triggerActor = static_cast<GameObject*>(pairs[i].triggerActor->userData);

				if (otherActor && triggerActor)
					triggerActor->GetComponent<Rigid>()->OnTrigger(otherActor, pairs[i].triggerShape);
			}
		}
	}

	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override { Debug::Log("Avance !"); }
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
	{ // TODO do all on contact like trigger
		if (pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0 || pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1)
			return;

		physx::PxRigidActor* rd = pairHeader.actors[0]->is<physx::PxRigidActor>();
		physx::PxRigidActor* rd2 = pairHeader.actors[1]->is<physx::PxRigidActor>();

		if (rd && rd2 && rd->userData && rd2->userData)
		{
			Object* obj1 = static_cast<Object*>(rd->userData);
			Object* obj2 = static_cast<Object*>(rd2->userData);

			if (dynamic_cast<GameObject*>(obj1) && obj2)
				static_cast<GameObject*>(obj1)->GetComponent<Rigid>()->OnContact(obj2, pairHeader.pairs->shapes[0]);
			if (dynamic_cast<GameObject*>(obj2) && obj1)
				static_cast<GameObject*>(obj2)->GetComponent<Rigid>()->OnContact(obj1, pairHeader.pairs->shapes[0]);
		}

		PX_UNUSED((pairs));
		PX_UNUSED((nbPairs));
	}

public:
	PhysicEventCallback() = default;
	~PhysicEventCallback() = default;
};

class PhysicSystem
{
private:
	static physx::PxDefaultAllocator physxAllocator;
	static physx::PxDefaultErrorCallback physxErrorCallback;

public:
	static physx::PxFoundation* foundation;
	static physx::PxPvd* pvd;
	static physx::PxPhysics* physics;
	static physx::PxCooking* cooking;
	static physx::PxScene* scene;
	static physx::PxPvdTransport* transport;
	//static PhysicEventCallback* physicEventCallback;

	static bool reloading;

	static bool IsReloading() { return reloading; }

	PhysicSystem() = delete;
	~PhysicSystem() = delete;

	static void Init();
	static void InitScene();

	static void EndInit();

	static void Destroy();
};