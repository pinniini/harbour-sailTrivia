#ifndef STAT_H
#define STAT_H

#include <QObject>

class Stat : public QObject
{
    Q_OBJECT
public:
    Stat(QObject *parent = nullptr);
    Stat(QString key, double numericValue, QString textValue);
    ~Stat();

    Q_PROPERTY(QString key READ key NOTIFY keyChanged)
    Q_PROPERTY(double numericValue READ numericValue WRITE setNumericValue NOTIFY numericValueChanged)
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)

    QString key() const;

    double numericValue() const;
    void setNumericValue(const double numericValue);

    QString textValue() const;
    void setTextValue(const QString &textValue);

signals:
    void keyChanged();
    void numericValueChanged();
    void textValueChanged();

public slots:

private:
    QString _key;
    double _numericValue;
    QString _textValue;
};

#endif // STAT_H
