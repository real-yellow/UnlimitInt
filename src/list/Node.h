#ifndef NODE
#define NODE
namespace real_yellow
{
template <class Type>
class Node
{
    //元素存储器
    Type container;
  public:
    //指向下一个节点
    Node<Type> *nextNode;
    //构造函数
    Node<Type>(Type element);
    //更改存储器的内容
    void operator=(Type element);
    //获取存储器的内容
    const Type& getContainer() const;
    //修改元素
    Type& modify();
};
} // namespace real_yellow
#endif