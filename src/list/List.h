#include "Node.h"
#include "Node.cpp"
#include <stdexcept>
#include <string>
#ifndef LIST
#define LIST
namespace real_yellow
{
/*
*链表
*/
template <class Type>
class List
{
    //表头
    Node<Type> *listHead;
    //结尾
    Node<Type> *listEnd;
    //记录链表内数据数量
    unsigned amount;
    //
    static const std::string OUT_OF_RANGE;
    static const std::string FILE_LABEL;
    static const std::string LINE_LABEL;
    static const std::string LIST_IS_EMPTY;
  public:
    //构造函数
    List();
    //添加元素至末尾
    void add(Type element);
    //添加元素至指定位置
    void add(unsigned serialNumber,Type element);
    //返回元素数量
    unsigned count() const;
    //查找某个元素是否存在于此链表中(不兹磁没有重载"=="的对象)
    bool exists(const Type &match) const;
    //查找某个元素在本链表的位置(不兹磁没有重载"=="的对象)
    int find(const Type &match) const;
    //移除末端元素
    void remove();
    //移除指定位置元素
    void remove(unsigned serialNumber);
    //移除所有元素
    void clear();
    //获取或修改指定位置元素
    Type &operator[](unsigned serialNumber);
    //获取指定位置元素
    const Type &at(unsigned serialNumber) const;
    //复制这个列表
    List<Type> clone() const;
    //复制这个列表
    void operator=(const List<Type> &sourceList);
    //析构函数
    ~List();
};
} // namespace real_yellow
#endif