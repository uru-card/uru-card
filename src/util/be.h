#pragma once

#pragma pack(push, 1)
template <typename T>
class BigEndian
{
public:
    BigEndian() = default;
    BigEndian(const BigEndian &val)
    {
        value = val.value;
    }
    BigEndian(const T val)
    {
        value = swap(val);
    }

    /**
     * Read BigEndian value from the byte buffer
     */
    BigEndian(const uint8_t *val)
    {
        value = *((const T *)val);
    }

    BigEndian &operator=(T val)
    {
        value = swap(val);
        return *this;
    }

    operator T() const
    {
        return swap(value);
    }

private:
    T swap(T val) const
    {
        uint8_t *a = (uint8_t *)&val;

        T ret;
        uint8_t *b = (uint8_t *)&ret;

        for (auto i = 0; i < sizeof(T); i++)
        {
            b[i] = a[sizeof(T) - 1 - i];
        }

        return ret;
    }

private:
    T value;
};

typedef BigEndian<uint16_t> be_uint16_t;
#pragma pack(pop)