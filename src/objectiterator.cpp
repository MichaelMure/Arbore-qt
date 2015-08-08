#include <cassert>

#include "directory.h"
#include "objectiterator.h"

ObjectIterator::ObjectIterator(Object *obj)
    : depth_(0), current_(obj)
{
    this->stack_.push({obj, 0});

    Directory *dir = dynamic_cast<Directory*>(obj);
    if(dir)
    {
        const QHash<QString, Child*> childs = dir->getChilds();
        for(QHash<QString, Child*>::const_iterator i = childs.constBegin(); i != childs.constEnd(); i++)
        {
            this->stack_.push({i.value()->object, 1});
        }
    }
}

ObjectIterator::ObjectIterator()
    : current_(NULL)
{
}

ObjectIterator& ObjectIterator::operator++()
{
    advance();
    return *this;
}

ObjectIterator ObjectIterator::operator++(int)
{
    ObjectIterator tmp(*this);
    advance();
    return tmp;
}

bool ObjectIterator::operator==(const ObjectIterator &rhs) const
{
    return this->current_ == rhs.current_;
}

bool ObjectIterator::operator!=(const ObjectIterator &rhs) const
{
    return this->current_ != rhs.current_;
}

Object &ObjectIterator::operator*() const
{
    return *this->current_;
}

Object &ObjectIterator::operator->() const
{
    return *this->current_;
}

int ObjectIterator::depth() const
{
    return this->depth_;
}

void ObjectIterator::advance()
{
    assert(!this->stack_.empty());

    Node next;
    do
    {
        next = this->stack_.pop();
    }
    while (this->set_.contains(next.object->hash()) && !this->stack_.isEmpty());

    if(this->stack_.isEmpty())
    {
        this->depth_ = -1;
        this->current_ = NULL;
        return;
    }

    Directory *dir = dynamic_cast<Directory*>(next.object);
    if(dir)
    {
        const QHash<QString, Child*> childs = dir->getChilds();
        for(QHash<QString, Child*>::const_iterator i = childs.constBegin(); i != childs.constEnd(); i++)
        {
            this->stack_.push({i.value()->object, next.depth + 1});
        }
    }

    this->set_.insert(next.object->hash());
    this->depth_ = next.depth;
    this->current_ = next.object;
}

