#ifndef SHAREREPOSITORY_H
#define SHAREREPOSITORY_H

#include <QObject>
#include "share.h"

class ShareRepository : public QObject
{
    Q_OBJECT
public:
    explicit ShareRepository(QObject *parent = 0);
    virtual ~ShareRepository();

    virtual Share* get   (int id);
    virtual void   insert(Share *entity);
    virtual void   update(Share *entity);
    virtual void   remove(Share *entity);

private:
    void clear_objects(Share *entity);
    void insert_objects(Share *entity);
};

#endif // SHAREREPOSITORY_H
