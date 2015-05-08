#include "share.h"


Share::Share(QObject *parent):
    QObject(parent)
{
}

Share::Share(QString name, ShareState state, float progress, int size, QObject *parent):
    QObject(parent),
    name_(name),
    state_(state),
    progress_(progress),
    size_(size)
{
}

const QString& Share::name() const
{
    return name_;
}

ShareState Share::state() const
{
    return state_;
}

float Share::progress() const
{
    return progress_;
}

int Share::size() const
{
    return size_;
}

