#include "PacketParser.h"

PacketParser::PacketParser(const std::vector<DataType *> &dataTypes)
    : dataTypes(dataTypes)
{
    /*
        2 bytes for control characters: PI
        1 byte for 8U, 2 for 16U, 4 for 32U
        for STR, 2 bytes for length definition + the actual length of the string
    */
    /*
    expectedSize = 2;
    for (DataType *dataType : dataTypes)
    {
        switch (dataType->getType())
        {
        case DATA_8U:
            expectedSize += 1;
            break;
        case DATA_16U:
            expectedSize += 2;
            break;
        case DATA_32U:
            expectedSize += 4;
            break;
        case DATA_STR:
            expectedSize += 2 + 256; // We account for the max length a string can have
            break;
        }
    }
    */
}

PacketParser::PacketParser(std::vector<DataType *> &&dataTypes)
    : dataTypes(dataTypes)
{
}

PacketParser::~PacketParser()
{
    for (auto dataType : dataTypes)
    {
        delete dataType;
    }
}

bool PacketParser::parse(uint8_t *data, size_t len)
{
    if (data[0] != 'P' || data[1] != 'I')
        return false;

    size_t index = 2;

    for (DataType *dataType : dataTypes)
    {
        switch (dataType->getType())
        {
        case DATA_8U:
        {
            if (index + 1 >= len)
                return false;

            DataType8U *dT = static_cast<DataType8U *>(dataType);
            if (dT != nullptr)
            {
                uint8_t value;
                memcpy(&value, data + index, 1);
                dT->setData(value);
            }
            index++;
            break;
        }
        case DATA_16U:
        {
            if (index + 2 >= len)
                return false;

            DataType16U *dT = static_cast<DataType16U *>(dataType);
            if (dT != nullptr)
            {
                uint16_t value;
                memcpy(&value, data + index, 2);
                dT->setData(value);
            }
            index += 2;
            break;
        }
        case DATA_32U:
        {
            if (index + 4 >= len)
                return false;

            DataType32U *dT = static_cast<DataType32U *>(dataType);
            if (dT != nullptr)
            {
                uint32_t value;
                memcpy(&value, data + index, 4);
                dT->setData(value);
            }
            index += 4;
            break;
        }
        case DATA_STR:
        {
            DataTypeSTR *dT = static_cast<DataTypeSTR *>(dataType);
            if (dT != nullptr)
            {
                if (index + 2 >= len)
                    return false;

                uint16_t strlength;
                memcpy(&strlength, data + index, 2);

                index += 2;

                if (index + strlength >= len)
                    return false;

                if (strlength <= 256)
                {
                    char strdata[256];
                    memcpy(strdata, data + index, strlength);
                    index += strlength;
                    dT->setData(strdata, strlength);
                }
            }
            break;
        }
        }
    }
    return true;
}

bool PacketParser::parse(char *data, size_t len)
{
    return parse((uint8_t *)data, len);
}

const std::vector<DataType *> PacketParser::getData() const
{
    return dataTypes;
}