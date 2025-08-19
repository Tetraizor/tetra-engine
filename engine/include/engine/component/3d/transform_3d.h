#pragma once

#include "engine/component/component.h"
#include "engine/entity/entity.h"
#include "engine/math/vector3.h"
#include "engine/math/quaternion.h"
#include "engine/math/matrix4.h"

namespace Engine
{
    using namespace Math;

    class Transform3D : public Component
    {
    public:
        Transform3D() = default;
        ~Transform3D() = default;

        Vector3 get_position() { return position; }
        void set_position(const Vector3 &position) { this->position = position; }

        Vector3 get_rotation_radians() { return rotation_radians; }
        void set_rotation_radians(const Vector3 &rotation_degrees) { this->rotation_radians = rotation_degrees; }

        Quaternion get_rotation() { return rotation; }
        void set_rotation(const Quaternion &q) { rotation = q; }

        void translate(const Vector3 &delta);
        void rotate_degrees(const Vector3 &delta);

        Vector3 get_forward() const;
        Vector3 get_up() const;
        Vector3 get_right() const;

        Matrix4 get_local_matrix() const;

        void serialize(Serialization::SerializationContext &ctx) const override;
        void deserialize(Serialization::SerializationContext &ctx) override;

    private:
        Vector3 position;
        Vector3 rotation_radians;

        Quaternion rotation;
    };
}

REGISTER_COMPONENT(Transform3D, Engine::Transform3D)
