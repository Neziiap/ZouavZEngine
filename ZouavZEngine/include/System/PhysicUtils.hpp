#ifndef PHYSX_UTIL_HPP_
#define PHYSX_UTIL_HPP_


#include "Component/Transform.hpp"
#include "Maths/Quaternion.hpp"
#include "Maths/Vec3.hpp"
#include "Maths/Mat4.hpp"
#include "PxPhysics.h"
#include "foundation/PxMat44.h"

static Vec3 Vec3FromPxVec3(const physx::PxVec3& v) { return { v.x, v.y, v.z }; }
static physx::PxVec3 PxVec3FromVec3(const Vec3& v) { return { v.x, v.y, v.z }; }
static Quaternion QuaternionFromPxQuat(const physx::PxQuat& q) { return Quaternion(q.w, q.x, q.y, q.z); }
static physx::PxQuat PxQuatFromQuaternion(const Quaternion& q) { return physx::PxQuat(q.x, q.y, q.z, q.w); }
static Mat4 Mat4FromPxMat44(const physx::PxMat44& m) { return { m.column0.x, m.column0.y, m.column0.z, m.column0.w, m.column1.x, m.column1.y, m.column1.z, m.column1.w, m.column2.x, m.column2.y, m.column2.z, m.column2.w, m.column3.x, m.column3.y, m.column3.z, m.column3.w }; }
static physx::PxMat44 PxMat44FromMat4(const Mat4& m) { return { {m.matrix[0], m.matrix[1], m.matrix[2], m.matrix[3]}, {m.matrix[4], m.matrix[5], m.matrix[6], m.matrix[7]}, {m.matrix[8], m.matrix[9], m.matrix[10], m.matrix[11]}, {m.matrix[12], m.matrix[13], m.matrix[14], m.matrix[15]} }; }
static Transform TransformFromPxTransform(const physx::PxTransform& t) { return { Vec3FromPxVec3(t.p), QuaternionFromPxQuat(t.q), {1.0f, 1.0f, 1.0f} }; }
static physx::PxTransform PxTransformFromTransformGlobal(const Transform& t) { return { PxVec3FromVec3(t.WorldPosition()), PxQuatFromQuaternion(t.WorldRotation()) }; }
static physx::PxTransform PxTransformFromTransformLocal(const Transform& t) { return { PxVec3FromVec3(t.localPosition), PxQuatFromQuaternion(t.localRotation) }; }

#endif // !1