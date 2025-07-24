// tests/json_utils_test.cpp

#include <gtest/gtest.h>
#include "serialization/json/json_document.h"
#include "serialization/json/json_value.h"

using namespace Engine::Serialization::Json;

// 1) JsonDocument starts with an object root
TEST(JsonDocumentTest, RootIsObjectByDefault)
{
    JsonDocument doc;
    JsonValue &root = doc.get_root();

    EXPECT_TRUE(root.is_object());
    EXPECT_FALSE(root.is_array());
    EXPECT_EQ(doc.to_text(false), "{}"); // empty object serializes to "{}"
}

// 2) from_text accepts valid JSON and throws on invalid
TEST(JsonDocumentTest, FromTextValidAndInvalid)
{
    JsonDocument doc(R"({"a": 1, "b": [true,false]})");
    // valid JSON
    EXPECT_TRUE(doc.get_root().has("a"));
    EXPECT_TRUE(doc.get_root().has("b"));

    // invalid JSON
    EXPECT_THROW(JsonDocument incorrect_doc("{ this is not JSON"), std::runtime_error);
}

// 3) to_text respects indentation flag
TEST(JsonDocumentTest, ToTextIndentation)
{
    JsonDocument doc(R"({"x":10,"y":20})");
    std::string no_indent = doc.to_text(false);
    std::string with_indent = doc.to_text(true);

    // Compact form has no newlines or spaces between keys
    EXPECT_EQ(no_indent, R"({"x":10,"y":20})");

    // Indented form contains newlines and 4‑space indents
    EXPECT_NE(with_indent, no_indent);
    EXPECT_NE(with_indent.find("\n    \"x\": 10"), std::string::npos);
}

// 4) Object mutation: set, get, has, operator[]
TEST(JsonValueTest, ObjectSetGetAndHasKey)
{
    JsonDocument doc;
    JsonValue &root = doc.get_root();

    EXPECT_FALSE(root.has("foo"));

    root.set_empty_object("inner");
    root.get("inner").set_empty_array("foo");

    ASSERT_TRUE(root.has("inner"));
    ASSERT_FALSE(root.has("foo"));

    JsonValue fooVal = root.get("inner").get("foo");

    EXPECT_FALSE(fooVal.is_object());
    EXPECT_TRUE(fooVal.is_array());

    EXPECT_TRUE(root.get("inner").has("foo"));
}

// 5) Array mutation: append, size, get(index)
TEST(JsonValueTest, ArrayAppendSizeAndGet)
{
    JsonDocument doc;

    doc.get_root().set("arr", std::vector<int>());

    JsonValue arr = doc.get_root().get("arr");
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_FALSE(arr.is_object());

    // Append a few ints
    for (int i = 0; i < 3; ++i)
    {
        arr.append(i * 10);
    }

    EXPECT_TRUE(arr.is_array());
    EXPECT_EQ(arr.size(), 3u);

    // Check each element
    EXPECT_EQ(arr.get(0).as<int>(), std::optional<int>(0));
    EXPECT_EQ(arr.get(1).as<int>(), std::optional<int>(10));
    EXPECT_EQ(arr.get(2).as<int>(), std::optional<int>(20));

    // Out‑of‑bounds index returns null
    EXPECT_THROW(arr.get(5), std::out_of_range);
}

// 6) Null, object(), array() static factories
TEST(JsonValueTest, NullObjectArrayFactories)
{
    JsonDocument doc;

    JsonValue obj = doc.get_root().set_empty_object("obj");
    JsonValue arr = doc.get_root().set_empty_array("arr");

    EXPECT_TRUE(obj.is_empty());
    EXPECT_TRUE(obj.is_object());
    EXPECT_EQ(obj.size(), 0u); // object has no size()

    EXPECT_TRUE(arr.is_empty());
    EXPECT_TRUE(arr.is_array());
    EXPECT_EQ(arr.size(), 0u);
}

// 7) make_object and make_array overwrite existing node
TEST(JsonValueTest, MakeObjectAndMakeArrayOverwrite)
{
    JsonDocument doc;

    JsonValue obj = doc.get_root().set_empty_object("obj");

    EXPECT_TRUE(obj.is_object());

    doc.get_root().set_empty_array("obj");
    EXPECT_TRUE(obj.is_array());
    EXPECT_EQ(obj.size(), 0u);

    doc.get_root().set_empty_array("obj");
    EXPECT_FALSE(obj.is_object());
}

TEST(JsonValueTest, AccessNonExistentKeysThrowsOrReturnsNull)
{
    JsonDocument doc;
    JsonValue &root = doc.get_root();

    EXPECT_FALSE(root.has("missing_key"));

    // Trying to get non-existent key throws
    EXPECT_THROW(root.get("missing_key"), std::runtime_error);

    // Trying to remove a missing key returns false
    EXPECT_FALSE(root.remove("missing_key"));
}

TEST(JsonValueTest, NestedObjectAndArrayMutation)
{
    JsonDocument doc;
    JsonValue &root = doc.get_root();

    root.set_empty_object("level1");
    JsonValue level1 = root.get("level1");

    level1.set_empty_array("level2_array");
    JsonValue level2_array = level1.get("level2_array");

    // Append objects to array
    level2_array.append(nlohmann::json::object());
    level2_array.append(nlohmann::json::object());

    EXPECT_EQ(level2_array.size(), 2u);

    // Set fields in the first object in array
    JsonValue first_obj = level2_array.get(0);
    first_obj.set("foo", 42);
    first_obj.set("bar", "baz");

    EXPECT_TRUE(first_obj.has("foo"));
    EXPECT_TRUE(first_obj.has("bar"));

    EXPECT_EQ(first_obj.get("foo").as<int>(), std::optional<int>(42));
    EXPECT_EQ(first_obj.get("bar").as<std::string>(), std::optional<std::string>("baz"));
}

TEST(JsonValueTest, CopyConstructorAndAssignment)
{
    JsonDocument doc;
    JsonValue &root = doc.get_root();

    root.set_empty_object("obj");
    JsonValue obj = root.get("obj");
    obj.set("key", 123);

    JsonValue copy1 = obj; // Copy constructor
    JsonValue copy2(root); // New JsonValue to root
    copy2 = obj;           // Assignment operator

    EXPECT_TRUE(copy1.has("key"));
    EXPECT_EQ(copy1.get("key").as<int>(), std::optional<int>(123));

    EXPECT_TRUE(copy2.has("key"));
    EXPECT_EQ(copy2.get("key").as<int>(), std::optional<int>(123));
}

TEST(JsonValueTest, ConstJsonValueAccess)
{
    JsonDocument doc(R"({"foo": 123, "bar": [1,2,3]})");
    const JsonValue &root = doc.get_root();

    EXPECT_TRUE(root.has("foo"));
    EXPECT_EQ(root.get("foo").as<int>(), std::optional<int>(123));

    const JsonValue bar = root.get("bar");
    EXPECT_TRUE(bar.is_array());
    EXPECT_EQ(bar.size(), 3u);
}
