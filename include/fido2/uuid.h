#pragma once

namespace FIDO2
{
    class UUID
    {
    public:
        UUID();
        UUID(const char *);
        UUID(const uint8_t *, const size_t);
        UUID(const UUID &);

        const uint8_t *get_bytes() const;

    private:
        uint8_t uuid[16];
    };
} // namespace FIDO2
