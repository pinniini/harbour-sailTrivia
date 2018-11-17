#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QJsonObject>

#include "category.h"

class CategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief The CategoryRoles enum
     */
    enum CategoryRoles
    {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2
    };

    /*!
     * \brief CategoryModel
     * \param parent
     */
    explicit CategoryModel(QObject *parent = 0);

    ~CategoryModel();

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
    QVector<Category> *_categories;

    void readJson(const QJsonObject &object);
    void readCategory(const QJsonObject &json);
};

#endif // CATEGORYMODEL_H
