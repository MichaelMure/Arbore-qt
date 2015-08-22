#include "sharerepository.h"
#include "persist.h"
#include "object.h"
#include "directory.h"
#include "file.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QVariant>
#include <QDebug>

ShareRepository::ShareRepository(QObject *parent)
    : QObject(parent)
{
}

ShareRepository::~ShareRepository()
{
}

QVector<Share *> ShareRepository::get_all()
{
    QSqlQuery q;
    QVector<Share*> list;

    if(!q.exec("SELECT * FROM `Share`"))
    {
        qDebug() << "Sql error: " << q.lastError();
        return list;
    }

    while(q.next())
    {
        list << build_share(&q);
    }

    foreach (Share *share, list)
    {
        build_objects(share);
    }

    return list;
}

Share *ShareRepository::get(int id)
{
    Q_ASSERT(id >= 0);

    QSqlQuery q;
    q.prepare("SELECT * FROM `Share` WHERE id = :id");
    q.bindValue(":id", id);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
        return NULL;
    }

    if(!q.next())
    {
        qDebug() << "No Share found with id: " << id;
        return NULL;
    }

    Share *share = build_share(&q);
    build_objects(share);

    return share;
}

void ShareRepository::persist(Share *entity)
{
    if(entity->id_ > 0)
    {
        update(entity);
    }
    else
    {
        insert(entity);
    }
}

void ShareRepository::insert(Share *entity)
{
    QSqlDatabase database = QSqlDatabase::database();
    database.transaction();

    QSqlQuery q;
    q.prepare("INSERT INTO `Share` (title, description, path, creation_date, starred, state)"
            "VALUES (:title, :description, :path, :creation_date, :starred, :state);");
    q.bindValue(":title", entity->title_);
    q.bindValue(":description", entity->description_);
    q.bindValue(":path", entity->path_.absolutePath());
    q.bindValue(":creation_date", entity->creation_date_.toTime_t());
    q.bindValue(":starred", entity->starred_);
    q.bindValue(":state", entity->state_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
        return;
    }

    entity->id_ = q.lastInsertId().toInt();

    Q_ASSERT(entity->id_ >= 0);

    q.finish();

    insert_objects(entity);

    database.commit();
}

void ShareRepository::update(Share *entity)
{
    Q_ASSERT(entity->id_ >= 0);

    QSqlDatabase database = QSqlDatabase::database();
    database.transaction();

    QSqlQuery q;
    q.prepare("UPDATE `Share` SET "
              "title = :title, "
              "description = :description, "
              "path = :path, "
              "creation_date = :creation_date, "
              "starred = :starred, "
              "state = :state "
              "WHERE id = :id ;");
    q.bindValue(":id", entity->id_);
    q.bindValue(":title", entity->title_);
    q.bindValue(":description", entity->description_);
    q.bindValue(":path", entity->path_.absolutePath());
    q.bindValue(":creation_date", entity->creation_date_.toTime_t());
    q.bindValue(":starred", entity->starred_);
    q.bindValue(":state", entity->state_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
    }

    q.finish();

    clear_objects(entity);
    insert_objects(entity);

    database.commit();
}

void ShareRepository::remove(Share *entity)
{
    Q_ASSERT(entity->id_ >= 0);

    QSqlDatabase database = QSqlDatabase::database();
    database.transaction();

    clear_objects(entity);

    QSqlQuery q;
    q.prepare("DELETE FROM `Share` WHERE id = :id");
    q.bindValue(":id", entity->id_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
    }

    q.finish();
    database.commit();
}

Share* ShareRepository::build_share(QSqlQuery *q)
{
    Share *share = new Share();
    share->id_            = q->value("id").toInt();
    share->title_         = q->value("title").toString();
    share->description_   = q->value("description").toString();
    share->path_          = QDir(q->value("path").toString());
    share->creation_date_.setTime_t(q->value("creation_date").toUInt());
    share->starred_       = q->value("starred").toBool();
    share->state_         = (ShareState) q->value("state").toInt();

    return share;
}

void ShareRepository::build_objects(Share *share)
{
    QSqlQuery q;
    q.prepare("SELECT * FROM `Object` WHERE share = :share");
    q.bindValue(":share", share->id_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
        return;
    }

    while(q.next())
    {
        IpfsHash hash = q.value("hash").toString();

        share->add_hash(
            hash,
            (Object::ObjectType) q.value("type").toInt()
        );
    }
}

void ShareRepository::clear_objects(Share *entity)
{
    QSqlQuery q;

    q.prepare("DELETE FROM `Object` WHERE share = :share");
    q.bindValue(":share", entity->id_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
    }

    q.finish();
}

void ShareRepository::insert_objects(Share *entity)
{
    Q_ASSERT(entity->id_ >= 0);

    QSqlQuery q;

    foreach (Object* obj, entity->objects_)
    {
        q.prepare("INSERT INTO `Object`(`hash`,`type`,`share`) "
                  "VALUES (:hash, :type, :share);");
        q.bindValue(":hash", obj->hash().ToString());
        q.bindValue(":type", (int) obj->type());
        q.bindValue(":share", entity->id_);

        if(!q.exec())
        {
            qDebug() << "Sql error: " << q.lastError();
            return;
        }
    }

    q.finish();
}
