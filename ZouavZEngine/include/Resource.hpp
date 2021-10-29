#pragma once

#include <string>
#include <vector>

class Resource
{
protected:
	friend class ResourcesManager;
	std::string name;
	std::string path;
	bool deletable = true;
	bool hasToBeDelete = false;

public:

	Resource() = default;
	Resource(const std::string& _name, const char* _path) : name(_name), path{ _path }{}
	virtual ~Resource() = default;

	virtual void RemoveFromResourcesManager() = 0;

	bool IsDeletable() { return deletable; }

	const std::string& GetPath() { return path; }
	const std::string& GetName() { return name; }
};
