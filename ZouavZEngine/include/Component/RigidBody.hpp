#pragma once

#include "Component/Component.hpp"
#include "Component/Rigid.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

namespace physx
{
	class PxGeometry;
	class PxRigidDynamic;
}

class RigidBody : public Rigid
{
public:
	bool lockX, lockY, lockZ = false;

	RigidBody() = delete;
	RigidBody(GameObject* _gameObject, std::string _name = "RigidBody");
	~RigidBody() = default;

	void Editor() override;

	void SetLinearVelocity(const class Vec3& _v);
	void AddForce(const class Vec3& _v);
	class Vec3 GetLinearVelocity();
	void DisableGravity();
	void EnableGravity();
	void LockAxis();

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(lockX, lockY, lockZ);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct);
};

CEREAL_REGISTER_TYPE(RigidBody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody)
