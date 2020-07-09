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
    _initialTokenLoaded = false;
    _initialCategoriesLoaded = false;
    _initialCategoriesData = "";
    _manager = new QNetworkAccessManager(this);
    _categoriesUrl = QUrl("https://opentdb.com/api_category.php");
    _questionsBaseUrl = "https://opentdb.com/api.php?";
    _reply = 0;
    _timeoutTimer = new QTimer();
    _timeoutTimer->setInterval(10000); // 5 seconds timeout by default.
    _timeoutTimer->setSingleShot(true);
    _sessionToken = "";
    _sessionTokenUrl = "https://opentdb.com/api_token.php?command=request";
    _sessionTokenReply = 0;
    _initialDataLoading = true;

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
    switch (_manager->networkAccessible())
    {
    case QNetworkAccessManager::UnknownAccessibility:
        qDebug() << "Network accessibility unknown...";
        break;
    case QNetworkAccessManager::NotAccessible:
        qDebug() << "Network not accessible...";
        break;
    case QNetworkAccessManager::Accessible:
        qDebug() << "Network accessible...";
        break;
    }

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

void DataLoader::loadInitialData()
{
    _initialDataLoading = true;
    _initialCategoriesLoaded = false;
    _initialTokenLoaded = false;
    loadSessionToken();
}

void DataLoader::stopInitialLoading()
{
    _initialDataLoading = false;

    if (_reply && !_reply->isFinished())
    {
        _reply->abort();
        cleanCategoriesRequest();
    }

    if (_sessionTokenReply && !_sessionTokenReply->isFinished())
    {
        _sessionTokenReply->abort();
        cleanSessionTokenRequest();
    }

    emit initialDataLoaded(_initialCategoriesData);
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
        cleanCategoriesRequest();
        return;
    }

    qDebug() << "Categories loaded...";

    // Temp for data.
    QString categoryData = QString::fromLatin1(_reply->readAll());

    // Clear initial categories data just for safety.
    _initialCategoriesData = "";

    if (!_initialCategoriesLoaded)
    {
        _initialCategoriesData = categoryData;
    }

    // Clean stuff.
    cleanCategoriesRequest();

    _initialCategoriesLoaded = true;

    // If this was initial data loading.
    if (_initialDataLoading)
    {
        _initialDataLoading = false;
        emit initialDataLoaded(_initialCategoriesData);
        return;
    }

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
    // Stop the timer.
    if (_timeoutTimer && _timeoutTimer->isActive())
    {
        _timeoutTimer->stop();
    }

    QString errMsg = "Error loading data.";
    if (_reply)
    {
        errMsg = _reply->errorString();

        cleanCategoriesRequest();
    }

    // If initial data loading and OperationCanceled-error -> retry.
    if (_initialDataLoading && error == QNetworkReply::OperationCanceledError)
    {
        qDebug() << "Error loading data. _initialDataLoading=true & error=OperationCanceledError...";
        _initialDataLoading = false;
        loadCategories();
        return;
    }

    // Just for safety.
    _initialDataLoading = false;

    emit dataLodingErrorOccured(errMsg);
}

/*!
 * \brief DataLoader::downloadTimeout
 */
void DataLoader::downloadTimeout()
{
    if (!_reply->isFinished())
    {
        disconnectRequest();
        _reply->abort();
        delete _reply;
        _reply = 0;

        setLoadingStatus(false);
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

        _initialTokenLoaded = true;

        emit sessionTokenLoaded(_sessionToken);

        // If still loading initial data, load initial categories as well.
        if (_initialDataLoading)
        {
            loadCategories();
            return;
        }
    }
    else // Got something funky instead of session token.
    {
        // Clean the reply.
        cleanSessionTokenRequest();

        QString errorMessage = "Got something unexpected when loading session token.";

        _initialTokenLoaded = true;

        emit sessionTokenLoadingError(errorMessage);

        // If still loading initial data, load initial categories as well.
        if (_initialDataLoading)
        {
            loadCategories();
        }
    }
}

void DataLoader::errorLoadingSessionToken(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)

    QString errorMessage;

    // Check that the reply exists.
    if (_sessionTokenReply)
    {
        errorMessage = _sessionTokenReply->errorString();

        // Clean the reply.
        cleanSessionTokenRequest();
    }

    // Just for safety.
    _initialTokenLoaded = true;

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

void DataLoader::disconnectRequest()
{
    if (_reply)
    {
        disconnect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
        disconnect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));
        disconnect(_reply, SIGNAL(finished()), this, SLOT(questionsFinished()));
    }
}
