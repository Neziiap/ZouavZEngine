#pragma once

#include "GameObject.hpp"
#include <vector>


void RegisterComponents(std::vector<bool (*)(GameObject* _go)>& _addComponentsFunctions);