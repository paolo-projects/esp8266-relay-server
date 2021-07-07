#ifndef TEST_H
#define TEST_H

#define _TEST_ENV

#include <unity.h>
#include <string>
#include <random>
#include <string>
#include <sstream>
#include <unity.h>
#include "test/mocks.h"
#include "Map.h"
#include "SerialMap.h"

void test_serialization_deserialization();
void test_stream_read_write();
void test_errors();
void fillWithRandom(char *buffer, size_t size);
void test_map();

#endif // TEST_H