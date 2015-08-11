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
    // Singleton
    static Persist& instance();

    // Repository access
    ShareRepository share;

    QSqlQuery exec(const QString &query);

private:
    Persist();                      // hide constructor
    ~Persist();                     // hide destructor
    Persist(Persist const&);        // hide copy constructor
    void operator=(Persist const&); // hide assign op

    void init();
    void open_database();
    void init_database();

    QString db_type_;
};

#endif // PERSIST_H
