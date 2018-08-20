#include <QVariant>
#include "question.h"

Question::Question(QObject* parent) : QObject(parent)
{

}

Question::Question(QString category, QString type, QString difficulty, QString question, QString correctAnswer, QVector<QString> answers)
    : _category(category), _type(type), _difficulty(difficulty), _question(question), _correctAnswer(correctAnswer)
{
    foreach (QString answer, answers)
    {
        _answers.append(QVariant::fromValue(answer));
    }
}

Question::~Question()
{

}

QString Question::category() const
{
    return _category;
}

QString Question::type() const
{
    return _type;
}

QString Question::difficulty() const
{
    return _difficulty;
}

QString Question::question() const
{
    return _question;
}

QString Question::correctAnswer() const
{
    return _correctAnswer;
}

QVariantList Question::answers() const
{
    return _answers;
}
