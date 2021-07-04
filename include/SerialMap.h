#ifndef BUFFER_PARSER_H
#define BUFFER_PARSER_H

#include <sstream>
#include "Arduino.h"
#include "Map.h"

/**
 * @brief A serializable map based on the existing implementation with String support
 * 
 * @tparam S The maximum size of the map in number of elements
 */
template <int S = 24>
class SerialMap : public Map<String, String, S>
{
public:
    SerialMap() = default;
    /**
     * @brief Construct a new Serial Map object from the raw data
     * 
     * @param buffer The data buffer
     * @param len The buffer size
     */
    SerialMap(const char *buffer, size_t len)
    {
        std::stringstream sst;
        sst.write(buffer, len);
        char buf[257] = {};

        while (!sst.eof())
        {
            if (!sst.good() || sst.get() != KEY_TYPE)
            {
                break;
            }

            unsigned char length = sst.get();
            sst.read(buf, length);
            String key(buf);

            if (!sst.good() || sst.get() != VALUE_TYPE)
            {
                break;
            }

            length = sst.get();
            sst.read(buf, length);
            String value(buf);

            Map<String, String, S>::put(key, value);
        }
    }

    /**
     * @brief Serializes the map into a binary data stream
     * 
     * @param data The output data buffer
     * @param len The size of the output data buffer
     * @return size_t The size of the data written into the buffer, or -1 if it failed
     */
    size_t serialize(char *data, size_t len)
    {
        size_t expectedSize = 0;
        for (auto it = Map<String, String, S>::begin(); it != Map<String, String, S>::end(); it++)
        {
            expectedSize += (*it).key().length() + (*it).value().length() + 4;
        }

        if (expectedSize > len)
        {
            return -1;
        }

        std::stringstream sst;
        for (auto it = Map<String, String, S>::begin(); it != Map<String, String, S>::end(); it++)
        {
            const String &key = (*it).key();
            const String &value = (*it).value();

            sst.put(KEY_TYPE);
            sst.put(key.length());
            sst << key;

            sst.put(VALUE_TYPE);
            sst.put(value.length());
            sst << value;
        }

        sst.read(data, expectedSize);

        return expectedSize;
    }

private:
    static constexpr char KEY_TYPE = 0x10;
    static constexpr char VALUE_TYPE = 0x11;
};

#endif
