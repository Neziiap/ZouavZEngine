#include "Component/SphereCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"
#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Component/RigidBody.hpp"

using namespace physx;

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, bool _isTrigger, Transform _tranform, std::string _name)
	: ShapeCollision(_gameObject, _tranform, _isTrigger), radius(_radius)
{	
	name = _name;
	geometry = new PxSphereGeometry(_radius);
	shape = PhysicSystem::physics->createShape(*geometry, *material);

	//attach shape to a Rigid component if exist
	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Sphere");
	UpdateScale();
}

void SphereCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat("Radius : ", &radius, 0.1f, 100.0f))
		UpdateScale();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void SphereCollision::UpdateScale(Rigid* _toAttach)
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (!rigid)
		rigid = _toAttach;

	if (rigid && shape)
		rigid->actor->detachShape(*shape);

	//TODO world scale
	geometry = new PxSphereGeometry(radius);

	AttachToRigidComponent();
	shape->userData = this;
}

void SphereCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	gizmoShader->Use();

	Mat4 trsLocal = Mat4::CreateTRSMatrix(transform.localPosition, transform.localRotation, Vec3(radius / 1.5f, radius /1.5f, radius / 1.5f));
	Mat4 trsGlobal = Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale());

	Mat4 mat = trsGlobal * trsLocal;

	gizmoShader->SetMatrix("matrix", mat);
	gizmoShader->SetMatrix("view", _camera.GetMatrix().Reversed());
	gizmoShader->SetMatrix("projection", _camera.GetProjectionMatrix());
	gizmoShader->SetVector4("color", Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	glBindVertexArray(gizmoMesh->GetID());
	glDrawElements(GL_TRIANGLES, gizmoMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}