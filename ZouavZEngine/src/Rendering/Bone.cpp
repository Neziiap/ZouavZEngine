#include "Rendering/Bone.hpp"
#include "System/Debug.hpp"
#include "Component/Transform.hpp"

Bone::Bone(const std::string& _name, int _id, const aiNodeAnim* channel):
    name(_name),
    id(_id),
    localTransform(Mat4::identity)
{
    positions.clear();
    rotations.clear();
    scales.clear();
    nbPositionsKeys = channel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < nbPositionsKeys; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        positions.emplace_back(KeyPosition{ { aiPosition.x, aiPosition.y, aiPosition.z }, timeStamp });
    }

    nbRotationsKeys = channel->mNumRotationKeys;

    for (int rotationIndex = 0; rotationIndex < nbRotationsKeys; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        rotations.emplace_back(KeyRotation{ {aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z}, timeStamp });
    }

    nbScalesKeys = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < nbScalesKeys; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = channel->mScalingKeys[keyIndex].mTime;
        scales.emplace_back(KeyScale{ { scale.x, scale.y, scale.z }, timeStamp });
    }
}

void Bone::Update(float _animationTime)
{
    Vec3 translation    = InterpolatePosition(_animationTime);
    Quaternion rotation = InterpolateRotation(_animationTime);
    Vec3 scale          = InterpolateScaling(_animationTime);

    localTransform = Mat4::CreateTRSMatrix(translation, rotation, scale);
}

int Bone::GetPositionIndex(float _animationTime)
{
    for (int index = 0; index < nbPositionsKeys - 1; ++index)
    {
        if (_animationTime < positions[index + 1].timeStamp)
            return index;
    }
    
    Debug::LogError("No position found");
}

int Bone::GetRotationIndex(float _animationTime)
{
    for (int index = 0; index < nbRotationsKeys - 1; ++index)
    {
        if (_animationTime < rotations[index + 1].timeStamp)
            return index;
    }

    Debug::LogError("No rotation found in bone ");
}


int Bone::GetScaleIndex(float _animationTime)
{
    for (int index = 0; index < nbScalesKeys - 1; ++index)
    {
        if (_animationTime < scales[index + 1].timeStamp)
            return index;
    }

    Debug::LogError("No scale found in bone ");
}

float Bone::GetScaleFactor(float _lastTimeStamp, float _nextTimeStamp, float _animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = _animationTime - _lastTimeStamp;
    float framesDiff = _nextTimeStamp - _lastTimeStamp;

    scaleFactor = midWayLength / framesDiff;

    return scaleFactor;
}

Vec3 Bone::InterpolateVector(Vec3& v1, Vec3& v2, float scalar)
{
    return v1 * (1.0f - scalar) + v2 * scalar;
}

Vec3 Bone::InterpolatePosition(float _animationTime)
{
    if (nbPositionsKeys == 1)
        return positions[0].position;

    int p0Index = GetPositionIndex(_animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, _animationTime);
    Vec3 finalPosition = InterpolateVector(positions[p0Index].position, positions[p1Index].position, scaleFactor);

    return finalPosition;
}

Quaternion Bone::InterpolateRotation(float _animationTime)
{
    if (nbRotationsKeys == 1)
        return rotations[0].orientation.Normalised();

    int p0Index = GetRotationIndex(_animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, _animationTime);

    Quaternion finalRotation = Quaternion::SLerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);

    return finalRotation.Normalised();
}

Vec3 Bone::InterpolateScaling(float animationTime)
{
    if (nbScalesKeys == 1)
        return scales[0].scale;

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, animationTime);
    Vec3 finalScale = InterpolateVector(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);

    return finalScale;
}