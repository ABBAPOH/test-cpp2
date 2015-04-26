#include "message.h"

Message::Message(int64_t seq, const ByteArray &data) :
    _seq(0),
    _data(data)
{
}

Message::~Message()
{
}

