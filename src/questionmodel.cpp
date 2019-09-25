#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QtQml>

#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "questionmodel.h"
#include "jsonconstants.h"

using namespace JsonConstants;

/*!
 * \brief QuestionModel::QuestionModel Constructor
 * \param parent
 */
QuestionModel::QuestionModel(QObject *parent) : QAbstractListModel(parent)
{
    std::srand(unsigned(std::time(0)));

    _questions = new QVector<Question*>();
    _lastError = "";
    _responseCode = -1;
    _currentQuestionIndex = -1;
}

/*!
 * \brief QuestionModel::~QuestionModel Destructor
 */
QuestionModel::~QuestionModel()
{
    for(int i = 0; i < _questions->length(); ++i)
    {
        delete _questions->at(i);
        (*_questions)[i] = 0;
    }

    _questions->clear();
    delete _questions;
    _questions = 0;
}

/*!
 * \brief QuestionModel::rowCount
 * \param parent
 * \return
 */
int QuestionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (_questions != 0)
    {
        return _questions->length();
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief QuestionModel::data
 * \param index
 * \param role
 * \return
 */
QVariant QuestionModel::data(const QModelIndex &index, int role) const
{
    // Validate index.
    if (index.isValid())
    {
        switch (role) {
        case CategoryRole:
            return QVariant(_questions->at(index.row())->category());
        case TypeRole:
            return QVariant(_questions->at(index.row())->type());
        case DifficultyRole:
            return QVariant(_questions->at(index.row())->difficulty());
        case QuestionRole:
            return QVariant(_questions->at(index.row())->question());
        case CorrectRole:
            return QVariant(_questions->at(index.row())->correctAnswer());
        case AnswersRole:
            return QVariant::fromValue(_questions->at(index.row())->answers());
        default:
            return QVariant();
        }
    }
    else // Invalid index, return empty.
    {
        return QVariant();
    }
}

/*!
 * \brief QuestionModel::roleNames
 * \return
 */
QHash<int, QByteArray> QuestionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CategoryRole] = "category";
    roles[TypeRole] = "type";
    roles[DifficultyRole] = "difficulty";
    roles[QuestionRole] = "question";
    roles[CorrectRole] = "correct";
    roles[AnswersRole] = "answers";
    return roles;
}

Question* QuestionModel::get(int index)
{
    if (_questions == 0 || index < 0 || index > _questions->length() - 1)
    {
        return 0;
    }

    _currentQuestionIndex = index;

    return _questions->at(index);
}

/*!
 * \brief QuestionModel::fillModel
 * \param json
 * \return
 */
bool QuestionModel::fillModel(const QString &json)
{
    _lastError = "";
    _responseCode = -1;

    // Clear old data.
    beginResetModel();

    for (int i = 0; i < _questions->length(); ++i)
    {
        delete _questions->at(i);
        (*_questions)[i] = 0;
    }

    _questions->clear();
    endResetModel();

    QJsonDocument document(QJsonDocument::fromJson(json.toLatin1()));
    return readJson(document.object());
}

Question* QuestionModel::getCurrentQuestion() const
{
    qDebug() << "Current question index: " << _currentQuestionIndex;

    if (_questions == 0 || _currentQuestionIndex < 0 || _currentQuestionIndex > _questions->length() - 1)
    {
        qDebug() << "Current question not found...";
        return 0;
    }

    qDebug() << "Current question: " << _questions->at(_currentQuestionIndex)->question();

    return _questions->at(_currentQuestionIndex);
}

void QuestionModel::clearCurrentIndex()
{
    _currentQuestionIndex = -1;
}

QString QuestionModel::getLastError() const
{
    return _lastError;
}

int QuestionModel::getLastResponseCode() const
{
    return _responseCode;
}

/*!
 * \brief QuestionModel::readJson
 * \param object
 */
bool QuestionModel::readJson(const QJsonObject &object)
{
    // JSON contains response code and results.
    if (object.contains("response_code") && object.value("response_code").isDouble()
            && object.contains("results") && object.value("results").isArray())
    {
        bool error = false;

        // Check response code.
        int responseCode = object.value("response_code").toInt(-1);
        _responseCode = responseCode;

        switch (responseCode) {
        case 0:
            qDebug() << "Everything went fine.";
            break;
        case 1:
            _lastError = "Not enough questions, change parameters.";
            error = true;
            break;
        case 2:
            _lastError = "Query parameters are not valid.";
            error = true;
            break;
        case 3:
            _lastError = "Token not found.";
            error = true;
            break;
        case 4:
            _lastError = "Token empty, change parameters.";
            error = true;
            break;
        default:
            _lastError = "Something went horribly wrong.";
            error = true;
            break;
        }

        // Return if the response code indicates error on loaded data.
        if (error)
        {
            return false;
        }

        // Questions.
        QJsonArray questions = object.value("results").toArray();
        qDebug() << "JSON questions count: " << questions.size();

        // Begin inserts.
        beginInsertRows(QModelIndex(), 0, questions.size() - 1);

        for (int i = 0; i < questions.size(); ++i)
        {
            QJsonObject quest = questions.at(i).toObject();
            readQuestion(quest);
        }

        // End inserts.
        endInsertRows();

        return true;
    }
    else // No response code -> something is wrong with the data.
    {
        // TODO: response code missing from questions JSON.
        _lastError = "No response code.";
        return false;
    }

    _lastError = "Invalid data.";
    return false;
}

/*!
 * \brief QuestionModel::readQuestion
 * \param json
 */
void QuestionModel::readQuestion(const QJsonObject &json)
{
    // Check all necessary fields.
    if (json.contains(JsonCategory) && json.value(JsonCategory).isString()
            && json.contains(JsonType) && json.value(JsonType).isString()
            && json.contains(JsonDifficulty) && json.value(JsonDifficulty).isString()
            && json.contains(JsonQuestion) && json.value(JsonQuestion).isString()
            && json.contains(JsonCorrect) && json.value(JsonCorrect).isString()
            && json.contains(JsonIncorrect) && json.value(JsonIncorrect).isArray())
    {
        QString category = json.value(JsonCategory).toString();
        QString type = json.value(JsonType).toString();
        QString difficulty = json.value(JsonDifficulty).toString();
        QString quest = json.value(JsonQuestion).toString();
        QString correct = json.value(JsonCorrect).toString();

        QVector<QString> answers;
        answers.push_back(correct);

        // Incorrect answers.
        QJsonArray answs = json.value(JsonIncorrect).toArray();
        for (int j = 0; j < answs.size(); ++j)
        {
            answers.push_back(answs.at(j).toString());
        }

        // Sort the answers if the question type is boolean (True/False) and shuffle otherwise.
        if (type == "boolean")
        {
            std::sort(answers.begin(), answers.end());
        }
        else
        {
            std::random_shuffle(answers.begin(), answers.end());
        }

        Question *que = new Question(category, type, difficulty, quest, correct, answers);

        // This is necessary, so that the js engine won't garbage collect the object after using get-method.
        // Because the ownership moves to the js engine if we return the object from here to there.
        QQmlEngine::setObjectOwnership(que, QQmlEngine::CppOwnership);

        _questions->push_back(que);
    }
}
