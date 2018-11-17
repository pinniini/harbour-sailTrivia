#include "stat.h"

Stat::Stat(QObject *parent) : QObject(parent)
{

}

Stat::Stat(QString key, double numericValue, QString textValue)
    : _key(key), _numericValue(numericValue), _textValue(textValue)
{

}

Stat::~Stat()
{

}

QString Stat::key() const
{
    return _key;
}

double Stat::numericValue() const
{
    return _numericValue;
}

void Stat::setNumericValue(const double numericValue)
{
    if (numericValue != _numericValue)
    {
        _numericValue = numericValue;
        emit numericValueChanged();
    }
}

QString Stat::textValue() const
{
    return _textValue;
}

void Stat::setTextValue(const QString &textValue)
{
    if (textValue != _textValue)
    {
        _textValue = textValue;
        emit textValueChanged();
    }
}
