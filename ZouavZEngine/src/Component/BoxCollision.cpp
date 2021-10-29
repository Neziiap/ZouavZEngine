#include "Component/BoxCollision.hpp"
#include "GameObject.hpp"
#include "PxActor.h"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "extensions/PxRigidActorExt.h"
#include "System/PhysicUtils.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Component/RigidBody.hpp"
#include "imgui.h"
#include <memory>

using namespace physx;

BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger, Transform _transform, std::string _name)
	: ShapeCollision(_gameObject, _transform, _isTrigger), halfExtends(_halfExtends)
{
	name = _name;
	geometry = new PxBoxGeometry(PxVec3FromVec3(_halfExtends));
	shape = nullptr;

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Default");

	UpdateScale();
}

void BoxCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::DragFloat3("Half Extends : ", &halfExtends.x, 0.1f, 0.1f, 100.0f))
		UpdateScale();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void BoxCollision::UpdateScale(Rigid* _toAttach)
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (!rigid)
		rigid = _toAttach;

	if (rigid && shape)
		rigid->actor->detachShape(*shape);

	geometry = new PxBoxGeometry(PxVec3FromVec3(halfExtends * gameObject->WorldScale()) / 2.0f);

	AttachToRigidComponent(_toAttach);
	if(shape)
		shape->userData = this;

	UpdateShapeTransform();
}

void BoxCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	gizmoShader->Use();
	
	Mat4 trsLocal = Mat4::CreateTRSMatrix(transform.localPosition, transform.localRotation, halfExtends * gameObject->WorldScale());
	Mat4 trsGlobal = Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), { 1.f, 1.f, 1.f });

	Mat4 mat = trsGlobal * trsLocal;

	gizmoShader->SetMatrix("matrix", mat);
	gizmoShader->SetVector4("color", Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	glBindVertexArray(gizmoMesh->GetID());
	glDrawElements(GL_LINE_LOOP, gizmoMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}