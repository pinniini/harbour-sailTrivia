#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include <QVector>
#include <QVariantList>

#include "difficulty.h"

class Question : public QObject
{
    Q_OBJECT
public:
    Question(QObject* parent = 0);
    Question(QString category, QString type, QString difficulty, QString question, QString correctAnswer, QVector<QString> answers);
    ~Question();

    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString difficulty READ difficulty NOTIFY difficultyChanged)
    Q_PROPERTY(QString question READ question NOTIFY questionChanged)
    Q_PROPERTY(QString correctAnswer READ correctAnswer NOTIFY correctAnswerChanged)
    Q_PROPERTY(QVariantList answers READ answers NOTIFY answersChanged)

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
     * \brief answers
     * \return
     */
    QVariantList answers() const;

signals:
    void categoryChanged();
    void typeChanged();
    void difficultyChanged();
    void questionChanged();
    void correctAnswerChanged();
    void answersChanged();

private:
    QString _category;
    QString _type;
    QString _difficulty;
    QString _question;
    QString _correctAnswer;
    QVariantList _answers;
};

#endif // QUESTION_H
