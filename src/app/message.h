#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <sys/types.h>
#include <cstring>

class ByteArray : public std::vector<char>
{
public:
    ByteArray() = default;
    ByteArray(const char *s, size_t length) { resize(length); std::memmove(data(), s, length); }
    ByteArray(const ByteArray &other) = default;
};

class Message
{
public:
    explicit Message(int64_t seq = 0, const ByteArray &data = ByteArray());
    ~Message();

    inline int64_t seq() const { return _seq; }
    inline void setSeq(int64_t seq) { _seq = seq; }

    inline size_t size() const { return _data.size(); }

    inline const ByteArray &data() const { return _data; }
    inline void setData(const ByteArray &data) { _data = data; }

private:
    int64_t _seq {0};
    ByteArray _data;
};

inline bool operator ==(const Message &lhs, const Message &rhs)
{
    return lhs.seq() == rhs.seq() && lhs.data() == rhs.data();
}

#endif // MESSAGE_H
