#include "Component/CapsuleCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"
#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace physx;

CapsuleCollision::CapsuleCollision(GameObject* _gameObject, float _radius, float _halfHeight, bool _isTrigger, Transform _transform, std::string _name)
	: ShapeCollision(_gameObject, _transform, _isTrigger), radius(_radius), halfHeight(_halfHeight)
{
	name = _name;
	geometry = new PxCapsuleGeometry(radius, halfHeight);
	shape = PhysicSystem::physics->createShape(*geometry, *material);

	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Capsule");
	UpdateScale();
}

CapsuleCollision::~CapsuleCollision()
{

}

void CapsuleCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat("Radius : ", &radius, 0.1f, 100.0f))
		UpdateScale();
	
	if (ImGui::SliderFloat("HalfHeight : ", &halfHeight, 0.1f, 100.0f))
		UpdateScale();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void CapsuleCollision::UpdateScale(Rigid* _toAttach)
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (!rigid)
		rigid = _toAttach;
	
	if (rigid && shape)
		rigid->actor->detachShape(*shape);
	
	//TODO use gameObject->WorldScale()
	geometry = new PxCapsuleGeometry(radius, halfHeight);

	AttachToRigidComponent();
	shape->userData = this;
}

void CapsuleCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	gizmoShader->Use();

	Mat4 trsLocal = Mat4::CreateTRSMatrix(transform.localPosition, transform.localRotation, Vec3(radius, halfHeight, radius));
	Mat4 trsGlobal = Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale());

	Mat4 mat = trsGlobal * trsLocal;

	gizmoShader->SetMatrix("matrix", mat);
	gizmoShader->SetMatrix("view", _camera.GetMatrix().Reversed());
	gizmoShader->SetMatrix("projection", _camera.GetProjectionMatrix());
	gizmoShader->SetVector4("color", Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	glBindVertexArray(gizmoMesh->GetID());
	glDrawElements(GL_LINE_LOOP, gizmoMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}