#include <QNetworkRequest>
#include <QJsonDocument>

#include "dataloader.h"

DataLoader::DataLoader(QObject *parent) : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
    _categoriesUrl = QUrl("https://opentdb.com/api_category.php");
}

DataLoader::~DataLoader()
{
    if(_manager)
    {
        delete _manager;
        _manager = 0;
    }
}

void DataLoader::loadCategories()
{
    QNetworkRequest request(_categoriesUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "sailfish/pinniini/sailtrivia");
    _reply = _manager->get(request);

    connect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));
}

// ------------
// Slots

void DataLoader::categoriesFinished()
{
    qDebug() << "Categories loaded...";
    //QJsonDocument doc = QJsonDocument::fromBinaryData(_reply->readAll());
    emit categoriesLoaded(QString::fromLatin1(_reply->readAll()));
}
