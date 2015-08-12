#ifndef SHAREREPOSITORY_H
#define SHAREREPOSITORY_H

#include <QObject>
#include "share.h"

class QSqlQuery;

class ShareRepository : public QObject
{
    Q_OBJECT
public:
    explicit ShareRepository(QObject *parent = 0);
    virtual ~ShareRepository();

    QVector<Share*> get_all();
    Share*          get   (int id);
    void            persist(Share *entity);
    void            insert(Share *entity);
    void            update(Share *entity);
    void            remove(Share *entity);

private:
    Share* build_share(QSqlQuery *query);
    void build_objects(Share* share);
    void clear_objects(Share *entity);
    void insert_objects(Share *entity);
};

#endif // SHAREREPOSITORY_H
