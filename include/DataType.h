#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

enum DATATYPE
{
    DATA_8U,
    DATA_16U,
    DATA_32U,
    DATA_STR
};

class DataType
{
public:
    DataType(DATATYPE type) : type(type) {}
    DATATYPE getType() const { return type; }
    virtual ~DataType() = default;

private:
    DATATYPE type;
};

class DataType8U : public DataType
{
public:
    DataType8U() : DataType(DATA_8U) {}
    DataType8U(uint8_t data) : DataType(DATA_8U), value(data) {}
    void setData(uint8_t data) { value = data; }
    uint8_t getData() const { return value; }

private:
    uint8_t value;
};

class DataType16U : public DataType
{
public:
    DataType16U() : DataType(DATA_16U) {}
    DataType16U(uint16_t data) : DataType(DATA_16U), value(data) {}
    void setData(uint16_t data) { value = data; }
    uint16_t getData() const { return value; }

private:
    uint16_t value;
};

class DataType32U : public DataType
{
public:
    DataType32U() : DataType(DATA_32U) {}
    DataType32U(uint32_t data) : DataType(DATA_32U), value(data) {}
    void setData(uint32_t data) { value = data; }
    uint32_t getData() const { return value; }

private:
    uint32_t value;
};

class DataTypeSTR : public DataType
{
public:
    DataTypeSTR() : DataType(DATA_STR) {}
    DataTypeSTR(const char *data, size_t len) : DataType(DATA_STR), length(min(len, (size_t)256))
    {
        value = new char[length];
        memcpy(value, data, length);
    }
    ~DataTypeSTR()
    {
        if (value != nullptr)
        {
            delete[] value;
        }
    }
    void setData(const char *data, size_t len)
    {
        if (value != nullptr)
        {
            delete[] value;
        }
        length = min(len, (size_t)256);
        value = new char[length];
        memcpy(value, data, length);
    }
    const char *getData() const { return value; }
    size_t getLength() const { return length; }

private:
    char *value = nullptr;
    size_t length;
};

#endif