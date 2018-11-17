#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QTimer>

#include "difficulty.h"

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

    /*!
     *
     */
    Q_INVOKABLE void loadQuestions(int questionCount, int categoryId, int difficulty);

    /*!
     *
     */
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    bool loading() const;

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

    /*!
     * \brief Signal to tell that the given parameters for questions query were invalid.
     * \param Error message telling which parameter(s) were invalid.
     */
    void invalidParameters(const QString& errorMessage);

    /*!
     * \brief Signal to tell when dataloader loading status changes.
     */
    void loadingChanged();

    /*!
     * \brief dataLodingErrorOccured
     * \param errorMessage
     */
    void dataLodingErrorOccured(const QString& errorMessage);

public slots:

private slots:
    void categoriesFinished();
    void questionsFinished();
    void errorLoadingData(QNetworkReply::NetworkError error);
    void downloadTimeout();

private:
    QNetworkAccessManager* _manager;
    QUrl _categoriesUrl;
    QString _questionsBaseUrl;
    QNetworkReply* _reply;
    bool _loading;
    QTimer* _timeoutTimer;

    /*!
     * \brief setLoadingStatus
     * \param status
     */
    void setLoadingStatus(const bool status);

    /*!
     * \brief cleanCategoriesRequest
     */
    void cleanCategoriesRequest();

    /*!
     * \brief cleanQuestionsRequest
     */
    void cleanQuestionsRequest();
};

#endif // DATALOADER_H
