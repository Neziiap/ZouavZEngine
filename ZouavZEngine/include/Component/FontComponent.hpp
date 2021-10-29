#pragma once

#include "Rendering/Font.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Component/Component.hpp"
#include "Maths/Vec3.hpp"
#include "Maths/Vec4.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

#include <string>
#include <memory>

enum class E_FONT_TYPE
{
	FONT3D = 1,
	FONTBILLBOARD,
	FONT2D,
	NOFONT
};

class FontComponent : public Component
{
private:
	std::shared_ptr<Font> font;
	std::shared_ptr<Shader> shader3D;
	std::shared_ptr<Shader> shaderBillboard;
	std::shared_ptr<Shader> shader2D;

	Mesh mesh{ "TextMesh" };

	E_FONT_TYPE fontType = E_FONT_TYPE::FONT3D;
	std::string text{ "Default Text" };
	Vec4 color{ 0.f, 0.f, 0.f, 1.0f };
	Vec3 position{ 0.f, 0.f, 0.f};
	float fontSize {4.0f};
	
	float width = 0.5f;
	float edge = 0.01f;

	float outlineWidth = 0.0f;
	float outlineEdge = 0.001f;
	Vec3 outlineColor{ 0.0f, 0.0f, 0.0f };

	Vec2 offset{0.0f, 0.0f};

	bool depthTest = false;

	bool isGenerated = false;

public:
	FontComponent() = delete;
	FontComponent(class GameObject* _gameObject, std::string _name = "FontComponent");
	FontComponent(class GameObject* _gameObject, std::shared_ptr<Font>& _font, std::string _name = "FontComponent");
	~FontComponent() override;

	const std::string& GetText() const { return text; }

	void ChangeText(const char* _newText, int _size);
	void ChangeText(std::string _newText);
	void ChangeType(E_FONT_TYPE _newType);
	void Draw3D(const class Camera& _camera);
	void DrawBillboard(const class Camera& _camera);
	void Draw2D(const class Camera& _camera);

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(font->GetName(), font->IsDeletable(), font->GetPath(), (int)fontType, text,
			color.x, color.y, color.z, color.w, position.x, position.y, position.z,
			fontSize, width, edge, outlineWidth, outlineEdge, 
			outlineColor.x, outlineColor.y, outlineColor.z, offset.x, offset.y, depthTest);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<FontComponent>& _construct);
};

CEREAL_REGISTER_TYPE(FontComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FontComponent)