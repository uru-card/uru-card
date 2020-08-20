#pragma once

template <size_t S>
struct FixedBuffer
{
    uint8_t value[S];
    const size_t size = S;

    void set(uint8_t *data, size_t len)
    {
        assert(len <= size);
    }
};

typedef FixedBuffer<16> FixedBuffer16;
typedef FixedBuffer<32> FixedBuffer32;
typedef FixedBuffer<64> FixedBuffer64;