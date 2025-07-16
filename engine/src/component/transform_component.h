#include "component.h"
#include "entity/entity.h"

namespace Engine
{
    class TransformComponent : public Component
    {
    public:
        float x = 0.0f, y = 0.0f;

        void serialize(SerializationContext &ctx) const override
        {
            ctx.write_float(x, "x");
            ctx.write_float(y, "y");
        }

        void deserialize(SerializationContext &ctx) override
        {
            x = ctx.read_float("x");
            y = ctx.read_float("y");
        }

        void update(float delta_time) override {}
        void setup() override;
    };
}