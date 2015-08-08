#ifndef OBJECTITERATOR_H
#define OBJECTITERATOR_H

#include <cstddef>
#include <iterator>
#include <QStack>
#include <QSet>
#include "object.h"

class ObjectIterator : public std::iterator<
        std::forward_iterator_tag,
        Object,
        std::ptrdiff_t>
{
public:
    explicit ObjectIterator(Object *obj);
    ObjectIterator(); // Default constructor gives end.

    ObjectIterator& operator++ ();
    ObjectIterator operator++ (int);

    bool operator== (const ObjectIterator& rhs) const;
    bool operator!= (const ObjectIterator& rhs) const;

    Object& operator* () const;
    Object& operator-> () const;

    int depth() const;

private:
    void advance();

private:
    struct Node
    {
        Object *object;
        int depth;
    };

    int depth_;
    Object* current_;
    QStack<Node> stack_;
    QSet<IpfsHash> set_;
};

#endif // OBJECTITERATOR_H
