#include <QNetworkRequest>
#include <QNetworkConfiguration>
#include <QJsonDocument>
#include <QJsonObject>

#include "dataloader.h"

/*!
 * \brief DataLoader::DataLoader
 * \param parent
 */
DataLoader::DataLoader(QObject *parent) : QObject(parent)
{
    _loading = false;
    _manager = new QNetworkAccessManager(this);
    _categoriesUrl = QUrl("https://opentdb.com/api_category.php");
    _questionsBaseUrl = "https://opentdb.com/api.php?";
    _timeoutTimer = new QTimer();
    _timeoutTimer->setInterval(5000); // 5 seconds timeout by default.
    _timeoutTimer->setSingleShot(true);
    _sessionToken = "";
    _sessionTokenUrl = "https://opentdb.com/api_token.php?command=request";

    // Connect timeout timer timeout.
    connect(_timeoutTimer, SIGNAL(timeout()), this, SLOT(downloadTimeout()));
}

/*!
 * \brief DataLoader::~DataLoader
 */
DataLoader::~DataLoader()
{
    if (_reply)
    {
        delete _reply;
        _reply = 0;
    }

    if (_manager)
    {
        delete _manager;
        _manager = 0;
    }

    // Session token.
    if (_sessionTokenReply)
    {
        delete _sessionTokenReply;
        _sessionTokenReply = 0;
    }

    // Delete timer.
    if (_timeoutTimer)
    {
        // Stop the timer if it is still running.
        if (_timeoutTimer->isActive())
        {
            _timeoutTimer->stop();
        }

        disconnect(_timeoutTimer, SIGNAL(timeout()), this, SLOT(downloadTimeout()));
        delete _timeoutTimer;
        _timeoutTimer = 0;
    }
}

/*!
 * \brief DataLoader::loadCategories
 */
void DataLoader::loadCategories()
{
    // Stop timer if it is for some reason still running.
    if (_timeoutTimer->isActive())
    {
        _timeoutTimer->stop();
    }

    // Start loading.
    setLoadingStatus(true);

    QNetworkRequest request(_categoriesUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "sailfish/pinniini/sailtrivia");
    _reply = _manager->get(request);

    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));

    // Start the timeout.
    _timeoutTimer->start();
}

/*!
 * \brief Load questions from the api with given options. Emits questionsLoaded-signal when done.
 * \param questionCount - How many questions is requested.
 * \param categoryId - In which category the questions should be. -1 means all categories should be requested.
 * \param difficulty - How difficult the requested questions should be. This will be casted as Difficulty::DiffEnum.
 */
void DataLoader::loadQuestions(int questionCount, int categoryId, int difficulty)
{
    // Stop timer if it is for some reason still running.
    if (_timeoutTimer->isActive())
    {
        _timeoutTimer->stop();
    }

    // Just in case.
    setLoadingStatus(false);

    QString query = _questionsBaseUrl;

    // Check questionCount parameter, should be between 1 and 50.
    if(questionCount < 1 || questionCount > 50)
    {
        emit invalidParameters(QString("Invalid question count."));
        return;
    }

    // Add question amount.
    query += "amount=" + QString::number(questionCount);

    // Check the category, should be greater or equal to -1.
    if(categoryId < -1)
    {
        emit invalidParameters(QString("Invalid category."));
        return;
    }
    else if (categoryId != -1) // Some category, not All.
    {
        // Add category to query.
        query += "&category=" + QString::number(categoryId);
    }

    // Cast the given difficulty to Difficulty::DiffEnum.
    Difficulty::DiffEnum diff = static_cast<Difficulty::DiffEnum>(difficulty);

    // Check difficulty, should not be empty. Only valid values are: easy, medium, hard.
    QString diffQuery = "&difficulty=";
    bool validDifficulty = true;
    switch (diff)
    {
        case Difficulty::All:
            diffQuery = "";
            break;
        case Difficulty::Easy:
            diffQuery += "easy";
            break;
        case Difficulty::Medium:
            diffQuery += "medium";
            break;
        case Difficulty::Hard:
            diffQuery += "hard";
            break;
        default:
            validDifficulty = false;
            break;
    }

    // Invalid difficulty.
    if(!validDifficulty)
    {
        emit invalidParameters(QString("Invalid difficulty."));
        return;
    }
    else
    {
        query += diffQuery;
    }

    // Session token.
    if (!_sessionToken.isEmpty())
    {
        query += ("&token=" + _sessionToken);
    }

    qDebug() << "Questions query: " << query;

    // Parameters checked, start loading.
    setLoadingStatus(true);

    // Send request.
    QUrl url = QUrl(query);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "sailfish/pinniini/sailtrivia");
    _reply = _manager->get(request);

    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(finished()), this, SLOT(questionsFinished()));

    // Start the timeout.
    _timeoutTimer->start();
}

void DataLoader::loadSessionToken()
{
    // If the reply does not exists or the reply exists and is finished.
    if (!_sessionTokenReply || (_sessionTokenReply && _sessionTokenReply->isFinished()))
    {
        // Clear current session token.
        _sessionToken = "";

        QNetworkRequest request(_sessionTokenUrl);
        request.setHeader(QNetworkRequest::UserAgentHeader, "sailfish/pinniini/sailtrivia");
        _sessionTokenReply = _manager->get(request);

        connect(_sessionTokenReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingSessionToken(QNetworkReply::NetworkError)));
        connect(_sessionTokenReply, SIGNAL(finished()), this, SLOT(sessionTokenFinished()));
    }
}

/*!
 * \brief DataLoader::loading
 * \return Status whether or not the loader is still loading data.
 */
bool DataLoader::loading() const
{
    return _loading;
}


// ------------
// Slots

/*!
 * \brief DataLoader::categoriesFinished
 */
void DataLoader::categoriesFinished()
{
    // Stop the timer.
    if (_timeoutTimer && _timeoutTimer->isActive())
    {
        _timeoutTimer->stop();
    }

    // Loading done.
    setLoadingStatus(false);

    // Check for errors. These should be already reported by error-signal.
    if (_reply->error() != QNetworkReply::NoError)
    {
        cleanQuestionsRequest();
        return;
    }

    qDebug() << "Categories loaded...";

    // Temp for data.
    QString categoryData = QString::fromLatin1(_reply->readAll());

    // Clean stuff.
    disconnect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    disconnect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));
    delete _reply;
    _reply = 0;

    emit categoriesLoaded(categoryData);
}

/*!
 * \brief DataLoader::questionsFinished
 */
void DataLoader::questionsFinished()
{
    // Stop the timer.
    if (_timeoutTimer && _timeoutTimer->isActive())
    {
        _timeoutTimer->stop();
    }

    // Questions loading done.
    setLoadingStatus(false);

    // Check for errors. These should be already reported by error-signal.
    if (_reply->error() != QNetworkReply::NoError)
    {
        cleanQuestionsRequest();
        return;
    }

    qDebug() << "Questions loaded...";

    // Temp for data.
    QString questionData = QString::fromLatin1(_reply->readAll());

    // Clean stuff.
    disconnect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    disconnect(_reply, SIGNAL(finished()), this, SLOT(questionsFinished()));
    delete _reply;
    _reply = 0;

    emit questionsLoaded(questionData);
}

/*!
 * \brief DataLoader::errorLoadingData
 * \param error
 */
void DataLoader::errorLoadingData(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    QString errMsg = _reply->errorString();
    emit dataLodingErrorOccured(errMsg);
}

/*!
 * \brief DataLoader::downloadTimeout
 */
void DataLoader::downloadTimeout()
{
    if (!_reply->isFinished())
    {
        _reply->abort();
        QString errMsg = "Timeout occured while loading data.";
        emit dataLodingErrorOccured(errMsg);
    }
}

void DataLoader::sessionTokenFinished()
{
    // Check for errors. These should be already reported by error-signal.
    if (_sessionTokenReply->error() != QNetworkReply::NoError)
    {
        return;
    }

    qDebug() << "Session token loaded...";

    // Read json.
    QJsonDocument doc(QJsonDocument::fromJson(_sessionTokenReply->readAll()));
    QJsonObject json = doc.object();

    // Read the token.
    if (json.contains("token") && json.value("token").isString())
    {
        // All is well in the world.
        _sessionToken = json.value("token").toString();

        // Clean the reply.
        cleanSessionTokenRequest();

        emit sessionTokenLoaded(_sessionToken);
    }
}

void DataLoader::errorLoadingSessionToken(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    QString errorMessage;

    // Check that the reply exists.
    if (_sessionTokenReply)
    {
        errorMessage = _sessionTokenReply->errorString();
    }
    emit sessionTokenLoadingError(errorMessage);
}

// ------------
// Private functions.

void DataLoader::setLoadingStatus(const bool status)
{
    // Only emit the change if status actually changes.
    if(status != _loading)
    {
        _loading = status;
        emit loadingChanged();
    }
}

void DataLoader::cleanCategoriesRequest()
{
    // Clean stuff.
    disconnect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    disconnect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));
    delete _reply;
    _reply = 0;
}

void DataLoader::cleanQuestionsRequest()
{
    // Clean stuff.
    disconnect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    disconnect(_reply, SIGNAL(finished()), this, SLOT(questionsFinished()));
    delete _reply;
    _reply = 0;
}

void DataLoader::cleanSessionTokenRequest()
{
    // Clean stuff.
    disconnect(_sessionTokenReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingSessionToken(QNetworkReply::NetworkError)));
    disconnect(_sessionTokenReply, SIGNAL(finished()), this, SLOT(sessionTokenFinished()));
    delete _sessionTokenReply;
    _sessionTokenReply = 0;
}
