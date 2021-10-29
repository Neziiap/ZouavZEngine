#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <memory>

class CapsuleCollision: public ShapeCollision
{
public:
	float radius;
	float halfHeight;

	CapsuleCollision(GameObject* _gameObject, float _radius = 1.0f, float _halfHeight = 1.0f, bool _isTrigger = false, Transform _tranform = Transform(), std::string _name = "CapsuleCollision");
	~CapsuleCollision();

	void Editor() override;
	void UpdateScale(class Rigid* _toAttach = nullptr) override;
	void DrawGizmos(const Camera& _camera) override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(radius, halfHeight);
		_ar(isTrigger);
		_ar(transform.localPosition.x, transform.localPosition.y, transform.localPosition.z,
			transform.localRotation.x, transform.localRotation.y, transform.localRotation.z, transform.localRotation.w,
			transform.localScale.x, transform.localScale.y, transform.localScale.z);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<CapsuleCollision>& _construct)
	{
		float _radius;
		float _halfHeight;
		bool trigger;
		Transform t;

		_ar(_radius, _halfHeight);
		_ar(trigger);
		_ar(t.localPosition.x, t.localPosition.y, t.localPosition.z,
			t.localRotation.x, t.localRotation.y, t.localRotation.z, t.localRotation.w,
			t.localScale.x, t.localScale.y, t.localScale.y);

		_construct(GameObject::currentLoadedGameObject, _radius, _halfHeight, trigger, t);
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(CapsuleCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, CapsuleCollision)