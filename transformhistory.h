#ifndef TRANSFORMHISTORY_H
#define TRANSFORMHISTORY_H

#include <QString>
#include "boost/variant.hpp"
#include <QVector>
#include "opencv2/opencv.hpp"

#define _effect boost::variant<TransformHistory::blur_effect,TransformHistory::distortion_effect>

class TransformHistory
{
public:
    TransformHistory();

    typedef struct blur_effect{
        QString type;
        int kernel;
    } blur_effect;

    typedef struct distortion_effect{
        QString type;
        int level;
    } distortion_effect;

    void updateHistory(_effect);

    void undo();

    cv::Mat recalculateAll(cv::Mat);

    cv::Mat blur(QString, cv::Mat, int);

private:

    QVector<_effect> history;

};

#endif // TRANSFORMHISTORY_H
