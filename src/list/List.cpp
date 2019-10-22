#include "List.h"
#include "Node.h"
#pragma once
namespace real_yellow
{
template <class Type>
const std::string List<Type>::OUT_OF_RANGE = "subscript is out of range--at:";

template <class Type>
const std::string List<Type>::FILE_LABEL = " File:";

template <class Type>
const std::string List<Type>::LINE_LABEL = " Line:";

template <class Type>
const std::string List<Type>::LIST_IS_EMPTY = "list is empty--at:";

template <class Type>
List<Type>::List()
{
    listHead = nullptr;
    listEnd = nullptr;
    amount = 0;
}

//添加元素至末尾
template <class Type>
void List<Type>::add(Type element)
{
    Node<Type> *newNode = new Node<Type>(element);
    if (amount == 0)
    {
        listHead = newNode;
        listEnd = listHead;
    }
    else
    {
        listEnd->nextNode = newNode;
        listEnd = newNode;
    }
    amount++;
}

//添加元素至指定位置
template <class Type>
void List<Type>::add(unsigned serialNumber, Type element)
{
    if (serialNumber > amount)
    {
        std::out_of_range outOfRange(OUT_OF_RANGE + FILE_LABEL + __FILE__ + LINE_LABEL);
        throw outOfRange;
    }
    //节点加入至表头或表中
    else if (amount > 0 && serialNumber < amount)
    {
        //加入至表头
        if (serialNumber == 0)
        {
            Node<Type> *newNode = new Node<Type>(element);
            newNode->nextNode = listHead;
            listHead = newNode;
        }
        //加入至表中
        else
        {
            Node<Type> *newNode = new Node<Type>(element);
            Node<Type> *previousPointer = listHead;
            for (unsigned lop = 0; lop < amount - 1; lop++)
            {
                previousPointer = previousPointer->nextNode;
            }
            newNode->nextNode = previousPointer->nextNode;
            previousPointer->nextNode = newNode;
        }
        amount++;
    }
    //加入至表尾
    else
    {
        add(element);
    }
}

template <class Type>
unsigned List<Type>::count() const
{
    return amount;
}

template <class Type>
bool List<Type>::exists(const Type &match) const
{
    Node<Type> *buffer = listHead;
    for (unsigned lop = 0; lop < amount; lop++)
    {
        if (match == buffer->getContainer())
        {
            return true;
        }
        if (buffer->nextNode != nullptr)
        {
            buffer = buffer->nextNode;
        }
    }
    return false;
}

template <class Type>
int List<Type>::find(const Type &match) const
{
    Node<Type> *buffer = listHead;
    for (int lop = 0; lop < amount; lop++)
    {
        if (match == buffer->getContainer())
        {
            return lop;
        }
        if (buffer->nextNode != nullptr)
        {
            buffer = buffer->nextNode;
        }
    }
    return -1;
}

template <class Type>
void List<Type>::remove()
{
    if (amount <= 0)
    {
        std::range_error listIsEmpty(LIST_IS_EMPTY + FILE_LABEL + __FILE__);
        throw listIsEmpty;
    }
    else
    {
        remove(amount - 1);
    }
}

template <class Type>
void List<Type>::remove(unsigned serialNumber)
{
    if (serialNumber >= amount)
    {
        std::out_of_range outOfRange(OUT_OF_RANGE + __FUNCTION__ + " " + FILE_LABEL + __FILE__);
        throw outOfRange;
    }
    else
    {
        Node<Type> *deleteNode = listHead;
        if (serialNumber > 0 && serialNumber < amount)
        {
            Node<Type> *previousBuffer = listHead;
            for (int lop = 0; lop < serialNumber - 1; lop++)
            {
                previousBuffer = previousBuffer->nextNode;
            }
            deleteNode = previousBuffer->nextNode;
            if(serialNumber < amount - 1)
            {
                previousBuffer->nextNode = deleteNode->nextNode;
            }
            //被删节点位于表尾
            else
            {
                previousBuffer = listEnd;
            }
        }
        //被删节点位于表头
        else if(serialNumber == 0)
        {
            if(amount > 1)
            {
                listHead = listHead->nextNode;
            }
            //被删除的节点是最后一个节点
            else
            {
                listHead = nullptr;
                listEnd = nullptr;
            }
        }
        delete deleteNode;
        amount--;
    }
}

template <class Type>
void List<Type>::clear()
{
    unsigned int amountBuffer = amount;
    for (unsigned int i = 0; i < amountBuffer; i++)
    {
        remove();
    }
}

template <class Type>
Type &List<Type>::operator[](unsigned serialNumber)
{
    if (serialNumber >= amount)
    {
        std::out_of_range outOfRange(OUT_OF_RANGE + __FUNCTION__ + " " + FILE_LABEL + __FILE__);
        throw outOfRange;
    }
    else
    {
        Node<Type> *bufferPointer = listHead;
        for (unsigned lop = 0; lop < serialNumber; lop++)
        {
            bufferPointer = bufferPointer->nextNode;
        }
        return bufferPointer->modify();
    }
}

template <class Type>
const Type &List<Type>::at(unsigned serialNumber) const
{
    if (serialNumber >= amount)
    {
        std::out_of_range outOfRange(OUT_OF_RANGE + __FUNCTION__ + " " + FILE_LABEL + __FILE__);
        throw outOfRange;
    }
    else
    {
        Node<Type> *bufferPointer = listHead;
        for (unsigned lop = 0; lop < serialNumber; lop++)
        {
            bufferPointer = bufferPointer->nextNode;
        }
        return bufferPointer->getContainer();
    }
}

template <class Type>
List<Type> List<Type>::clone() const
{
    List<Type> copiedList;
    Node<Type> *bufferPointer = listHead;
    for(int lop = 0;lop < amount;lop ++)
    {
        copiedList.add(bufferPointer->getContainer());
        bufferPointer = bufferPointer->nextNode;
    }
    return copiedList;
}

template <class Type>
void List<Type>::operator=(const List<Type> &rightList)
{
    clear();
    Node<Type> *bufferPointer = rightList.listHead;
    for (int lop = 0; lop < rightList.amount; lop++)
    {
        add(bufferPointer->getContainer());
        bufferPointer = bufferPointer->nextNode;
    }
}

template <class Type>
List<Type>::~List()
{
    clear();
}

} // namespace real_yellow