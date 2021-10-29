#pragma once

#include <string>

class Object
{
protected:
	std::string name;
	std::string tag;

public:
	Object(const std::string& _name, const std::string& _tag = "") : name{ _name }, tag{ _tag } {}
	virtual ~Object() = default;

	const std::string& GetName() const { return name; }
	void SetName(const std::string& _newName) { name = _newName; }
	const std::string& GetTag() const { return tag; }
	void SetTag(const std::string& _newTag) { tag = _newTag; }
};