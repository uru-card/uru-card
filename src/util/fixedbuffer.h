#pragma once

template <size_t S>
struct FixedBuffer
{
    /**
     * @brief stored buffer value
     */
    uint8_t value[S];
    /**
     * @brief stored value length
     */
    size_t length;
    /**
     * @brief maximal possible value length
     */
    const size_t maxLength = S;

    FixedBuffer() : length(0)
    {
        memset(value, 0, S);
    }

    FixedBuffer(const FixedBuffer<S> &b)
    {
        this->length = b.length;
        memcpy(this->value, b.value, b.length);
    }

    FixedBuffer &operator=(const FixedBuffer<S> &b)
    {
        this->length = b.length;
        memcpy(this->value, b.value, b.length);
        return *this;
    }

    bool operator==(const FixedBuffer<S> &b)
    {
        return this->length == b.length && memcmp(this->value, b.value, b.length) == 0;
    }

    void alloc(const size_t len)
    {
        assert(len <= S);
        length = len;
    }
};

typedef FixedBuffer<16> FixedBuffer16;
typedef FixedBuffer<32> FixedBuffer32;
typedef FixedBuffer<64> FixedBuffer64;