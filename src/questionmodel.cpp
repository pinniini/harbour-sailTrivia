#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include <algorithm>

#include "questionmodel.h"
#include "jsonconstants.h"

using namespace JsonConstants;

/*!
 * \brief QuestionModel::QuestionModel Constructor
 * \param parent
 */
QuestionModel::QuestionModel(QObject *parent) : QAbstractListModel(parent)
{
    _questions = new QVector<Question>();
}

/*!
 * \brief QuestionModel::~QuestionModel Destructor
 */
QuestionModel::~QuestionModel()
{
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
            return QVariant(_questions->at(index.row()).category());
        case TypeRole:
            return QVariant(_questions->at(index.row()).type());
        case DifficultyRole:
            return QVariant(_questions->at(index.row()).difficulty());
        case QuestionRole:
            return QVariant(_questions->at(index.row()).question());
        case CorrectRole:
            return QVariant(_questions->at(index.row()).correctAnswer());
        case AnswersRole:
            return QVariant::fromValue(_questions->at(index.row()).answers());
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

/*!
 * \brief QuestionModel::fillModel
 * \param json
 * \return
 */
bool QuestionModel::fillModel(const QString &json)
{
    // Clear old data.
    beginResetModel();
    _questions->clear();
    endResetModel();

    QJsonDocument document(QJsonDocument::fromJson(json.toLatin1()));
    readJson(document.object());

    return true;
}

/*!
 * \brief QuestionModel::readJson
 * \param object
 */
void QuestionModel::readJson(const QJsonObject &object)
{
    // JSON contains response code and results.
    if (object.contains("response_code") && object.value("response_code").isDouble()
            && object.contains("results") && object.value("results").isArray())
    {
        // Check response code.
        int responseCode = object.value("response_code").toInt(-1);
        if (responseCode == -1)
        {
            return;
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

        qDebug() << "QuestionsModel size: " << _questions->size();

        // End inserts.
        endInsertRows();
    }
    else // No response code -> something is wrong with the data.
    {
        // TODO: response code missing from questions JSON.
    }
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

        // Sort the answers.
        std::sort(answers.begin(), answers.end());

        _questions->push_back(Question(category, type, difficulty, quest, correct, answers));
    }
}
