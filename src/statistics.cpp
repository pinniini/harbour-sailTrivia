#include <QDebug>
#include <QVariant>
#include <QtQml>

#include "statistics.h"

Statistics::Statistics(QObject *parent) : QObject(parent)
{

}

double Statistics::getNumericStat(const QString &statKey)
{
    // Check for empty key.
    if (statKey.isEmpty())
    {
        return 0;
    }
    else // Return value or empty string.
    {
        // Get statistic.
        Stat *stat =  _statistics.value(statKey, 0);
        if (stat != 0)
        {
            return stat->numericValue();
        }
        else
        {
            return 0;
        }
    }
}

QString Statistics::getTextStat(const QString &statKey) const
{
    // Check for empty key.
    if (statKey.isEmpty())
    {
        return "";
    }
    else // Return value or empty string.
    {
        // Get statistic.
        Stat *stat =  _statistics.value(statKey, 0);
        if (stat != 0)
        {
            return stat->textValue();
        }
        else
        {
            return "";
        }
    }
}

Stat* Statistics::getStatistic(const QString &statKey)
{
    // Check for empty key.
    if (statKey.isEmpty())
    {
        return 0;
    }
    else // Return proper value or zero.
    {
        Stat *stat = _statistics.value(statKey, 0);
        return stat;
    }
}

QVariantList Statistics::getStatistics()
{
    QVariantList list;
    for (Stat* stat : _statistics.values())
    {
        list.append(QVariant::fromValue(stat));
    }
    return list;
}

void Statistics::setStat(const QString &statKey, const double numericValue, const QString &textValue)
{
    qDebug() << "Setting stat: " << statKey << " " << numericValue << " " << textValue;
    Stat *stat = new Stat(statKey, numericValue, textValue);

    // This is necessary, so that the js engine won't garbage collect the object after using get-method.
    // Because the ownership moves to the js engine if we return the object from here to there.
    QQmlEngine::setObjectOwnership(stat, QQmlEngine::CppOwnership);

    _statistics.insert(statKey, stat);
}
