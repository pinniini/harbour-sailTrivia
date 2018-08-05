#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include <QVector>

#include "difficulty.h"

class Question
{
public:
    Question();
    Question(QString category, QString type, QString difficulty, QString question, QString correctAnswer, QVector<QString> answers);
    ~Question();

    /*!
     * \brief category
     * \return
     */
    QString category() const;

    /*!
     * \brief type
     * \return
     */
    QString type() const;

    /*!
     * \brief difficulty
     * \return
     */
    QString difficulty() const;

    /*!
     * \brief question
     * \return
     */
    QString question() const;

    /*!
     * \brief correctAnswer
     * \return
     */
    QString correctAnswer() const;

    /*!
     * \brief incorrectAnswers
     * \return
     */
    QVector<QString> answers() const;

private:
    QString _category;
    QString _type;
    QString _difficulty;
    QString _question;
    QString _correctAnswer;
    QVector<QString> _answers;
};

#endif // QUESTION_H
