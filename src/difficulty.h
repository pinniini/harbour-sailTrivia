#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <QObject>

class Difficulty : public QObject
{
    Q_OBJECT
public:
    enum DiffEnum
    {
        All,
        Easy,
        Medium,
        Hard
    };
    Q_ENUM(DiffEnum)
};

#endif // DIFFICULTY_H
