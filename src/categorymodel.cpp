#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "categorymodel.h"

/*!
 * \brief CategoryModel::CategoryModel Constructor.
 * \param parent
 */
CategoryModel::CategoryModel(QObject *parent) : QAbstractListModel(parent)
{
    _categories = new QVector<Category>();
}

/*!
 * \brief CategoryModel::~CategoryModel Destructor.
 */
CategoryModel::~CategoryModel()
{
    _categories->clear();
    delete _categories;
    _categories = 0;
}

/*!
 * \brief CategoryModel::rowCount
 * \param parent
 * \return
 */
int CategoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (_categories != 0)
    {
        return _categories->length();
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief CategoryModel::data
 * \param index
 * \param role
 * \return
 */
QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    // Valid index, proceed.
    if (index.isValid())
    {
        switch (role) {
        case IdRole:
            return QVariant(_categories->at(index.row()).id());
        case NameRole:
            return QVariant(_categories->at(index.row()).name());
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
 * \brief CategoryModel::roleNames
 * \return
 */
QHash<int, QByteArray> CategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IdRole] = "id";
    return roles;
}

bool CategoryModel::fillModel(const QString &json)
{
    // Clear old data.
    beginResetModel();
    _categories->clear();
    endResetModel();

    QJsonDocument doc(QJsonDocument::fromJson(json.toLatin1()));
    readJson(doc.object());

    return true;
}

// --------------------------
// Private

void CategoryModel::readJson(const QJsonObject &object)
{
    // JSON contains array of results.
    if (object.contains("trivia_categories") && object.value("trivia_categories").isArray())
    {
        QJsonArray arr = object.value("trivia_categories").toArray();
        qDebug() << "JSON categories count: " << arr.size();

        beginInsertRows(QModelIndex(), 0, arr.size()); // Use size since we add extra "All" -category.

        // Add All-category.
        Category all = Category(-1, "All");
        _categories->append(all);

        for (int i = 0; i < arr.size(); ++i)
        {
            QJsonObject cat = arr.at(i).toObject();
            readCategory(cat);
        }

        endInsertRows();
    }
}

void CategoryModel::readCategory(const QJsonObject &json)
{
    // Check that all the needed data exists.
    if (json.contains("id") && json.value("id").isDouble() && json.contains("name") && json.value("name").isString())
    {
        Category category(json.value("id").toInt(), json.value("name").toString());
        _categories->append(category);
    }
}
