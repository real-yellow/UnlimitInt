#ifndef COMPARE
#define COMPARE
class Compare
{
  public:
    static int max(int a, int b)
    {
        if (a > b)
        {
            return a;
        }
        return b;
    }
    static int min(int a, int b)
    {
        if (a < b)
        {
            return a;
        }
        return b;
    }
};
#endif