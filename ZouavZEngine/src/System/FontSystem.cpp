#include "Component/FontComponent.hpp"
#include "System/FontSystem.hpp"

std::vector<FontComponent*> FontSystem::fonts3D;
std::vector<FontComponent*> FontSystem::fontsBillboard;
std::vector<FontComponent*> FontSystem::fonts2D;

void FontSystem::AddFont3D(FontComponent* _font)
{
	fonts3D.push_back(_font);
}

void FontSystem::RemoveFont3D(FontComponent* _font)
{
	for (auto it = fonts3D.begin(); it != fonts3D.end(); )
	{
		if (*it == _font)
		{
			fonts3D.erase(it);
			return;
		}
		else
			++it;
	}
}
void FontSystem::AddFontBillboard(FontComponent* _font)
{
	fontsBillboard.push_back(_font);
}

void FontSystem::RemoveFontBillboard(FontComponent* _font)
{
	for (auto it = fontsBillboard.begin(); it != fontsBillboard.end(); )
	{
		if (*it == _font)
		{
			fontsBillboard.erase(it);
			return;
		}
		else
			++it;
	}
}void FontSystem::AddFont2D(FontComponent* _font)
{
	fonts2D.push_back(_font);
}

void FontSystem::RemoveFont2D(FontComponent* _font)
{
	for (auto it = fonts2D.begin(); it != fonts2D.end(); )
	{
		if (*it == _font)
		{
			fonts2D.erase(it);
			return;
		}
		else
			++it;
	}
}

void FontSystem::DrawFont3D(const Camera& _camera)
{
	for (auto& font3D : fonts3D)
		if (font3D->IsActive())
			font3D->Draw3D(_camera);
}

void FontSystem::DrawFontBillboard(const Camera& _camera)
{
	for (auto& fontBillboard : fontsBillboard)
		if (fontBillboard->IsActive())
			fontBillboard->DrawBillboard(_camera);
}

void FontSystem::DrawFont2D(const Camera& _camera)
{
	for (auto& font2D : fonts2D)
		if (font2D->IsActive())
			font2D->Draw2D(_camera);
}

void FontSystem::DrawFonts(const Camera& _camera)
{
	DrawFont3D(_camera);
	DrawFontBillboard(_camera);
	DrawFont2D(_camera);
}