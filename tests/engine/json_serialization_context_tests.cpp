#include <gtest/gtest.h>
#include "serialization/json/json_serialization_context.h"
#include "stage/stage.h"

using namespace Engine;

class JsonSerializationContextTest : public ::testing::Test
{
protected:
    Stage *stage = nullptr;
};

TEST(JsonSerializationContextTest, RootObjectCreatedAndUIntWritten)
{
    Stage dummy_stage;
    JSONSerializationContext ctx(&dummy_stage);

    auto &root = ctx.get_result();
    EXPECT_TRUE(root.is_object());

    ctx.write_int(10, "outer_int");
    EXPECT_EQ(ctx.read_int("outer_int"), 10);

    ctx.begin_object("test_object");

    const std::string field_name = "inner_uint";
    uint32_t value_to_write = 42u;
    ctx.write_UInt(value_to_write, field_name);

    EXPECT_EQ(ctx.read_int("inner_uint"), value_to_write);

    ctx.end_object();

    auto result_value = root
                            .get("test_object")
                            .get(field_name)
                            .get_int();

    ASSERT_TRUE(result_value.has_value());
    EXPECT_EQ(result_value.value(), static_cast<int>(value_to_write));
}

TEST(JsonSerializationContextTest, WriteAndReadUInt)
{
    JSONSerializationContext ctx(nullptr);

    ctx.write_UInt(42u, "answer");
    auto &json = ctx.get_result();

    EXPECT_TRUE(json.is_object());
    EXPECT_EQ(json.get("answer").get_int().value(), 42u);

    JSONSerializationContext read_ctx(nullptr, std::move(json));
    EXPECT_EQ(read_ctx.read_UInt("answer"), 42u);
}