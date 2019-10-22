#include <cstdint>
#ifndef INTMATH
#define INTMATH
class IntMath
{
public:
    //计算相反数
    static int64_t reverse(int64_t integer)
    {
        return -integer;
    }

    //计算绝对值
    static int64_t absolute(int64_t integer)
    {
        if(integer < 0)
        {
            return -integer;
        }
    }
};
#endif