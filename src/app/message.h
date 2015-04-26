#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>

typedef std::vector<char> ByteArray;

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
    std::vector<char> _data;
};

#endif // MESSAGE_H
