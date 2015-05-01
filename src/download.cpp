#include "download.h"


Download::Download(QObject *parent):
    QObject(parent)
{
}

Download::Download(QString name, DownloadState state, float progress, int size, QObject *parent):
    QObject(parent),
    name_(name),
    state_(state),
    progress_(progress),
    size_(size)
{
}

const QString& Download::name() const
{
    return name_;
}

DownloadState Download::state() const
{
    return state_;
}

float Download::progress() const
{
    return progress_;
}

int Download::size() const
{
    return size_;
}

