#include "Prefab.hpp"
#include "GameObject.hpp"
#include "System/Debug.hpp"
#include "imgui.h"

#include <cereal/types/string.hpp>
#include "cereal/access.hpp"
#include <fstream>
#include <iostream>


void Prefab::Editor(const char* _label)
{
    ImGui::Text(_label);
    ImGui::SameLine();
    ImGui::Text(path.c_str());
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in moving file in hierarchy");
            path = *(const std::string*)payload->Data;
            if (path.find(".zepref") != std::string::npos)
                gameobject = GameObject::LoadPrefab(path);
        }
        ImGui::EndDragDropTarget();
    }
}

