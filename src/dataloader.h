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
     *
     */
    Q_INVOKABLE void loadCategories();

    Q_INVOKABLE void loadQuestions(int questionCount, int categoryId, string difficulty);

signals:

    /*!
     * \brief Signal to tell when the categories json is loaded.
     * \param Categories data as a json string.
     */
    void categoriesLoaded(const QString& categoriesData);

    /*!
     * \brief Signal to tell when the questions json is loaded.
     * \param Questions data as a json string.
     */
    void questionsLoaded(const QString& questionData);

public slots:

private slots:
    void categoriesFinished();

private:
    QNetworkAccessManager* _manager;
    QUrl _categoriesUrl;
    QNetworkReply* _reply;
};

#endif // DATALOADER_H
