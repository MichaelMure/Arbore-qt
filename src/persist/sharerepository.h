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

    /**
     * Instantiate a Share from a record in database
     */
    Share* get (int id);

    /**
     * Retrieve and instanciate all the Share present in the database
     */
    QVector<Share*> get_all();

    /**
     * Make sure that the entity is up do date in the database, by either inserting or updating it.
     */
    void persist(Share *entity);

    /**
     * Insert a newly created entity in database
     */
    void insert(Share *entity);

    /**
     * Update a record in database
     */
    void update(Share *entity);

    /**
     * Delete a record in database
     */
    void remove(Share *entity);

private:
    Share* build_share(QSqlQuery *query);
    void build_objects(Share* share);
    void clear_objects(Share *entity);
    void insert_objects(Share *entity);
};

#endif // SHAREREPOSITORY_H
