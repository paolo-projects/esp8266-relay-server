#define _TEST_ENV

#include "tests.h"
#include <iostream>
#include <string>
#include "SerialMap.h"
#include "Map.h"

void test_serialization();
void test_map();

int main(int argc, char **argv)
{
    test_serialization();
    test_map();

    TEST_SUCCESS();

    return 0;
}

void test_serialization()
{
    SerialMap<std::string, 10> map;
    map.put("key", "value");

    char buffer[512];
    char good[12] = {
        0x10, 3, 'k', 'e', 'y', 0x11, 5, 'v', 'a', 'l', 'u', 'e'};

    auto res = map.serialize(buffer, 512);

    TEST_CASE("SerialMap serialization of { \"key\": \"value\" }");

    TEST(std::equal(buffer, buffer + res, good));

    TEST_CASE("SerialMap deserialization of { \"will\": \"it\", \"truly\": \"work?\"}");

    char data[] = {0x10, 4, 'w', 'i', 'l', 'l', 0x11, 2, 'i', 't', 0x10, 5, 't', 'r', 'u', 'l', 'y', 0x11, 5, 'w', 'o', 'r', 'k', '?'};

    SerialMap<std::string, 10> parsed(data, sizeof(data));

    TEST(parsed.has("will"));
    TEST(parsed.has("truly"));

    TEST(*parsed.get("will") == "it");
    TEST(*parsed.get("truly") == "work?");
}

void test_map()
{
    Map<int, std::string, 3> map;

    TEST_CASE("Should succeed setting the entries when below the size limit");

    TEST(map.put(45, "45"));
    TEST(map.put(69, "69"));
    TEST(map.put(90, "90"));

    TEST_CASE("Map should contain the saved entries");

    TEST(map.has(45));
    TEST(map.has(69));
    TEST(map.has(90));

    TEST_CASE("Map should not contain other random stuff")

    TEST(!map.has(666));
    TEST(!map.has(123));
    TEST(!map.has(6969));

    TEST_CASE("Map values should match the ones set previously");

    TEST(*map.get(45) == "45");
    TEST(*map.get(69) == "69");
    TEST(*map.get(90) == "90");

    TEST_CASE("Accessing a random key should give a nullptr back");

    TEST(map.get(666) == nullptr);

    TEST_CASE("Putting an entry when the map is full sould return false and the key should not exist");

    TEST(!map.put(666, "fail"));
    TEST(!map.has(666));
}