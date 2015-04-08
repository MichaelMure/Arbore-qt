#include "download.h"


Download::Download(QObject *parent):
    QObject(parent)
{
}

Download::Download(QString name, float progress, int size, QObject *parent):
    QObject(parent),
    name_(name),
    progress_(progress),
    size_(size)
{
}

const QString& Download::name() const
{
    return name_;
}

float Download::progress() const
{
    return progress_;
}

int Download::size() const
{
    return size_;
}

