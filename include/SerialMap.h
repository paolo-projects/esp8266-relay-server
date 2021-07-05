#ifndef BUFFER_PARSER_H
#define BUFFER_PARSER_H

#include <sstream>
#include "Map.h"
#include "Serializable.h"

/**
 * @brief A serializable map based on the existing implementation with String support
 * 
 * @tparam T The data type used for keys and valued. Generally it has to be a string-like type, such as std::string,
 *  otherwise the provided type must implement a constructor that accepts a null terminated char array, a << stream operator to std::io_stream types
 *  and a length() method
 * @tparam S The maximum size of the map in number of elements
 */
template <typename T = std::string, int S = 24>
class SerialMap : public Map<T, T, S>, public Serializable
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
            buf[length] = 0;
            T key(buf);

            if (!sst.good() || sst.get() != VALUE_TYPE)
            {
                break;
            }

            length = sst.get();
            sst.read(buf, length);
            buf[length] = 0;
            T value(buf);

            Map<T, T, S>::put(key, value);
        }
    }

    /**
     * @brief Serializes the map into a binary data stream
     * 
     * @param data The output data buffer
     * @param len The size of the output data buffer
     * @return size_t The size of the data written into the buffer, or -1 if it failed
     */
    size_t serialize(char *data, size_t len) const override
    {
        size_t expectedSize = 0;
        for (int i = 0; i < Map<T, T, S>::size; i++)
        {
            expectedSize += Map<T, T, S>::keys[i].length() + Map<T, T, S>::values[i].length() + 4;
        }

        if (expectedSize > len)
        {
            return -1;
        }

        std::stringstream sst;
        for (int i = 0; i < Map<T, T, S>::size; i++)
        {
            const String &key = Map<T, T, S>::keys[i];
            const String &value = Map<T, T, S>::values[i];

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
