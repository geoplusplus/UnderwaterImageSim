#include "savethread.h"
#include "opencv2/opencv.hpp"
#include "mainwindow.h"

saveThread::saveThread(QStringList s, QDir *directory, TransformHistory *transform, QString path) : images(s), directory(directory), transform(transform), path(path) {}

void saveThread::run() {
    for(int i=0;i<images.size();i++) {
        cv::Mat temp = cv::imread(directory->absoluteFilePath(images.at(i)).toStdString());
        cv::Mat edited = transform->recalculateAll(temp);
        QString savePath = path + '/' + images.at(i);
        cv::imwrite(savePath.toStdString(),edited);
        Q_EMIT updateProgress(i+1);
    }
}
