#pragma once

namespace CTAP
{
    class InputBuffer
    {
    public:
        void reset();

        void setCmd(uint8_t cmd);
        void setLength(uint16_t len);

        void append(uint8_t *data, size_t len);

    protected:
    };

    InputBuffer inputBuffer;

} // namespace CTAP