#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include "blurdialog.h"
#include "distdialog.h"
#include "lightdialog.h"
#include "resolutiondialog.h"
#include "transformhistory.h"
#include "savethread.h"
#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void resizeEvent(QResizeEvent *);

public Q_SLOTS:

    void blurAccepted(bool, TransformHistory::blur_effect);

    void blurApplied(TransformHistory::blur_effect);

    void distAccepted(bool, TransformHistory::distortion_effect);

    void distApplied(TransformHistory::distortion_effect);

    void lightAccepted(bool,TransformHistory::lighting_effect);

    void lightApplied(TransformHistory::lighting_effect);

    void resolutionAccepted(bool,TransformHistory::resolution_effect);

    void resolutionApplied(TransformHistory::resolution_effect);

private slots:

    void on_actionQuit_triggered();

    void on_actionBlur_triggered();

    void on_actionDistortion_Noise_triggered();

    void on_actionLighting_triggered();

    void on_actionResolution_triggered();

    void on_actionImport_triggered();

    void on_actionUndo_triggered();

    void on_actionSave_triggered();

    void on_actionHelp_triggered();

    void on_actionRedo_triggered();

private:
    Ui::MainWindow *ui;
    QDir *directory;
    QPixmap preview;
    QPixmap original;
    QPixmap raw;
    BlurDialog *bDialog;
    DistDialog *dDialog;
    LightDialog *lDialog;
    ResolutionDialog *rDialog;
    TransformHistory *transform;
    saveThread *save;

    bool dialogOpen;
    bool checkDialogOpenError();
    void repaintPreview();

    inline cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true )
       {
          switch ( inImage.format() )
          {
             // 8-bit, 4 channel
             case QImage::Format_ARGB32:
             case QImage::Format_ARGB32_Premultiplied:
             {
                cv::Mat  mat( inImage.height(), inImage.width(),
                              CV_8UC4,
                              const_cast<uchar*>(inImage.bits()),
                              static_cast<size_t>(inImage.bytesPerLine())
                              );

                return (inCloneImageData ? mat.clone() : mat);
             }

             // 8-bit, 3 channel
             case QImage::Format_RGB32:
             case QImage::Format_RGB888:
             {
                if ( !inCloneImageData )
                {
                   qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
                }

                QImage   swapped = inImage;

                if ( inImage.format() == QImage::Format_RGB32 )
                {
                   swapped = swapped.convertToFormat( QImage::Format_RGB888 );
                }

                swapped = swapped.rgbSwapped();

                return cv::Mat( swapped.height(), swapped.width(),
                                CV_8UC3,
                                const_cast<uchar*>(swapped.bits()),
                                static_cast<size_t>(swapped.bytesPerLine())
                                ).clone();
             }

             // 8-bit, 1 channel
             case QImage::Format_Indexed8:
             {
                cv::Mat  mat( inImage.height(), inImage.width(),
                              CV_8UC1,
                              const_cast<uchar*>(inImage.bits()),
                              static_cast<size_t>(inImage.bytesPerLine())
                              );

                return (inCloneImageData ? mat.clone() : mat);
             }

             default:
                qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
                break;
          }

          return cv::Mat();
       }

       // If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
       // with the cv::Mat directly
       //    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
       inline cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true )
       {
          return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
       }

       inline QImage  cvMatToQImage( const cv::Mat &inMat )
       {
          switch ( inMat.type() )
          {
             // 8-bit, 4 channel
             case CV_8UC4:
             {
                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_ARGB32 );

                return image;
             }

             // 8-bit, 3 channel
             case CV_8UC3:
             {
                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

                return image.rgbSwapped();
             }

             // 8-bit, 1 channel
             case CV_8UC1:
             {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_Grayscale8 );
    #else
                static QVector<QRgb>  sColorTable;

                // only create our color table the first time
                if ( sColorTable.isEmpty() )
                {
                   sColorTable.resize( 256 );

                   for ( int i = 0; i < 256; ++i )
                   {
                      sColorTable[i] = qRgb( i, i, i );
                   }
                }

                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_Indexed8 );

                image.setColorTable( sColorTable );
    #endif

                return image;
             }

             default:
                qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
                break;
          }

          return QImage();
       }

       inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
       {
          return QPixmap::fromImage( cvMatToQImage( inMat ) );
       }

};

#endif // MAINWINDOW_H
