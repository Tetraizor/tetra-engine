#include "engine/component/component_registry.h"
#include "engine/component/3d/transform_3d.h"

namespace Engine
{
    void Transform3D::translate(const Vector3 &delta)
    {
        position.x += delta.x;
        position.y += delta.y;
        position.z += delta.z;
    }

    void Transform3D::rotate_degrees(const Vector3 &delta)
    {
        rotation_radians.x += delta.x;
        rotation_radians.y += delta.y;
        rotation_radians.z += delta.z;

        auto wrap_angle = [](float &a)
        {
            if (a > 360.0f || a < -360.0f)
            {
                a = std::fmod(a, 360.0f);
            }
        };

        wrap_angle(rotation_radians.x);
        wrap_angle(rotation_radians.y);
        wrap_angle(rotation_radians.z);
    }
    void Transform3D::serialize(Serialization::SerializationContext &ctx) const
    {
        Component::serialize(ctx);

        ctx.write("x", position.x);
        ctx.write("y", position.y);
        ctx.write("z", position.z);
    }

    Vector3 Transform3D::get_forward() const
    {
        Quaternion q = Quaternion::from_euler(rotation_radians.x, rotation_radians.y, rotation_radians.z);

        Vector3 local_forward = Vector3(0.0f, 0.0f, -1.0f);
        Vector3 world_forward = q * local_forward;

        return world_forward.normalized();
    }

    Vector3 Transform3D::get_up() const
    {
        Quaternion q = Quaternion::from_euler(rotation_radians.x, rotation_radians.y, rotation_radians.z);

        Vector3 local_up = Vector3(0.0f, 1.0f, 0.0f);
        Vector3 world_up = q * local_up;

        return world_up.normalized();
    }

    Vector3 Transform3D::get_right() const
    {
        Vector3 euler_rad = rotation_radians;
        Quaternion q = Quaternion::from_euler(rotation_radians.x, rotation_radians.y, rotation_radians.z);

        Vector3 local_right = Vector3(1.0f, 0.0f, 0.0f);
        Vector3 world_right = q * local_right;

        return world_right.normalized();
    }

    Matrix4 Transform3D::get_local_matrix() const
    {
        Matrix4 t = Matrix4::identity().translate(position);

        Vector3 euler_rad = rotation_radians;
        Quaternion q = Quaternion::from_euler(euler_rad.x, euler_rad.y, euler_rad.z);
        Matrix4 r = Matrix4::from_quaternion(q);

        Matrix4 m = t * r;
        return m;
    }

    void Transform3D::deserialize(Serialization::SerializationContext &ctx)
    {
        Component::deserialize(ctx);

        position.x = ctx.read<float>("x");
        position.y = ctx.read<float>("y");
        position.z = ctx.read<float>("z");
    }
}
