#include <gtest/gtest.h>
#include "serialization/json/json_serialization_context.h"
#include "serialization/serializer.h"
#include "stage/stage.h"

#include "exceptions/json_key_not_found_exception.h"
#include "exceptions/json_type_mismatch_exception.h"

#include <string>

using namespace Engine::Serialization;
using namespace Engine::Serialization::Json;

class JsonSerializationContextTest : public ::testing::Test
{
protected:
    Engine::Stage *stage = nullptr;

    void SetUp() override
    {
        stage = new Engine::Stage();
    }

    void TearDown() override
    {
        delete stage;
        stage = nullptr;
    }
};

TEST_F(JsonSerializationContextTest, WriteReadPrimitiveValues)
{
    JSONSerializationContext ctx(stage);

    float float_test = 32.56f;
    int int_test = 24;
    uint32_t uint_test = 16u;
    std::string string_test = "Hello, hope this works!";

    ctx.write("float", float_test);
    ctx.write("int", int_test);
    ctx.write("uint", uint_test);
    ctx.write("string", string_test);

    EXPECT_EQ(ctx.read<float>("float"), float_test);
    EXPECT_EQ(ctx.read<int>("int"), int_test);
    EXPECT_EQ(ctx.read<uint32_t>("uint"), uint_test);
    EXPECT_EQ(ctx.read<std::string>("string"), string_test);
}

TEST_F(JsonSerializationContextTest, HasFieldObjectArrayChecks)
{
    JSONSerializationContext ctx(stage);

    // Initially, the root should be an object
    EXPECT_FALSE(ctx.is_primitive()) << "Initially, context is not a field";
    EXPECT_FALSE(ctx.is_array()) << "Initially, context is not an array";
    EXPECT_TRUE(ctx.is_object()) << "Initially, context is an object";

    // Field presence checks
    ctx.write("test_float", 24.3f);

    EXPECT_TRUE(ctx.has_key("test_float")) << "Field 'test_float' was written and should exist";
    EXPECT_FALSE(ctx.has_key("not_test_float")) << "Nonexistent field should not exist";

    // has_object should be false unless field6 is a nested object
    EXPECT_FALSE(ctx.has_object("test_float")) << "Primitive field should not be detected as object";

    // Write nested object
    ctx.begin_object_key("my_obj");
    ctx.write("inner", 123);
    ctx.write("inner2", 246);
    ctx.end_object();

    EXPECT_TRUE(ctx.has_object("my_obj")) << "Nested object should be detected correctly";

    // has_array should be false unless field is a nested array
    EXPECT_FALSE(ctx.has_array("my_obj")) << "Object should not be mistaken for array";

    ctx.begin_object_key("my_obj");
    EXPECT_TRUE(ctx.has_key("inner")) << "Field must not be deleted when entering inside it again";
    EXPECT_EQ(ctx.read<int>("inner"), 123) << "Field value must be protected";

    EXPECT_TRUE(ctx.has_key("inner2")) << "Field must not be deleted when entering inside it again";

    ctx.remove("inner2");
    EXPECT_FALSE(ctx.has_key("inner2")) << "Field must not exist after remove() is called";
    EXPECT_TRUE(ctx.has_key("inner")) << "Other fields should not be deleted after remove() called on its siblings";

    EXPECT_THROW(ctx.end_array(), std::runtime_error) << "Can use end_array() when in object";

    ctx.clear();

    EXPECT_FALSE(ctx.has_key("inner")) << "Fields must be fully erased after clear() is called";

    ctx.end_object();

    // Write nested array
    ctx.begin_array_key("my_array");
    ctx.append(1);
    ctx.append(2);
    ctx.end_array();

    EXPECT_TRUE(ctx.has_array("my_array")) << "Array should be detected correctly";

    // Let's enter the array and check context shape
    ctx.begin_array_key("my_array");

    EXPECT_FALSE(ctx.is_object()) << "Inside array, should not be object";
    EXPECT_TRUE(ctx.is_array()) << "Inside array, should be array";
    EXPECT_FALSE(ctx.is_primitive()) << "Inside array, should not be field";

    ctx.end_array();

    // Check overriding for array and objects
    ctx.begin_object_key("my_array");

    EXPECT_EQ(ctx.size(), 0) << "After overriding, there should be no elements inside the new object";

    ctx.write("test_int", 32);

    ctx.end_object();

    // Switch to array again
    ctx.begin_array_key("my_array");

    EXPECT_EQ(ctx.size(), 0) << "After overriding, there should be no elements inside the new array";

    int max = 10;
    for (int i = 0; i < max; i++)
    {
        ctx.append(i * i);
    }

    EXPECT_EQ(ctx.size(), max) << "Insertion must work as expected after overriding an object";

    ctx.end_array();
}

TEST_F(JsonSerializationContextTest, Objects)
{
    JSONSerializationContext ctx(stage);

    // Check if an empty object is automatically created with ctx
    EXPECT_TRUE(ctx.get_current().is_object());

    ctx.begin_object_key("upper_test_object");
    ctx.write("four_squared", 16);

    EXPECT_FALSE(ctx.has_object("upper_test_object"));
    EXPECT_TRUE(ctx.has_key("four_squared"));

    ctx.end_object();

    EXPECT_TRUE(ctx.has_object("upper_test_object"));
    EXPECT_FALSE(ctx.has_key("four_squared"));

    // Go inside an already existing object
    ctx.begin_object_key("upper_test_object");
    EXPECT_TRUE(ctx.has_key("four_squared"));
    EXPECT_EQ(ctx.read<int>("four_squared"), 16);

    ctx.end_object();
}

TEST_F(JsonSerializationContextTest, BeginObjectOnPrimitiveOverride)
{
    JSONSerializationContext ctx(stage);
    ctx.write("some_int", 42);

    // Try to begin an object inside a primitive field
    ctx.begin_object_key("some_int");

    ctx.write("some_float", 32.56f);
    EXPECT_FLOAT_EQ(ctx.read<float>("some_float"), 32.56f);

    ctx.end_object();

    EXPECT_TRUE(ctx.is_object());
}

TEST_F(JsonSerializationContextTest, BeginArrayOnPrimitiveThrows)
{
    JSONSerializationContext ctx(stage);
    ctx.write("str_field", "hi");

    EXPECT_TRUE(ctx.has_key("str_field"));

    // Try to begin an object inside a primitive field
    ctx.begin_array_key("str_field");

    EXPECT_TRUE(ctx.is_array());

    ctx.append(32.56f);
    ctx.append(76.21f);
    ctx.append(12.26f);

    EXPECT_FLOAT_EQ(ctx.read_at<float>(0), 32.56f);
    EXPECT_FLOAT_EQ(ctx.read_at<float>(1), 76.21f);
    EXPECT_FLOAT_EQ(ctx.read_at<float>(2), 12.26f);

    EXPECT_TRUE(ctx.is_array());

    ctx.end_array();
}

TEST_F(JsonSerializationContextTest, EndObjectWithoutBeginThrows)
{
    JSONSerializationContext ctx(stage);
    EXPECT_THROW(ctx.end_object(), std::runtime_error);
}

TEST_F(JsonSerializationContextTest, EndArrayWithoutBeginThrows)
{
    JSONSerializationContext ctx(stage);
    EXPECT_THROW(ctx.end_array(), std::runtime_error);
}

TEST_F(JsonSerializationContextTest, ArrayOfObjects)
{
    JSONSerializationContext ctx(stage);

    ctx.begin_array_key("object_list");

    for (int i = 0; i < 3; i++)
    {
        ctx.begin_object_push();
        ctx.write("index", i);
        ctx.write("string_test", "Object Name: " + i);
        ctx.end_object();
    }

    ctx.end_array();

    // Read back
    ctx.begin_array_key("object_list");

    for (int i = 0; i < 3; i++)
    {
        ctx.begin_object_index(i);
        EXPECT_EQ(ctx.read<int32_t>("index"), i);
        ctx.end_object();
    }

    ctx.end_array();
}

TEST_F(JsonSerializationContextTest, OverwriteFields)
{
    JSONSerializationContext ctx(stage);
    ctx.write("duplicate", 10);
    ctx.write("duplicate", 20);

    EXPECT_EQ(ctx.read<int32_t>("duplicate"), 20);
}

TEST_F(JsonSerializationContextTest, EmptyStringRoundtrip)
{
    JSONSerializationContext ctx(stage);
    std::string empty = "";
    ctx.write("empty", empty);
    EXPECT_EQ(ctx.read<std::string>("empty"), "");
}

TEST_F(JsonSerializationContextTest, ArrayOfPrimitives)
{
    JSONSerializationContext ctx(stage);
    ctx.begin_array_key("nums");
    for (int i = 0; i < 5; ++i)
        ctx.append(i * 2);

    EXPECT_EQ(ctx.size(), 5u);
    ctx.end_array();

    ctx.begin_array_key("nums");
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(ctx.read_at<int>(i), i * 2);
    ctx.end_array();
}

TEST_F(JsonSerializationContextTest, DeeplyNested)
{
    JSONSerializationContext ctx(stage);
    ctx.begin_object_key("level1");
    ctx.begin_array_key("level2");
    for (int i = 0; i < 3; ++i)
    {
        ctx.begin_object_push();
        ctx.write("value", i);
        ctx.end_object();
    }
    ctx.end_array();
    ctx.end_object();

    // Read back
    ctx.begin_object_key("level1");
    ctx.begin_array_key("level2");
    for (int i = 0; i < 3; ++i)
    {
        ctx.begin_object_index(i);
        EXPECT_EQ(ctx.read<int>("value"), i);
        ctx.end_object();
    }
    ctx.end_array();
    ctx.end_object();
}

TEST_F(JsonSerializationContextTest, ReadWrongTypeThrows)
{
    JSONSerializationContext ctx(stage);

    ctx.write("num", 123);

    EXPECT_THROW(ctx.read<std::string>("num"), Engine::Exceptions::JsonTypeMismatchException);
}

TEST_F(JsonSerializationContextTest, OverwriteWithDifferentType)
{
    JSONSerializationContext ctx(stage);

    ctx.write("key", "hello");
    ctx.write("key", 42);

    EXPECT_EQ(ctx.read<int>("key"), 42);
    EXPECT_THROW(ctx.read<std::string>("key"), Engine::Exceptions::JsonTypeMismatchException);
}

TEST_F(JsonSerializationContextTest, EmptyArrayAndObject)
{
    JSONSerializationContext ctx(stage);
    ctx.begin_array_key("arr");
    ctx.end_array();

    EXPECT_TRUE(ctx.has_array("arr"));
    EXPECT_EQ(ctx.size(), 1u);

    ctx.begin_object_key("obj");
    ctx.end_object();

    EXPECT_TRUE(ctx.has_object("obj"));
    EXPECT_EQ(ctx.size(), 2u);
}