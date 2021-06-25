#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include <vector>
#include "DataType.h"

class PacketParser
{
public:
    PacketParser() = delete;
    PacketParser(const PacketParser &) = delete;
    PacketParser(PacketParser &&) = delete;
    PacketParser &operator=(const PacketParser &) = delete;

    PacketParser(const std::vector<DataType *> &dataTypes);
    PacketParser(std::vector<DataType *> &&dataTypes);
    bool parse(uint8_t *data, size_t len);
    bool parse(char *data, size_t len);
    const std::vector<DataType *> getData() const;
    ~PacketParser();

private:
    std::vector<DataType *> dataTypes;
    size_t expectedSize;
};

#endif