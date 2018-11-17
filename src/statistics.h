#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QMap>
#include <QVariantList>

#include "stat.h"

class Statistics : public QObject
{
    Q_OBJECT
public:
    explicit Statistics(QObject *parent = nullptr);

    /*!
      \brief Returns numeric statistic value with the given key.
      \param statKey Statistic key for which to retrieve a numeric value.
      */
    Q_INVOKABLE double getNumericStat(const QString &statKey);

    /*!
      \brief Returns text statistic value with the given key.
      \param statKey Statistic key for which to retrieve a numeric value.
      */
    Q_INVOKABLE QString getTextStat(const QString &statKey) const;

    /*!
      \brief Returns Stat-object with the given key.
      \param statKey Statistic key.
      */
    Q_INVOKABLE Stat* getStatistic(const QString &statKey);

    /*!
      \brief Get all stored statistics.
      */
    Q_INVOKABLE QVariantList getStatistics();

    /*!
      \brief Set statistic values with the given key.
      \param statKey Statistic key.
      \param numericValue Numeric value of the statistic.
      \param textValue Text value of the statistic.
      */
    Q_INVOKABLE void setStat(const QString &statKey, const double numericValue, const QString &textValue);

signals:

public slots:

private:
    QMap<QString, Stat*> _statistics;
};

#endif // STATISTICS_H
