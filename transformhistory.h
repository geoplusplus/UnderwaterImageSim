#ifndef TRANSFORMHISTORY_H
#define TRANSFORMHISTORY_H

#include <QString>
#include "boost/variant.hpp"
#include <QVector>
#include "opencv2/opencv.hpp"

#define _effect boost::variant<TransformHistory::blur_effect,TransformHistory::distortion_effect,TransformHistory::lighting_effect>

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
        double level;
    } distortion_effect;

    typedef struct lighting_effect{
        double contrast;
        int brightness;
    } lighting_effect;

    void updateHistory(_effect);

    void undo();

    cv::Mat recalculateAll(cv::Mat);

    cv::Mat blur(QString, cv::Mat, int);
    cv::Mat distort(QString,cv::Mat,double);
    cv::Mat light(cv::Mat,double,int);

private:

    QVector<_effect> history;

};

#endif // TRANSFORMHISTORY_H
