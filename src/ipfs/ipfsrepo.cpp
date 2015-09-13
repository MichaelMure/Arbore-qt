#include "ipfsrepo.h"
#include "ipfs/ipfs.h"

#include <QUrl>

const QString API_COMMAND = "pin";

IpfsRepo::IpfsRepo(QObject *parent) : QObject(parent)
{
}

void IpfsRepo::gc()
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/gc");
    Ipfs::instance()->query(url);
}

