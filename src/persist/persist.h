#ifndef PERSIST_H
#define PERSIST_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

#include "sharerepository.h"

class Persist : public QObject
{
    Q_OBJECT
public:
    Persist();
    virtual ~Persist();

    // Singleton
    static Persist *instance();

    // Repository access
    ShareRepository share;

private:
    Persist(Persist const&);        // hide copy constructor
    void operator=(Persist const&); // hide assign op

    void open_database();
    void init_database();

    QString db_type_;
};

#endif // PERSIST_H
