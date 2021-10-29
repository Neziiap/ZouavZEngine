#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "geometry/PxGeometry.h"

namespace physx
{
	class PxActor;
	class PxMaterial;
}

class ShapeCollision: public Component
{
protected:
	Transform transform;
	std::shared_ptr<Shader> gizmoShader;
	std::shared_ptr<Mesh> gizmoMesh;
	void AttachToRigidComponent(class Rigid* _toAttach = nullptr);
	void InternalActivate() override;
	void InternalDeactivate() override;
	bool isTrigger = false;

public:
	physx::PxGeometry* geometry;
	physx::PxMaterial* material = nullptr;
	physx::PxShape* shape = nullptr;
	bool isAttach = false;

	ShapeCollision(GameObject* _gameObject, Transform _transform = Transform(), bool _isTrigger = false, std::string _name = "ShapeCollision");
	~ShapeCollision();

	void releasePhysXComponent();
	bool IsTrigger() { return isTrigger; }
	void SetTrigger(bool _isTrigger);
	void UpdateIsTrigger();
	Vec3 GetPosition() { return transform.localPosition; }
	void EditPosition(const Vec3& _newPos);
	void EditRotation(const Vec3& _newRot);
	virtual void Editor() override;
	virtual void DrawGizmos(const Camera& _camera);
	void UpdateShapeTransform();
	virtual void UpdateScale(class Rigid* _toAttach = nullptr) { };
		
	void Activate() override;
	void Deactivate() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ShapeCollision>& _construct)
	{
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}

};

CEREAL_REGISTER_TYPE(ShapeCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShapeCollision)