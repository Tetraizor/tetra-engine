#include <gtest/gtest.h>
#include "serialization/json/json_serialization_context.h"
#include "stage/stage.h"

using namespace Engine::Serialization::Json;
using namespace Engine;

class JsonSerializationContextTest : public ::testing::Test
{
protected:
    Stage *stage = nullptr;

    void SetUp() override
    {
        stage = new Stage();
    }

    void TearDown() override
    {
        delete stage;
        stage = nullptr;
    }
};

TEST_F(JsonSerializationContextTest, WriteUInt_WritesValueToJson)
{
    JSONSerializationContext ctx(stage);

    ctx.write_UInt(35u, "uint_test");

    ctx.begin_object("child_test");

    ctx.write_float(24.353f, "float_test");

    ctx.end_object();

    ctx.write_string("hi :)", "string_test");

    ctx.begin_array("nums");
    ctx.append<int>(3);
    ctx.append<int>(6);
    ctx.append<int>(9);

    auto root = ctx.get_result();
    std::cout << root.to_text(true);

    ASSERT_TRUE(root.is_object());
    EXPECT_EQ(root.get("uint_test").as<uint32_t>().value(), 35u);
}

TEST_F(JsonSerializationContextTest, WriteUInt_OnNonObjectParent_Throws)
{
    JSONSerializationContext context(stage);

    // By default, the root node is empty, let's push an array node manually
    context.begin_array("");

    // Now try to write uint in an array parent - should throw
    EXPECT_THROW(context.write_UInt(123, "field"), std::runtime_error);

    context.end_array();
}
