#include "UnlimitedInteger.h"
#include <iostream>
using std::endl;
using std::cerr;
using namespace real_yellow;

const int UnlimitedInteger::SPLIT = 1000000000;

void UnlimitedInteger::parse(int64_t sourceNumber)
{
    container.clear();
    if(sourceNumber < 0)
    {
        sourceNumber = IntMath::reverse(sourceNumber);
        negativeNumber = true;
    }
    else
    {
        negativeNumber = false;
    }
    
    while(sourceNumber > 0)
    {
        container.add((int)(sourceNumber % SPLIT));
        sourceNumber = sourceNumber / SPLIT;
    }
}

void UnlimitedInteger::parse(const std::string &sourceString)
{
    container.clear();
    std::stringstream buffer;
    int addItem = 0;
    const int SPLIT_LENGTH = 9;
    try
    {
        for (int i = sourceString.size(); i > 0;)
        {
            buffer.clear();
            if (i - SPLIT_LENGTH > 0)
            {
                buffer << sourceString.substr(i - SPLIT_LENGTH, SPLIT_LENGTH);
                i -= SPLIT_LENGTH;
                buffer >> addItem;
                if (addItem < 0)
                {
                    throw new std::invalid_argument(sourceString);
                }
                container.add(addItem);
            }
            else
            {
                buffer << sourceString.substr(0, i);
                i = 0;
                buffer >> addItem;
                if (addItem < 0)
                {
                    addItem = (int)IntMath::reverse(addItem);
                    negativeNumber = true;
                }
                else
                {
                    negativeNumber = false;
                }
                container.add(addItem);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::invalid_argument invaild(sourceString + " is not an integer");
        throw invaild;
    }
}

/**
 * @brief 绝对值加法
 * 
 * @param rightInteger 右值绝对值
 * @return UnlimitedInteger 两值绝对值累加结果
 */
UnlimitedInteger UnlimitedInteger::absAdd(const UnlimitedInteger &rightInteger) const
{
    UnlimitedInteger newInteger;
    int maxSize = Compare::max(container.count(),rightInteger.container.count());
    int minSize = Compare::min(container.count(),rightInteger.container.count());
    //判断左值的元素是否最多
    bool leftIsMax = (container.count() > rightInteger.container.count());
    //进位临时存储
    int64_t carryBuffer = 0;
    //累加临时存储
    int64_t addBuffer = 0;
    for(int i = 0;;i++)
    {
        if (i < minSize)
        {
            addBuffer = (int64_t)container.at(i) + (int64_t)rightInteger.container.at(i) + carryBuffer;
            carryBuffer = addBuffer / (int64_t)SPLIT;
            newInteger.container.add((int)(addBuffer % (int64_t)SPLIT));
        }
        else if (i < maxSize)
        {
            if(leftIsMax)
            {
                addBuffer = (int64_t)container.at(i) + carryBuffer;
            }
            else
            {
                addBuffer = (int64_t)rightInteger.container.at(i) + carryBuffer;
            }           
            carryBuffer = addBuffer / (int64_t)SPLIT;       
            newInteger.container.add((int)(addBuffer % (int64_t)SPLIT));
        }
        else if(i >= maxSize && carryBuffer != 0)
        {
            newInteger.container.add((int)(carryBuffer % (int64_t)SPLIT));
            carryBuffer /= SPLIT;
        }
        else
        {
            break;
        }
    }
    return newInteger;
}

/**
 * @brief 绝对值减法
 * 
 * @param rightInteger 右值绝对值 
 * @return UnlimitedInteger 两绝对值相减结果
 */
UnlimitedInteger UnlimitedInteger::absMinus(const UnlimitedInteger &rightInteger) const
{
    UnlimitedInteger newInteger;
    if(*this == rightInteger)
    {
        newInteger = 0;
        return newInteger;
    }
    //每个节点做减法的结果
    int64_t minusBuffer = 0;
    //左值绝对值大于右值绝对值
    if(absMoreThan(rightInteger,false))
    {
        //被减数
        List<int> buffer = container.clone();
        for(int i = 0;;i++)
        {
            if (i < rightInteger.container.count())
            {
                minusBuffer = (int64_t)buffer.at(i) - (int64_t)rightInteger.container.at(i);
                if(minusBuffer < 0)
                {
                    minusBuffer += SPLIT;
                    //借位
                    for (int j = i + 1; j < buffer.count(); j++)
                    {
                        buffer[j]--;
                        if (buffer.at(j) >= 0)
                        {
                            break;
                        }
                        else
                        {
                            buffer[j] += SPLIT;
                        }
                    }
                }
                newInteger.container.add(minusBuffer);
            }
            else if (i < buffer.count())
            {
                newInteger.container.add(buffer.at(i));
            }
            else
            {
                break;
            }
        }
    }
    //左值绝对值小于右值绝对值
    else
    {
        newInteger.negativeNumber = true;
        //被减数
        List<int> buffer = rightInteger.container.clone();
        for(int i = 0;;i++)
        {
            if (i < container.count())
            {
                minusBuffer = (int64_t)buffer.at(i) - (int64_t)container.at(i);
                if(minusBuffer < 0)
                {
                    minusBuffer += SPLIT;
                    //借位
                    for (int j = i + 1; j < buffer.count(); j++)
                    {
                        buffer[j]--;                     
                        if (buffer.at(j) >= 0)
                        {
                            break;
                        }
                        else
                        {
                            buffer[j] += SPLIT;
                        }
                    }
                }
                newInteger.container.add(minusBuffer);
            }
            else if (i < buffer.count())
            {
                newInteger.container.add(buffer.at(i));
            }
            else
            {
                break;
            }
        }
    }
    //移除冗余的0
    for (unsigned int i = newInteger.container.count() - 1;i >= 0; i--)
    {
        if(newInteger.container.at(i) == 0)
        {
            newInteger.container.remove(i);
        }
        else
        {
            break;
        }
    }
    return newInteger;
}

/**
 * @brief 左值的绝对值是否大于右值
 * 
 * @param rightInteger 右值
 * @param acceptEqual 两值相等时是否返回true(大于等于)
 * @return true 
 * @return false 
 */
bool UnlimitedInteger::absMoreThan(const UnlimitedInteger &rightInteger,bool acceptEqual) const
{
    //两个大整数占用的链表节点数相同
    if(container.count() == rightInteger.container.count())
    {
        for (int i = container.count() - 1; i >= 0; i--)
        {
            if(container.at(i) != rightInteger.container.at(i))
            {
                return container.at(i) > rightInteger.container.at(i);
            }   
        }
        //两值绝对值完全相同
        return acceptEqual;
    } 
    else 
    {
        return container.count() > rightInteger.container.count();
    }
}

/**
 * @brief 左值的绝对值是否小于右值
 * 
 * @param rightInteger 右值
 * @param acceptEqual 两值相等时是否返回true(小于等于)
 * @return true 
 * @return false 
 */
bool UnlimitedInteger::absLessThan(const UnlimitedInteger &rightInteger,bool acceptEqual) const
{
    //两个大整数占用的链表节点数相同
    if(container.count() == rightInteger.container.count())
    {
        for (int i = container.count() - 1; i >= 0; i--)
        {
            if(container.at(i) != rightInteger.container.at(i))
            {
                return container.at(i) < rightInteger.container.at(i);
            }   
        }
        //两值绝对值完全相同
        return acceptEqual;
    } 
    else 
    {
        return container.count() < rightInteger.container.count();
    }
}

UnlimitedInteger::UnlimitedInteger()
{
    negativeNumber = false;
}

UnlimitedInteger::UnlimitedInteger(int64_t sourceNumber)
{
    parse(sourceNumber);
}

UnlimitedInteger::UnlimitedInteger(const UnlimitedInteger &sourceObject)
{
    container = sourceObject.container;
    negativeNumber = sourceObject.negativeNumber;
}

UnlimitedInteger::UnlimitedInteger(const std::string &sourceString)
{
    parse(sourceString);
}

void UnlimitedInteger::operator=(int64_t sourceNumber)
{
    parse(sourceNumber);
}

void UnlimitedInteger::operator=(const UnlimitedInteger &sourceObject)
{
    container = sourceObject.container;
    negativeNumber = sourceObject.negativeNumber;
}

void UnlimitedInteger::operator=(const std::string &sourceString)
{
    parse(sourceString);
}

UnlimitedInteger UnlimitedInteger::operator+(const int64_t rightInteger) const
{
    UnlimitedInteger right(rightInteger);
    return this->operator+(right);
}

UnlimitedInteger UnlimitedInteger::operator+(const UnlimitedInteger &rightInteger) const
{
    if(negativeNumber && rightInteger.negativeNumber)
    {
        UnlimitedInteger newInteger(absAdd(rightInteger));
        newInteger.negativeNumber = true;
        return newInteger;
    }
    //左值为负数
    else if(negativeNumber)
    {
        return rightInteger.absMinus(*this);
    }
    //右值为负数
    else if(rightInteger.negativeNumber)
    {
        return absMinus(rightInteger);
    }
    else
    {
        return absAdd(rightInteger);
    }
}

UnlimitedInteger UnlimitedInteger::operator+(const std::string &rightString) const
{
    UnlimitedInteger right(rightString);
    return operator+(right);
}

UnlimitedInteger UnlimitedInteger::operator-(const int64_t rightInteger) const
{
    UnlimitedInteger right(rightInteger);
    return *this + right;
}

UnlimitedInteger UnlimitedInteger::operator-(const UnlimitedInteger &rightInteger) const
{
    if(negativeNumber && rightInteger.negativeNumber)
    {
        return rightInteger.absMinus(*this);
    }
    //左值为负数
    else if(negativeNumber)
    {
        UnlimitedInteger newInt(absAdd(rightInteger));
        newInt.negativeNumber = true;
        return newInt;
    }
    //右值为负数
    else if(rightInteger.negativeNumber)
    {
        return absAdd(rightInteger);
    }
    else
    {
        return absMinus(rightInteger);
    }
}

UnlimitedInteger UnlimitedInteger::operator-(const std::string &rightString) const
{
    UnlimitedInteger right(rightString);
    return operator-(right);
}

UnlimitedInteger UnlimitedInteger::operator*(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this * newInteger;
}

UnlimitedInteger UnlimitedInteger::operator*(const UnlimitedInteger &rightInteger) const
{
    UnlimitedInteger newInteger(0);
    //模拟手工乘法:分部乘法
    for(int i = 0;i < container.count();i++)
    {
        //进位临时存储
        int64_t carryBuffer = 0;
        //乘法临时存储
        int64_t multiBuffer = 0;
        UnlimitedInteger buffer = 0;
        //添0
        for (int zeroLoop = 0; zeroLoop < i; zeroLoop++)
        {
            buffer.container.add(0);
        }
        //分部乘法
        for(int j = 0; j < rightInteger.container.count();j++)
        {
            multiBuffer = (int64_t)container.at(i) * (int64_t)rightInteger.container.at(j) + carryBuffer;
            carryBuffer = multiBuffer / (int64_t)SPLIT;
            buffer.container.add((int)(multiBuffer % SPLIT));
        }
        //进位
        while(carryBuffer > 0)
        {
            buffer.container.add((int)(carryBuffer % SPLIT));
            carryBuffer /= SPLIT;
        }
        //累加
        newInteger = newInteger + buffer;
    }
    newInteger.negativeNumber = negativeNumber != rightInteger.negativeNumber;
    return newInteger;
}

UnlimitedInteger UnlimitedInteger::operator*(const std::string &rightString) const
{
    UnlimitedInteger newInteger(rightString);
    return *this * newInteger;
}

UnlimitedInteger UnlimitedInteger::operator/(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this / newInteger;
}

UnlimitedInteger UnlimitedInteger::operator/(const UnlimitedInteger &rightInteger) const
{
    UnlimitedInteger newInteger(0);
    switch(rightInteger.container.count())
    {
    case 1:
        //检查右值是不是特殊值--{-1,1}
        if(rightInteger.container.at(0) == 1)
        {
            newInteger = *this;
            newInteger.negativeNumber = (negativeNumber != rightInteger.negativeNumber);
            return newInteger;
        }
        else if(container.count() == 1)
        {
            newInteger = container.at(0) / rightInteger.container.at(0);
            newInteger.negativeNumber = (negativeNumber != rightInteger.negativeNumber);
            return newInteger;
        }
        //检查右值是不是0
        else if(rightInteger.container.at(0) == 0)
        {
            throw std::invalid_argument("divisor is zero");
        } 
        break;
    case 0:
        throw std::invalid_argument("divisor is zero"); 
    }
    UnlimitedInteger dividentBuffer = *this;
    dividentBuffer.negativeNumber = false;
    UnlimitedInteger previousBuffer(0);
    UnlimitedInteger divisorBuffer(rightInteger);
    divisorBuffer.negativeNumber = false;
    //divisorBuffer 与 被除数的倍数
    UnlimitedInteger indexBuffer(1);
    //与除数多次做减法得到商
    while (true)
    {
        if(dividentBuffer.absLessThan(divisorBuffer,false))
        {
            if(dividentBuffer < rightInteger)
            {
                newInteger.negativeNumber = (negativeNumber != rightInteger.negativeNumber);
                return newInteger;
            }
            else
            {
                divisorBuffer = rightInteger;
                divisorBuffer.negativeNumber = false;
                indexBuffer = 1;
            }
        }
        dividentBuffer = dividentBuffer - divisorBuffer;
        newInteger = newInteger + indexBuffer;
        divisorBuffer = divisorBuffer * 2;
        indexBuffer = indexBuffer * 2;
    }
}

UnlimitedInteger UnlimitedInteger::operator/(const std::string &rightString) const
{
    UnlimitedInteger newInteger(rightString);
    return *this / newInteger;
}

bool UnlimitedInteger::operator==(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this == newInteger;
}

bool UnlimitedInteger::operator==(UnlimitedInteger rightInteger) const
{
    //两个大整数占用的链表节点数相同,且正负相同
    if(container.count() == rightInteger.container.count() && negativeNumber == rightInteger.negativeNumber)
    {
        for (int i = container.count() - 1; i >= 0; i--)
        {
            if(container.at(i) != rightInteger.container.at(i))
            {
                return false;
            }   
        }
        //两值完全相同
        return true;
    } 
    else 
    {
        return false;
    }
}
bool UnlimitedInteger::operator==(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return *this == newInteger;
}

bool UnlimitedInteger::operator!=(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return !(*this == newInteger);
}
bool UnlimitedInteger::operator!=(UnlimitedInteger rightInteger) const
{
    return !(*this == rightInteger);
}
bool UnlimitedInteger::operator!=(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return !(*this == newInteger);
}

bool UnlimitedInteger::operator>=(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this >= newInteger;
}

bool UnlimitedInteger::operator>=(UnlimitedInteger rightInteger) const
{
    if(!negativeNumber && !rightInteger.negativeNumber)
    {
        return absMoreThan(rightInteger,true);
    }
    else if(negativeNumber && rightInteger.negativeNumber)
    {
        return absLessThan(rightInteger,true);
    }
    else
    {
        return rightInteger.negativeNumber;
    }
}
bool UnlimitedInteger::operator>=(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return *this >= newInteger;
}

bool UnlimitedInteger::operator<=(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this <= newInteger;
}
bool UnlimitedInteger::operator<=(UnlimitedInteger rightInteger) const
{
    if(!negativeNumber && !rightInteger.negativeNumber)
    {
        return absLessThan(rightInteger,true);
    }
    else if(negativeNumber && rightInteger.negativeNumber)
    {
        return absMoreThan(rightInteger,true);
    }
    else
    {
        return negativeNumber;
    }
}
bool UnlimitedInteger::operator<=(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return *this <= newInteger;
}

bool UnlimitedInteger::operator>(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this > newInteger;
}

bool UnlimitedInteger::operator>(UnlimitedInteger rightInteger) const
{
    if(!negativeNumber && !rightInteger.negativeNumber)
    {
        return absMoreThan(rightInteger,false);
    }
    else if(negativeNumber && rightInteger.negativeNumber)
    {
        return absLessThan(rightInteger,false);
    }
    else
    {
        return rightInteger.negativeNumber;
    }
}

bool UnlimitedInteger::operator>(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return *this > newInteger;
}

bool UnlimitedInteger::operator<(const int64_t rightInteger) const
{
    UnlimitedInteger newInteger(rightInteger);
    return *this < newInteger;
}

bool UnlimitedInteger::operator<(UnlimitedInteger rightInteger) const
{
    if(!negativeNumber && !rightInteger.negativeNumber)
    {
        return absLessThan(rightInteger,false);
    }
    else if(negativeNumber && rightInteger.negativeNumber)
    {
        return absMoreThan(rightInteger,false);
    }
    else
    {
        return negativeNumber;
    }
}
bool UnlimitedInteger::operator<(const std::string &sourceString) const
{
    UnlimitedInteger newInteger(sourceString);
    return *this < newInteger;
}

std::string UnlimitedInteger::toString() const
{
    std::string str;
    std::stringstream streamBuffer;
    if(container.count() == 0)
    {
        return (std::string)"0";    
    }
    if (negativeNumber)
    {
        str += "-";
    }
    std::string buffer;
    for (int lop = container.count() - 1; lop >= 0; lop--)
    {
        const int SPLIT_LENGTH = 9;
        streamBuffer.clear();
        streamBuffer << container.at(lop);
        streamBuffer >> buffer;
        if(lop != container.count() - 1)
        {
            //补0
            for (int j = buffer.size(); j < SPLIT_LENGTH; j++)
            {
                buffer = "0" + buffer;
            }
        }
        str += buffer;
    }
    return str;
}

UnlimitedInteger UnlimitedInteger::power(UnlimitedInteger base,unsigned index)
{
    if(index == 1)
    {
        return base;
    }
    else if(base == 0)
    {
        return (UnlimitedInteger)0;
    }
    else if(index == 0 && base != 0)
    {
        return (UnlimitedInteger)1;
    }
    else
    {
        UnlimitedInteger newInteger = 1;
        unsigned buffer = 0;
        while (index != 0)
        {
            buffer = index % 2;
            index = index >> 1;
            if(buffer == 1)
            {
                newInteger = newInteger * base;
            }
            base = base * base;
        }
        newInteger.negativeNumber = base.negativeNumber;
        return newInteger;
    }
}

UnlimitedInteger::~UnlimitedInteger()
{
    container.clear();
}