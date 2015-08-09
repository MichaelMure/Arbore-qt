#include "sharerepository.h"
#include "persist.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QVariant>
#include <QDebug>
#include <cassert>

ShareRepository::ShareRepository(QObject *parent)
    : QObject(parent)
{

}

ShareRepository::~ShareRepository()
{

}

Share *ShareRepository::get(int id)
{
    assert(id >= 0);

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

    Share *share = new Share();
    share->id_            = id;
    share->title_         = q.value("title").toString();
    share->description_   = q.value("description").toString();
    share->path_          = QDir(q.value("path").toString());
    share->creation_date_.setTime_t(q.value("creation_date").toUInt());
    share->starred_       = q.value("starred").toBool();
    share->state_         = (ShareState) q.value("state").toInt();

    return share;
}

void ShareRepository::insert(Share *entity)
{
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
}

void ShareRepository::update(Share *entity)
{
    assert(entity->id_ >= 0);

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
    qDebug() << q.lastQuery();
}

void ShareRepository::remove(Share *entity)
{
    assert(entity->id_ >= 0);

    QSqlQuery q;
    q.prepare("DELETE FROM `Share` WHERE id = :id");
    q.bindValue(":id", entity->id_);

    if(!q.exec())
    {
        qDebug() << "Sql error: " << q.lastError();
    }
}
