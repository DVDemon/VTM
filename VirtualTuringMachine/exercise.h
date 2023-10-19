#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>
#include <utility>
#include <vector>
#include <iostream>
#include <QDebug>

class Exercise
{
protected:
    int _index;
    bool _completed;
    QString _text_ru;
    QString _text_en;
    QString _alphabit;
    std::vector<std::pair<QString,QString>> _tests;
public:
    Exercise();
    Exercise(const Exercise& other);

    void Clear();

    int GetIndex();
    bool GetCompleted();
    const QString & GetAlphabit();
    const QString & GetTextRu();
    const QString & GetTextEn();
    const std::vector<std::pair<QString,QString>>& GetTests();

    void SetIndex(int index);
    void SetCompleted(bool);
    void SetAlphabit(const QString &alphabit);
    void SetTextRu(const QString& text);
    void SetTextEn(const QString& text);
    void AddTest(const QString& start,const QString& finish);


};

QDebug operator<<(QDebug os, Exercise& ex);

#endif // EXERCISE_H
