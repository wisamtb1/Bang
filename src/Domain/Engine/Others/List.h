#ifndef LIST_H
#define LIST_H

#include <list>
#include <sstream>
#include <algorithm>

#include "String.h"

template <class T>
class List : public std::list<T>
{
public:
    typedef typename std::list<T>::iterator Iterator;
    typedef typename std::list<T>::const_iterator Const_Iterator;
    typedef typename std::list<T>::reverse_iterator Reverse_Iterator;
    typedef typename std::list<T>::const_reverse_iterator Const_Reverse_Iterator;

    List()
    {
    }

    List(const std::list<T> &l) : std::list<T>(l)
    {
    }

    bool Contains(const T& x) const
    {
        return std::find(Begin(), End(), x) != End();
    }

    void Splice(Iterator insertBefore, List &from)
    {
        this->splice(insertBefore, from);
    }
    void Splice(Iterator insertBefore, List &from, Iterator element)
    {
        this->splice(insertBefore, from, element);
    }
    void Splice(Iterator insertBefore, List &from,
                Iterator fromRangeBegin, Iterator fromRangeEnd)
    {
        this->splice(insertBefore, from, fromRangeBegin, fromRangeEnd);
    }

    void Clear()
    {
        this->clear();
    }

    int Size() const
    {
        return this->size();
    }
    bool Empty() const
    {
        return this->Size() == 0;
    }

    const T& Front() const
    {
        return this->front();
    }
    const T& Back() const
    {
        return this->back();
    }

    void Add(const T& x)
    {
        this->PushBack(x);
    }

    Iterator Find(const T& x)
    {
        return std::find(this->Begin(), this->End(), x);
    }
    Iterator FindLast(const T& x)
    {
        for (auto it = this->RBegin(); it != this->REnd(); ++it)
        {
            if (*it == x)
            {
                Iterator res = it.base();
                std::advance(res, -1);
                return res;
            }
        }
        return End();
    }

    void Insert(Iterator it, const T& x)
    {
        this->InsertBefore(it, x);
    }
    void InsertBefore(Iterator it, const T& x)
    {
        this->insert(it, x);
    }
    void InsertAfter(Iterator it, const T& x)
    {
        Iterator itAfter = it;
        std::advance(itAfter, 1);
        this->insert(itAfter, x);
    }

    void Remove(const T& x)
    {
        this->erase(std::find(this->Begin(), this->End(), x));
    }

    Iterator Remove(Iterator it)
    {
        return this->erase(it);
    }

    const T& PopBack()
    {
        const T& back = Back();
        this->pop_back();
        return back;
    }
    const T& PopFront()
    {
        const T& front = Front();
        this->pop_front();
        return front;
    }
    void PushFront(const T& x)
    {
        this->push_front(x);
    }
    void PushBack(const T& x)
    {
        this->push_back(x);
    }

    Iterator Begin()
    {
        return this->begin();
    }
    Iterator End()
    {
        return this->end();
    }
    Const_Iterator Begin() const
    {
        return this->begin();
    }
    Const_Iterator End() const
    {
        return this->end();
    }
    Reverse_Iterator RBegin()
    {
        return this->rbegin();
    }
    Reverse_Iterator REnd()
    {
        return this->rend();
    }
    Const_Reverse_Iterator RBegin() const
    {
        return this->rbegin();
    }
    Const_Reverse_Iterator REnd() const
    {
        return this->rend();
    }


    String ToString()
    {
        std::ostringstream oss;
        oss << "("; bool first = true;
        for (auto it = Begin(); it != End(); ++it)
        {
            if (!first) oss << ", ";
            oss << (*it);
            first = false;
        }
        oss <<")";
        return String(oss.str());
    }
};

#endif // LIST_H
