#include "persist.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

static bool initialized = false;

Persist &Persist::instance()
{
    static Persist instance;
    if(!initialized)
    {
        initialized = true;
        instance.init();
    }
    return instance;
}

QSqlQuery Persist::exec(const QString &query)
{
    return db_.exec(query);
}

Persist::Persist()
{
}

Persist::~Persist()
{
}

void Persist::init()
{
    open_database();
}

void Persist::open_database()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if(!dir.exists())
        dir.mkpath(".");

    QString path = dir.absolutePath() +
            QDir::separator() + "arbore.db3";

    qDebug() << "Database path: " << path;

    db_.setDatabaseName(path);

    if(!db_.open())
        qDebug() << db_.lastError();

    QSqlQuery q;

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
    db_.transaction();

    db_.exec("CREATE TABLE `Version` (`version`	INTEGER NOT NULL);");

    db_.exec("CREATE TABLE `Share` ("
             "	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
             "	`title`	TEXT NOT NULL,"
             "	`description`	TEXT,"
             "	`path`	TEXT NOT NULL,"
             "	`creation_date`	INTEGER NOT NULL,"
             "	`starred`	INTEGER NOT NULL,"
             "	`state`	INTEGER NOT NULL"
             ");");

    db_.exec("CREATE TABLE `Object` ("
             "	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
             "	`hash`	INTEGER NOT NULL,"
             "	`type`	INTEGER NOT NULL,"
             "	`share`	INTEGER NOT NULL,"
             "	FOREIGN KEY(`share`) REFERENCES Share(id)"
             ");");
    db_.exec("CREATE INDEX `share_id` ON `Share` (`id` ASC);");

    db_.exec("INSERT INTO `Version` VALUES (1);");

    db_.commit();
}

