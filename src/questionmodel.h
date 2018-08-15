#ifndef QUESTIONMODEL_H
#define QUESTIONMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QJsonObject>

#include "question.h"

class QuestionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum QuestionRoles
    {
        CategoryRole = Qt::UserRole + 1,
        TypeRole = Qt::UserRole + 2,
        DifficultyRole = Qt::UserRole + 3,
        QuestionRole = Qt::UserRole + 4,
        CorrectRole = Qt::UserRole + 5,
        AnswersRole = Qt::UserRole + 6
    };

    explicit QuestionModel(QObject *parent = 0);

    ~QuestionModel();

    /*!
     * \brief rowCount
     * \param parent
     * \return
     */
    virtual int rowCount(const QModelIndex &parent) const;

    /*!
     * \brief data
     * \param index
     * \param role
     * \return
     */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /*!
     * \brief roleNames
     * \return
     */
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE bool fillModel(const QString &json);

private:
    QVector<Question> *_questions;

    void readJson(const QJsonObject &object);
    void readQuestion(const QJsonObject &json);
};

#endif // QUESTIONMODEL_H
