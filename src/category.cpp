#include "category.h"

Category::Category() : _id(-1), _name("All")
{

}

Category::Category(int id, QString name) : _id(id), _name(name)
{

}

Category::~Category()
{

}

int Category::id() const
{
    return _id;
}

QString Category::name() const
{
    return _name;
}
