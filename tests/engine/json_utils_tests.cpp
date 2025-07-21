// tests/json_utils_test.cpp

#include <gtest/gtest.h>
#include "serialization/json/json_utils.h"

using namespace Engine::Serialization::Json;

// 1) JsonDocument starts with an object root
TEST(JsonDocumentTest, RootIsObjectByDefault)
{
    JsonDocument doc;
    JsonValue &root = doc.root();
    EXPECT_TRUE(root.is_object()); // should be an object
    EXPECT_FALSE(root.is_array());
    EXPECT_EQ(doc.to_text(false), "{}"); // empty object serializes to "{}"
}

// 2) from_text accepts valid JSON and throws on invalid
TEST(JsonDocumentTest, FromTextValidAndInvalid)
{
    JsonDocument doc;
    // valid JSON
    EXPECT_NO_THROW(doc.from_text(R"({"a": 1, "b": [true,false]})"));
    EXPECT_TRUE(doc.root().has_key("a"));
    EXPECT_TRUE(doc.root().has_key("b"));

    // invalid JSON
    EXPECT_THROW(doc.from_text("{ this is not JSON"), std::runtime_error);
}

// 3) to_text respects indentation flag
TEST(JsonDocumentTest, ToTextIndentation)
{
    JsonDocument doc;
    doc.from_text(R"({"x":10,"y":20})");
    std::string no_indent = doc.to_text(false);
    std::string with_indent = doc.to_text(true);

    // Compact form has no newlines or spaces between keys
    EXPECT_EQ(no_indent, R"({"x":10,"y":20})");

    // Indented form contains newlines and 4‑space indents
    EXPECT_NE(with_indent, no_indent);
    EXPECT_NE(with_indent.find("\n    \"x\": 10"), std::string::npos);
}

// 4) Object mutation: set, get, has_key, operator[]
TEST(JsonValueTest, ObjectSetGetAndHasKey)
{
    JsonDocument doc;
    JsonValue &root = doc.root();

    // Start empty
    EXPECT_FALSE(root.has_key("foo"));

    // Set a child object under "foo"
    JsonValue bar = JsonValue::object();
    bar.set("inner", JsonValue::array());
    root.set("foo", bar);

    // has_key and get() should now work
    ASSERT_TRUE(root.has_key("foo"));
    JsonValue fooVal = root.get("foo");
    EXPECT_TRUE(fooVal.is_object());
    EXPECT_TRUE(fooVal.has_key("inner"));

    // operator[] is alias for get()
    EXPECT_TRUE(root["foo"].has_key("inner"));
}

// 5) Array mutation: append, size, get(index)
TEST(JsonValueTest, ArrayAppendSizeAndGet)
{
    JsonValue arr = JsonValue::array();
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_FALSE(arr.is_object());

    // Append a few ints
    for (int i = 0; i < 3; ++i)
    {
        JsonValue v;
        v.set_int(i * 10);
        arr.append(v);
    }

    EXPECT_TRUE(arr.is_array());
    EXPECT_EQ(arr.size(), 3u);

    // Check each element
    EXPECT_EQ(arr.get(0).get_int(), std::optional<int>(0));
    EXPECT_EQ(arr.get(1).get_int(), std::optional<int>(10));
    EXPECT_EQ(arr.get(2).get_int(), std::optional<int>(20));

    // Out‑of‑bounds index returns null
    EXPECT_TRUE(arr.get(5).is_null());
}

// 6) Primitive setters/getters: string, int, float, bool
TEST(JsonValueTest, PrimitiveSetGet)
{
    // String
    JsonValue s;
    s.set_string("hello");
    EXPECT_TRUE(s.is_string());
    EXPECT_EQ(s.get_string(), std::optional<std::string>("hello"));

    // Int
    JsonValue i;
    i.set_int(-42);
    EXPECT_TRUE(i.is_int());
    EXPECT_EQ(i.get_int(), std::optional<int>(-42));

    // Float
    JsonValue f;
    f.set_float(3.14f);
    EXPECT_TRUE(f.is_float());
    EXPECT_NEAR(f.get_float().value(), 3.14f, 1e-6);

    // Bool
    JsonValue b;
    b.set_bool(true);
    EXPECT_TRUE(b.is_bool());
    EXPECT_EQ(b.get_bool(), std::optional<bool>(true));
}

// 7) Null, object(), array() static factories
TEST(JsonValueTest, NullObjectArrayFactories)
{
    JsonValue n = JsonValue::null();
    EXPECT_TRUE(n.is_null());

    JsonValue o = JsonValue::object();
    EXPECT_TRUE(o.is_object());
    EXPECT_EQ(o.size(), 0u); // object has no size()

    JsonValue a = JsonValue::array();
    EXPECT_TRUE(a.is_array());
    EXPECT_EQ(a.size(), 0u);
}

// 8) make_object and make_array overwrite existing node
TEST(JsonValueTest, MakeObjectAndMakeArrayOverwrite)
{
    // Start as null, turn into object
    JsonValue v = JsonValue::null();
    v.make_object();
    EXPECT_TRUE(v.is_object());

    // Fill with array, then make_object again
    v.make_array();
    EXPECT_TRUE(v.is_array());
    EXPECT_EQ(v.size(), 0u);
    v.make_object();
    EXPECT_TRUE(v.is_object());
}
