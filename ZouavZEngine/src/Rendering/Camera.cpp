#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "imgui.h"
#include "Rendering/Camera.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Camera* Camera::mainCamera = nullptr;
SceneCamera* SceneCamera::sceneCamera = nullptr;

Camera::Camera(class GameObject* _gameObject, int _width, int _height, bool _sceneCamera, std::string _name)
    : Component(_gameObject, _name), width{_width}, height{_height}
{
    if (!mainCamera && !_sceneCamera)
    {
        mainCamera = this;
        isMainCamera = true;
    }

    target = { 0.0f, 0.0f, 0.0f };
    position = { 0.0f, 0.0f, -5.0f };
    projection = Mat4::CreatePerspectiveProjectionMatrix((float)_width, (float)_height, near, far, fov);
}

Camera::~Camera()
{
    isMainCamera = false;
    if (mainCamera == this)
        mainCamera = nullptr;
}

void Camera::Editor()
{
    bool changeMainCamera = isMainCamera;
    if (ImGui::Checkbox("Main Camera", &changeMainCamera))
    {
        if (!isMainCamera)
        {
            if (mainCamera)
                mainCamera->isMainCamera = false;
            isMainCamera = true;
            mainCamera = this;
        }
        else
        {
            mainCamera = nullptr;
            isMainCamera = false;
        }
    }
    if (ImGui::SliderFloat("Near", &near, 0.001f, 1.0f)) Resize(width, height);
    if (ImGui::SliderFloat("Far", &far, 1.0f, 10000.0f)) Resize(width, height);
    if (ImGui::SliderFloat("Fov", &fov, 0.1f, 180.0f)) Resize(width, height);
    ImGui::DragFloat3("Postion", &position.x, 0.1f, -1000.f, 1000.f);
    ImGui::DragFloat3("Target", &target.x, 0.1f, -1000.f, 1000.f);
    ImGui::Checkbox("Follow GameObject rotation", &followGameObjectRotation);
}

Mat4 Camera::GetMatrix() const
{
    Vec3 pos;
    Vec3 targetPos;

    if (followGameObjectRotation)
    {
        pos = GetGameObject().GetTRSMatrix() * position;
        targetPos = GetGameObject().GetTRSMatrix() * target;
    }
    else
    {
        pos = GetGameObject().WorldPosition() + position;
        targetPos = GetGameObject().WorldPosition() + target;
    }

    const Vec3 forward = (targetPos - pos).Normalized();
    const Vec3 right = (forward.Cross(Vec3::up)).Normalized();
    const Vec3 up = right.Cross(forward);

    Mat4 cameraMatrix;

    cameraMatrix.Accessor(0, 0) = right.x;
    cameraMatrix.Accessor(0, 1) = right.y;
    cameraMatrix.Accessor(0, 2) = right.z;
    cameraMatrix.Accessor(1, 0) = up.x;
    cameraMatrix.Accessor(1, 1) = up.y;
    cameraMatrix.Accessor(1, 2) = up.z;
    cameraMatrix.Accessor(2, 0) = -forward.x;
    cameraMatrix.Accessor(2, 1) = -forward.y;
    cameraMatrix.Accessor(2, 2) = -forward.z;

    cameraMatrix.Accessor(0, 3) = -right.Dot(pos);
    cameraMatrix.Accessor(1, 3) = -up.Dot(pos);
    cameraMatrix.Accessor(2, 3) = forward.Dot(pos);

    cameraMatrix.Accessor(3, 3) = 1;

    return cameraMatrix.Reversed();
}

void Camera::Resize(int _width, int _height)
{
    width = _width;
    height = _height;
    projection = Mat4::CreatePerspectiveProjectionMatrix((float)_width, (float)_height, near, far, fov);
}

template <class Archive>
static void Camera::load_and_construct(Archive& _ar, cereal::construct<Camera>& _construct)
{
    Mat4 _projection;
    Vec3 _position;
    Vec3 _target;
    bool _wasMainCamera = false;
    _ar(_projection.matrix[0], _projection.matrix[1], _projection.matrix[2], _projection.matrix[3],
        _projection.matrix[4], _projection.matrix[5], _projection.matrix[6], _projection.matrix[7],
        _projection.matrix[8], _projection.matrix[9], _projection.matrix[10], _projection.matrix[11],
        _projection.matrix[12], _projection.matrix[13], _projection.matrix[14], _projection.matrix[15],
        _position.x, _position.y, _position.z,
        _target.x, _target.y, _target.z, _wasMainCamera);
    _construct(GameObject::currentLoadedGameObject, 10, 10);
    _ar(cereal::base_class<Component>(_construct.ptr()));
    _construct->projection = _projection;
    _construct->position = _position;
    _construct->target = _target;
    if (_wasMainCamera)
        _construct->SetMainCamera();
}

SceneCamera::SceneCamera(int _width, int _height)
    : Camera(nullptr, _width, _height, true, ""), pitch(0.0f), yaw(0.0f), speed{ 30.0f }
{
    if (!sceneCamera)
        sceneCamera = this;
}

float SceneCamera::Speed() const
{
    return speed;
}

void SceneCamera::MoveTo(const Vec3& _direction)
{
    position += Vec3((sin(yaw) / 10) * _direction.z + (cos(yaw) / 10) * _direction.x, _direction.y / 10 - (sin(pitch) / 10) * _direction.z, (cos(yaw) / 10) * _direction.z - (sin(yaw) / 10) * _direction.x);
}

Mat4 SceneCamera::GetMatrix() const
{
    return Mat4::CreateTRSMatrix(position, { pitch, yaw, 0 }, { 1, 1, 1 });
}