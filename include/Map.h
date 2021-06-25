#include <functional>

/**
 * @brief A static (bad) implementation of a map/dictionary
 * 
 * @tparam T The type of the keys
 * @tparam E The type of the values
 * @tparam S The pre-allocated max-number of elements the map is allowed to have. Defaults to 24 to spare device memory.
 */
template <typename T, typename E, int S = 24, class equals = std::equal_to<T>>
class Map
{
public:
    /**
     * @brief Construct a new Map object
     * 
     */
    Map() {}
    /**
     * @brief Puts the give key-value pair in the map
     * 
     * @param key The key
     * @param value The value
     */
    void put(const T &key, const E &value)
    {
        int vindex = indexOf(key);
        if (vindex < 0)
        {
            keys[cursor] = key;
            values[cursor] = value;
            size++;
            cursor++;
        }
        else
        {
            values[vindex] = value;
        }
    }
    /**
     * @brief Gets the value by the given key
     * 
     * @param key The key to search for
     * @return E* The pointer of the value
     * @retval nullptr When the key has not been found
     */
    const E *get(const T &key) const
    {
        int vindex = indexOf(key);
        if (vindex >= 0)
        {
            return &values[vindex];
        }
        return nullptr;
    }
    /**
     * @brief Attempts removing a key-value pair from the map
     * 
     * @param key The key to remove
     * @return true If the key-value pair has been removed successfully
     * @return false If the key hasn't been found
     */
    bool remove(const T &key)
    {
        int vindex = indexOf(key);
        if (vindex >= 0)
        {
            shiftLeft(vindex + 1);
            size--;
            cursor--;
            return true;
        }
        return false;
    }
    /**
     * @brief Checks if the given key has been set inside the map
     * 
     * @param key The key to search for
     * @return true If the given key is present in the map
     * @return false If the given key is not in the map
     */
    bool has(const T &key) const
    {
        return indexOf(key) >= 0;
    }
    /**
     * @brief Get the value paired with the given key
     * 
     * @param key The key to search for
     * @return E* The value
     * @retval nullptr When the key has not been found
     */
    const E *operator[](const T &key) const
    {
        return get(key);
    }

private:
    T keys[S];
    E values[S];
    int size = 0;
    int cursor = 0;

    /**
     * @brief Construct a new Map object from the given key-value arrays
     * 
     * @param k The array of keys
     * @param v The array of values
     * @param size The size of the arrays
     */
    Map(const T *k, const E *v, int size)
    {
        if (size < S)
        {
            for (int i = 0; i < size; i++)
            {
                keys[i] = k[i];
                values[i] = v[i];
            }
        }
        this->size = cursor = size;
    }
    /**
     * @brief Shifts left the elements of the arrays starting from the given index
     * 
     * @param startIndex The first index to shift left
     */
    void shiftLeft(int startIndex)
    {
        for (int i = startIndex; i < size; i++)
        {
            keys[i - 1] = keys[i];
            values[i - 1] = values[i];
        }
    }
    /**
     * @brief Returns the internal index of a given key.
     * The comparison is either done based on the == operator of E
     * or equals can be specified as a custom equality comparator
     * 
     * @param key The key to find
     * @return int The index
     * @retval -1 When the key has not been found
     */
    int indexOf(const T &key) const
    {
        for (int i = 0; i < size; i++)
        {
            if (equals()(keys[i], key))
            {
                return i;
            }
        }
        return -1;
    }
};