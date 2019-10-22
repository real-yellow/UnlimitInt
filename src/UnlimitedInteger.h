#include <string>
#include <stdexcept>
#include "list/List.h"
#include "list/List.cpp"
#include <sstream>
#include <cstdint>
#include "util/Compare.h"
#include "util/IntMath.h"
using real_yellow::List;
#ifndef UNLIMITEDINTEGER
#define UNLIMITEDINTEGER
class UnlimitedInteger
{
    const static int SPLIT; 
    List<int> container;
    //指示该整数是否为负数
    bool negativeNumber;

    void parse(const std::string &sourceString);
    void parse(int64_t sourceNumber);
    //绝对值加法
    UnlimitedInteger absAdd(const UnlimitedInteger &rightInteger) const;
    //绝对值减法
    UnlimitedInteger absMinus(const UnlimitedInteger &rightInteger) const;
    //左值的绝对值是否大于右值(acceptEqual 指示两值相等时是否返回true)
    bool absMoreThan(const UnlimitedInteger &rightInteger,bool acceptEqual) const;
    //左值的绝对值是否小于右值
    bool absLessThan(const UnlimitedInteger &rightInteger, bool acceptEqual) const;
  public:
    //默认构造:列表为空
    UnlimitedInteger();
    //使用64位整数构造
    UnlimitedInteger(int64_t sourceNumber);
    //复制构造
    UnlimitedInteger(const UnlimitedInteger &sourceObject);
    //使用字符串构造(使用例 UnlimitedInteger u("9999999999999999999999999999999999999999");)
    UnlimitedInteger(const std::string &sourceString);
    
    void operator=(int64_t sourceNumber);
    void operator=(const UnlimitedInteger &sourceObject);
    void operator=(const std::string &sourceString);

    UnlimitedInteger operator+(const UnlimitedInteger &rightInteger) const;
    UnlimitedInteger operator+(const int64_t rightInteger) const;
    UnlimitedInteger operator+(const std::string &rightString) const;

    UnlimitedInteger operator-(const UnlimitedInteger &rightInteger) const;
    UnlimitedInteger operator-(const int64_t rightInteger) const;
    UnlimitedInteger operator-(const std::string &rightString) const;

    UnlimitedInteger operator*(const UnlimitedInteger &rightInteger) const;
    UnlimitedInteger operator*(const int64_t rightInteger) const;
    UnlimitedInteger operator*(const std::string &rightString) const;

    UnlimitedInteger operator/(const UnlimitedInteger &rightInteger) const;
    UnlimitedInteger operator/(const int64_t rightInteger) const;
    UnlimitedInteger operator/(const std::string &sourceString) const;

    bool operator==(const int64_t rightInteger) const;
    bool operator==(UnlimitedInteger rightInteger) const;
    bool operator==(const std::string &sourceString) const;

    bool operator!=(const int64_t rightInteger) const;
    bool operator!=(UnlimitedInteger rightInteger) const;
    bool operator!=(const std::string &sourceString) const;

    bool operator>=(const int64_t rightInteger) const;
    bool operator>=(UnlimitedInteger rightInteger) const;
    bool operator>=(const std::string &sourceString) const;

    bool operator<=(const int64_t rightInteger) const;
    bool operator<=(UnlimitedInteger rightInteger) const;
    bool operator<=(const std::string &sourceString) const;

    bool operator>(const int64_t rightInteger) const;
    bool operator>(UnlimitedInteger rightInteger) const;
    bool operator>(const std::string &sourceString) const;

    bool operator<(const int64_t rightInteger) const;
    bool operator<(UnlimitedInteger rightInteger) const;
    bool operator<(const std::string &sourceString) const;

    std::string toString() const;

    static UnlimitedInteger power(UnlimitedInteger base,unsigned index);
    ~UnlimitedInteger();
};
#endif