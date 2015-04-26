#ifndef FRAME
#define FRAME

#include <sys/types.h>

#pragma pack(push, 1)
struct Frame
{
    int64_t seq {0};
    int64_t size {0};
};
#pragma pack(pop)

#endif // FRAME

