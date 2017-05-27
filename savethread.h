#ifndef SAVETHREAD_H
#define SAVETHREAD_H
#include <QThread>
#include <QDir>
#include "transformhistory.h"

class saveThread : public QThread
{
    Q_OBJECT

public:
    explicit saveThread(QStringList, QDir*, TransformHistory*, QString);
    void run();

Q_SIGNALS:
    void updateProgress(int);

private:
    QStringList images;
    QDir *directory;
    TransformHistory *transform;
    QString path;
};

#endif // SAVETHREAD_H
