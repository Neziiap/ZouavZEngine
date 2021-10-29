#pragma once

#include <vector>


class FontSystem
{
private:
	static std::vector<class FontComponent*> fonts3D;
	static std::vector<class FontComponent*> fontsBillboard;
	static std::vector<class FontComponent*> fonts2D;

public:
	FontSystem() = delete;
	~FontSystem() = delete;

	static void AddFont3D(class FontComponent* _script);
	static void RemoveFont3D(class FontComponent* _script);
	static void AddFontBillboard(class FontComponent* _script);
	static void RemoveFontBillboard(class FontComponent* _script);
	static void AddFont2D(class FontComponent* _script);
	static void RemoveFont2D(class FontComponent* _script);

	static void DrawFont3D(const class Camera& _camera);
	static void DrawFontBillboard(const class Camera& _camera);
	static void DrawFont2D(const class Camera& _camera);

	static void DrawFonts(const class Camera& _camera);
};