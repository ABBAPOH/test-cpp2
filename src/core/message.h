#ifndef MESSAGE
#define MESSAGE

#include <sys/types.h>

#pragma pack(push, 1)
struct Frame
{
    int16_t size {0};
    int16_t id {0};
    int64_t seq {0};
};
#pragma pack(pop)

struct Message
{
    int16_t size {0};
    int16_t id {0};
    int64_t seq {0};
    char *data {0};
};

#endif // MESSAGE

