#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

class DataLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataLoader(QObject *parent = nullptr);
    ~DataLoader();

    /*!
     * Stuff.
     */
    Q_INVOKABLE void loadCategories();

signals:
    void categoriesLoaded();

public slots:

private slots:
    void categoriesFinished();

private:
    QNetworkAccessManager* _manager;
    QUrl _categoriesUrl;
    QNetworkReply* _reply;
};

#endif // DATALOADER_H
