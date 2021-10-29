#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <memory>

class BoxCollision: public ShapeCollision
{
public:
	Vec3 halfExtends;

	BoxCollision(GameObject* _gameObject, Vec3 _halfExtends = { 1.0f, 1.0f, 1.0f }, bool _isTrigger = false, Transform _tranform = Transform(), std::string _name = "BoxCollision");
	~BoxCollision() = default;

	void Editor() override;
	void DrawGizmos(const Camera& _camera) override;
	void UpdateScale(class Rigid* _toAttach = nullptr) override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(halfExtends);
		_ar(isTrigger);
		_ar(transform.localPosition, transform.localRotation, transform.localScale);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<BoxCollision>& _construct)
	{
		Vec3 halfExtends;
		bool trigger;
		Transform t;
		_ar(halfExtends);
		_ar(trigger);
		_ar(t.localPosition, t.localRotation, t.localScale);

		_construct(GameObject::currentLoadedGameObject, halfExtends, trigger, t);
		_construct->UpdateScale();
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(BoxCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, BoxCollision)