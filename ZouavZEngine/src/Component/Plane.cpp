#include "Component/Plane.hpp"
#include "GameObject.hpp"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicSystem.hpp"
#include "imgui.h"

using namespace physx;

Plane::Plane(GameObject* _gameObject, std::string _name)
	: ShapeCollision(_gameObject)
{
	//shape = PhysicSystem::physics->createShape(PxPlaneGeometry(), *material);
	//name = _name;

	//AttachToRigidComponent();
}

Plane::~Plane()
{

}

void Plane::Editor()
{
}