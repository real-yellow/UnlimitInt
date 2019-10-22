#include "Node.h"
#pragma once
namespace real_yellow
{
template <class Type>
Node<Type>::Node(Type element)
{
    container = element;
    nextNode = nullptr;
}

template <class Type>
void Node<Type>::operator=(Type element)
{
    container = element;
}

template <class Type>
const Type& Node<Type>::getContainer() const
{
    return container;
}

template <class Type>
Type& Node<Type>::modify()
{
    return container;
}
} // namespace real_yellow