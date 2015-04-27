#include "message.h"

Message::Message(int64_t seq, const ByteArray &data) :
    _seq(seq),
    _data(data)
{
}

Message::~Message()
{
}

