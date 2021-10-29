#pragma once

#include "Component.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <cereal/types/base_class.hpp>

class Skybox : public Component
{
private:
	std::shared_ptr<class CubemapTexture> texture;
	std::shared_ptr<class Shader> shader;
	std::shared_ptr<class Mesh> cubeMesh;

public:
	Skybox() = delete;
	Skybox(class GameObject* _gameObject, std::string _name = "Skybox");
	Skybox(class GameObject* _gameObject, std::shared_ptr<class CubemapTexture>& _cubemapTexture, std::string _name = "Skybox");
	~Skybox() = default;

	void Draw(const class Camera& _camera);

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Skybox>& _construct);

};

CEREAL_REGISTER_TYPE(Skybox)

CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Skybox)
