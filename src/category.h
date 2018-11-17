#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>

class Category
{
public:
    Category();
    Category(int id, QString name);
    ~Category();

    /*!
     * \brief id
     * \return
     */
    int id() const;

    /*!
     * \brief name
     * \return
     */
    QString name() const;

signals:

public slots:

private:
    int _id;
    QString _name;
};

#endif // CATEGORY_H
