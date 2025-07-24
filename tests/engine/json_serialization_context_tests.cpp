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
    JSONSerializationContext context(stage);

    context.begin_object(""); // start root object scope

    context.write_UInt(42, "my_uint");

    context.end_object();

    auto root = context.get_result();

    // root should be an object with "my_uint" field = 42
    ASSERT_TRUE(root.is_object());
    EXPECT_EQ(root.get("my_uint").as<int>().value(), 42);
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
