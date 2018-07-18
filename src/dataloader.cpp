#include <QNetworkRequest>
#include <QJsonDocument>

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
}

/*!
 * \brief DataLoader::~DataLoader
 */
DataLoader::~DataLoader()
{
    if(_manager)
    {
        delete _manager;
        _manager = 0;
    }
}

/*!
 * \brief DataLoader::loadCategories
 */
void DataLoader::loadCategories()
{
    // Start loading.
    setLoadingStatus(true);

    QNetworkRequest request(_categoriesUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "sailfish/pinniini/sailtrivia");
    _reply = _manager->get(request);

    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorLoadingData(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(finished()), this, SLOT(categoriesFinished()));
}

/*!
 * \brief Load questions from the api with given options. Emits questionsLoaded-signal when done.
 * \param questionCount - How many questions is requested.
 * \param categoryId - In which category the questions should be. -1 means all categories should be requested.
 * \param difficulty - How difficult the requested questions should be. This will be casted as Difficulty::DiffEnum.
 */
void DataLoader::loadQuestions(int questionCount, int categoryId, int difficulty)
{
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
    // Loading done.
    setLoadingStatus(false);

    // Check for errors. These should be already reported by error-signal.
    if (_reply->error() != QNetworkReply::NoError)
    {
        cleanQuestionsRequest();
        return;
    }

    qDebug() << "Categories loaded...";
    //QJsonDocument doc = QJsonDocument::fromBinaryData(_reply->readAll());

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
