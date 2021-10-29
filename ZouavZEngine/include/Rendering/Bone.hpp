#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Mat4.hpp"
#include "Maths/Quaternion.hpp"
#include "Component/Transform.hpp"
#include <vector>
#include <string>
#include <assimp/anim.h>

struct KeyPosition
{
    Vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    Quaternion orientation;
    float timeStamp;
};

struct KeyScale
{
    Vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;

    int nbPositionsKeys;
    int nbRotationsKeys;
    int nbScalesKeys;

    float GetScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _animationTime);
    Vec3 InterpolatePosition(float _animationTime);
    Quaternion InterpolateRotation(float _animationTime);
    Vec3 InterpolateScaling(float animationTime);

    Vec3 InterpolateVector(Vec3& v1, Vec3& v2, float scalar);


public:
    
    int id;
    std::string name;
    Mat4 localTransform;

    Bone(const std::string& _name, int _id, const aiNodeAnim* channel);
    
    void Update(float _animationTime);
    int GetPositionIndex(float _animationTime);
    int GetRotationIndex(float _animationTime);
    int GetScaleIndex(float _animationTime);

};