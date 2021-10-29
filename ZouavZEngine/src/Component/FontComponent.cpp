#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include "System/ResourcesManager.hpp"
#include "System/FontSystem.hpp"
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Component/FontComponent.hpp"



FontComponent::FontComponent(class GameObject* _gameObject, std::string _name)
	: Component( _gameObject, _name),	font{ *ResourcesManager::GetResource<Font>("Default") }, 
								shader3D { *ResourcesManager::GetResource<Shader>("Font3DShader") },
								shaderBillboard{ *ResourcesManager::GetResource<Shader>("FontBillboardShader") },
								shader2D{ *ResourcesManager::GetResource<Shader>("Font2DShader") }
{
	ChangeText(text.c_str(), text.size());
	FontSystem::AddFont3D(this);
}

FontComponent::FontComponent(GameObject* _gameObject, std::shared_ptr<Font>& _font, std::string _name)
	: Component(_gameObject, _name),	font{ _font }, 
								shader3D{ *ResourcesManager::GetResource<Shader>("Font3DShader") },
								shaderBillboard{ *ResourcesManager::GetResource<Shader>("FontBillboardShader") },
								shader2D{ *ResourcesManager::GetResource<Shader>("Font2DShader") }
{
	ChangeText(text.c_str(), text.size());
	FontSystem::AddFont3D(this);
}

FontComponent::~FontComponent()
{
	switch (fontType)
	{
		case E_FONT_TYPE::FONT3D :
			FontSystem::RemoveFont3D(this);
			break;
		case E_FONT_TYPE::FONTBILLBOARD :
			FontSystem::RemoveFontBillboard(this);
			break;
		case E_FONT_TYPE::FONT2D :
			FontSystem::RemoveFont2D(this);
			break;
		default :
			break;
	}
}

void FontComponent::ChangeType(E_FONT_TYPE _newType)
{
	if (_newType == fontType)
		return;

	switch (fontType)
	{
		case E_FONT_TYPE::FONT3D:
			FontSystem::RemoveFont3D(this);
			break;
		case E_FONT_TYPE::FONTBILLBOARD:
			FontSystem::RemoveFontBillboard(this);
			break;
		case E_FONT_TYPE::FONT2D:
			FontSystem::RemoveFont2D(this);
			break;
		default:
			break;
	}

	fontType = _newType;

	switch (fontType)
	{
		case E_FONT_TYPE::FONT3D:
			FontSystem::AddFont3D(this);
			break;
		case E_FONT_TYPE::FONTBILLBOARD:
			FontSystem::AddFontBillboard(this);
			break;
		case E_FONT_TYPE::FONT2D:
			FontSystem::AddFont2D(this);
			break;
		default:
			break;
	}
}

void FontComponent::Editor()
{
	if (ImGui::InputText("Text : ", &text))
		ChangeText(text.c_str(), text.size());

	std::string oldValue = fontType == E_FONT_TYPE::FONT3D ? "Font 3D" : fontType == E_FONT_TYPE::FONTBILLBOARD ? "Font Billboard" : "Font 2D";
	if (ImGui::BeginCombo("Type : ", oldValue.c_str()))
	{
		E_FONT_TYPE newType = fontType;
		if (ImGui::Selectable("Font 3D", fontType == E_FONT_TYPE::FONT3D)) newType = E_FONT_TYPE::FONT3D;
		if (ImGui::Selectable("Font Billboard", fontType == E_FONT_TYPE::FONTBILLBOARD)) newType = E_FONT_TYPE::FONTBILLBOARD;
		if (ImGui::Selectable("Font 2D", fontType == E_FONT_TYPE::FONT2D)) newType = E_FONT_TYPE::FONT2D;
		ImGui::EndCombo();
		ChangeType(newType);
	}

	if (ImGui::InputFloat("Font Size :", &fontSize, 0.1f))
		ChangeText(text.c_str(), text.size());

	ImGui::ColorEdit4("Color :", &color.x);
	
	ImGui::DragFloat3("Position :", &position.x, 0.1f);
	
	ImGui::InputFloat("Width :", &width, 0.01f);
	ImGui::InputFloat("Edge :", &edge, 0.01f);

	ImGui::ColorEdit3("Outline Color :", &outlineColor.x);
	ImGui::InputFloat("Outline Width :", &outlineWidth, 0.01f);
	ImGui::InputFloat("Outline Edge :", &outlineEdge, 0.01f);

	ImGui::DragFloat2("Offset :", &offset.x, 0.0001f);

	ImGui::Checkbox("Depth Test :", &depthTest);
}

void FontComponent::Draw3D(const Camera& _camera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!depthTest)
		glDisable(GL_DEPTH_TEST);

	shader3D->Use();
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture().Use();

	shader3D->SetMatrix("model", Mat4::CreateTRSMatrix(GetGameObject().WorldPosition(), GetGameObject().WorldRotation(), GetGameObject().WorldScale()) 
								* Mat4::CreateTranslationMatrix(position));
	shader3D->SetVector4("color", color);
	shader3D->SetFloat("width", width);
	shader3D->SetFloat("edge", edge);
	shader3D->SetFloat("outlineWidth", outlineWidth);
	shader3D->SetFloat("outlineEdge", outlineEdge);
	shader3D->SetVector3("outlineColor", outlineColor);
	shader3D->SetVector2("offset", offset);

	glBindVertexArray(mesh.GetID());
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);

	if (!depthTest)
		glEnable(GL_DEPTH_TEST);
}

void FontComponent::DrawBillboard(const Camera& _camera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!depthTest)
		glDisable(GL_DEPTH_TEST);

	shaderBillboard->Use();
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture().Use();

	shaderBillboard->SetVector3("centerPos", GetGameObject().WorldPosition());
	shaderBillboard->SetMatrix("model", Mat4::CreateTranslationMatrix(position));	
	shaderBillboard->SetVector4("color", color);
	shaderBillboard->SetFloat("width", width);
	shaderBillboard->SetFloat("edge", edge);
	shaderBillboard->SetFloat("outlineWidth", outlineWidth);
	shaderBillboard->SetFloat("outlineEdge", outlineEdge);
	shaderBillboard->SetVector3("outlineColor", outlineColor);
	shaderBillboard->SetVector2("offset", offset);

	glBindVertexArray(mesh.GetID());
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);

	if (!depthTest)
		glEnable(GL_DEPTH_TEST);
}

void FontComponent::Draw2D(const Camera& _camera)
{
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!depthTest)
		glDisable(GL_DEPTH_TEST);

	shader2D->Use();
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture().Use();

	shader2D->SetMatrix("projection", Mat4::CreateTranslationMatrix(Vec3{ -1.f + (float)_camera.GetHeight() / (float)_camera.GetWidth() , 0.0f, 0.0f } + position / 100.f)
									* Mat4::CreateScaleMatrix({ (float)_camera.GetHeight() / (float)_camera.GetWidth(), 1.f, 1.f }));
	shader2D->SetVector4("color", color);
	shader2D->SetFloat("width", width);
	shader2D->SetFloat("edge", edge);
	shader2D->SetFloat("outlineWidth", outlineWidth);
	shader2D->SetFloat("outlineEdge", outlineEdge);
	shader2D->SetVector3("outlineColor", outlineColor);
	shader2D->SetVector2("offset", offset);

	glBindVertexArray(mesh.GetID());
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);

	if (!depthTest)
		glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void FontComponent::ChangeText(std::string _newText)
{
	const char* _text = _newText.c_str();
	ChangeText(_text, _newText.size());
}

void FontComponent::ChangeText(const char* _newText, int _size)
{
	if (_size < 1)
		return;
	float curserX = 0.25f;
	float curserY = 0.5f;
	std::vector<Vertex> vertices;
	vertices.reserve(_size * 4);
	std::vector<int> indices;
	indices.reserve(_size * 6);

	int i = 0;
	while (_newText[i] != '\0')
	{
		const int ascii = (int)_newText[i];
		if (ascii == ' ')
		{
			curserX += font->GetMetaData().GetSpaceWidth() * fontSize;
			++i;
			continue;
		}
		if (ascii == '\n')
		{
			curserX = 0.25f;
			curserY += 0.03f * fontSize; // like the other TODO in Font.cpp
			++i;
			continue;
		}
		const Character* character = font->GetMetaData().GetCharacter(ascii);

		const float x = curserX + (character->GetxOffset() * fontSize);
		const float y = curserY + (character->GetyOffset() * fontSize);
		const float maxX = x + (character->GetSizeX() * fontSize);
		const float maxY = y + (character->GetSizeY() * fontSize);
		const float properX = (2.f * x) - 1.f;
		const float properY = (-2.f * y) + 1.f;
		const float properMaxX = (2.f * maxX) - 1.f;
		const float properMaxY = (-2.f * maxY) + 1.f;
		
		indices.push_back(vertices.size());
		indices.push_back(vertices.size() + 2 );
		indices.push_back(vertices.size() + 3 );
		indices.push_back(vertices.size());
		indices.push_back(vertices.size() + 1 );
		indices.push_back(vertices.size() + 2 );

		vertices.push_back(Vertex{ Vec3(properX, properY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetxTextureCoord(), 1 - character->GetyTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properX, properMaxY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetxTextureCoord(), 1 - character->GetYMaxTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properMaxX, properMaxY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetXMaxTextureCoord(), 1 - character->GetYMaxTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properMaxX, properY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetXMaxTextureCoord(), 1 - character->GetyTextureCoord()) });

		curserX += character->GetxAdvance() * fontSize;

		++i;
	}

	if (isGenerated)
		mesh.ChangeSizeAndData(vertices.data(), vertices.size(), indices.data(), indices.size());
	else
		mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());

	isGenerated = true;
}

template <class Archive>
static void FontComponent::load_and_construct(Archive& _ar, cereal::construct<FontComponent>& _construct)
{
	std::string _fontName;
	std::string _fontPath;
	bool _isfontDeletebale;

	_ar(_fontName, _isfontDeletebale, _fontPath);
	_construct(GameObject::currentLoadedGameObject, *ResourcesManager::AddResourceFont(_fontName, _isfontDeletebale, _fontPath.c_str()));
	int _type;
	_ar(_type, _construct->text, _construct->color.x, _construct->color.y, _construct->color.z, _construct->color.w,
		_construct->position.x, _construct->position.y, _construct->position.z,
		_construct->fontSize, _construct->width, _construct->edge, _construct->outlineWidth, _construct->outlineEdge,
		_construct->outlineColor.x, _construct->outlineColor.y, _construct->outlineColor.z, 
		_construct->offset.x, _construct->offset.y, _construct->depthTest);
	_ar(cereal::base_class<Component>(_construct.ptr()));
	_construct->ChangeType((E_FONT_TYPE)_type);
	_construct->ChangeText(_construct->text.c_str(), _construct->text.size());
}