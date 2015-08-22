#include "persist.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

Q_GLOBAL_STATIC(Persist, singleton)

Persist::Persist()
  :db_type_("QSQLITE")
{
    open_database();
}

Persist::~Persist()
{
}

Persist *Persist::instance()
{
    return singleton();
}

QSqlQuery Persist::exec(const QString &query)
{
    QSqlDatabase db = QSqlDatabase::database();
    return db.exec(query);
}

void Persist::open_database()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(db_type_);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if(!dir.exists())
        dir.mkpath(".");

    QString path = dir.absolutePath() +
            QDir::separator() + "arbore.db3";

    qDebug() << "Database path: " << path;

    db.setDatabaseName(path);

    if(!db.open())
        qDebug() << db.lastError();

    QSqlQuery q;

    q.exec("PRAGMA foreign_keys = ON;");
    q.exec("SELECT count(*) FROM sqlite_master;");

    if(!q.next() || q.value(0).toInt() == 0)
    {
        init_database();
        return;
    }

    q.exec("SELECT version from `Version`;");
    q.next();
    qDebug() << "Found database version " << q.value(0).toInt();
}

void Persist::init_database()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    db.exec("CREATE TABLE `Version` (`version`	INTEGER NOT NULL);");

    db.exec("CREATE TABLE `Share` ("
             "	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
             "	`title`	TEXT NOT NULL,"
             "	`description`	TEXT,"
             "	`path`	TEXT NOT NULL,"
             "	`creation_date`	INTEGER NOT NULL,"
             "	`starred`	INTEGER NOT NULL,"
             "	`state`	INTEGER NOT NULL"
             ");");

    db.exec("CREATE TABLE `Object` ("
             "	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
             "	`hash`	INTEGER NOT NULL,"
             "	`type`	INTEGER NOT NULL,"
             "	`share`	INTEGER NOT NULL,"
             "	FOREIGN KEY(`share`) REFERENCES Share(id)"
             ");");
    db.exec("CREATE INDEX `share_id` ON `Share` (`id` ASC);");

    db.exec("INSERT INTO `Version` VALUES (1);");

    db.commit();
}
