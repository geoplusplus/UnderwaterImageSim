#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>

//This is built on Ubuntu 14.04 with OpenCV 3.2.0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Underwater Image Simulator");
    ui->progressBar->setValue(0);
    dialogOpen = false;
    bDialog = new BlurDialog();
    dDialog = new DistDialog();
    transform = new TransformHistory();
    //    cv::Mat inputImage = cv::imread("/home/peter/Pictures/fox.jpg");
    //    if(!inputImage.empty()) cv::imshow("Display Image", inputImage);
    QObject::connect(bDialog,SIGNAL(blurButtonAccepted(bool,TransformHistory::blur_effect)),this,SLOT(blurAccepted(bool,TransformHistory::blur_effect)));
    QObject::connect(bDialog,SIGNAL(blurButtonApplied(TransformHistory::blur_effect)),this,SLOT(blurApplied(TransformHistory::blur_effect)));
    QObject::connect(dDialog,SIGNAL(distButtonAccepted(bool,TransformHistory::distortion_effect)),this,SLOT(distAccepted(bool,TransformHistory::distortion_effect)));
    QObject::connect(dDialog,SIGNAL(distButtonApplied(TransformHistory::distortion_effect)),this,SLOT(distApplied(TransformHistory::distortion_effect)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::repaintPreview() {
    ui->imageLabel->setPixmap(preview.scaled(this->size(),Qt::KeepAspectRatio));
}

void MainWindow::on_actionImport_triggered()
{
    ui->progressBar->setValue(0);
    //Open file query window to get the directory of image files
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if ( path.isNull() == true )
    {
        return;
    }

    //Find first image

    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.bmp";
    directory = new QDir(path);
    QStringList images = directory->entryList(filters,QDir::Files,QDir::Name);
    raw = QPixmap(path + '/' + images.at(0));
    preview = raw;
    repaintPreview();
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    (void)e; //Suppress warning
    if(!preview.isNull())
        repaintPreview();
}

void MainWindow::on_actionQuit_triggered()
{
    //Ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Quit", "Are you sure you want to quit?",
                                  QMessageBox::Yes|QMessageBox::No);

    //Close the application
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

bool MainWindow::checkDialogOpenError() {
    if(preview.isNull()) {
        QMessageBox message;
        message.critical(0,"Error","Please load an image set!");
        return true;
    }
    if(dialogOpen) {
        QMessageBox message;
        message.critical(0,"Effect Open","Please close additional effect windows!");
        return true;
    }
    return false;
}

void MainWindow::on_actionBlur_triggered()
{
    ui->progressBar->setValue(0);
    if(checkDialogOpenError()) {
        return;
    }
    original = preview;
    //Open a window where user selects type of blur and level of blur
    bDialog->setModal(true);
    dialogOpen = true;
    bDialog->exec();
    dialogOpen = false;
}

void MainWindow::blurApplied(TransformHistory::blur_effect settings) {
    //Apply button puts the change on the image in the mainwindow
    //(make sure apply doesn't stack, i.e. it only puts transform on the very first image)
    cv::Mat cvImage = QPixmapToCvMat(original);
    cv::Mat blurredImage = transform->blur(settings.type,cvImage,settings.kernel);

    preview = cvMatToQPixmap(blurredImage);
    repaintPreview();
}

void MainWindow::blurAccepted(bool result,TransformHistory::blur_effect settings) {
    if(!result) {
        preview = original;
    }
    else {
        blurApplied(settings);
        transform->updateHistory(settings);
    }
    repaintPreview();
}

void MainWindow::on_actionDistortion_Noise_triggered()
{
    ui->progressBar->setValue(0);
    if(checkDialogOpenError()) {
        return;
    }
    original = preview;
    //Open a window where user selects type of blur and level of blur
    dDialog->setModal(true);
    dialogOpen = true;
    dDialog->exec();
    dialogOpen = false;
}

void MainWindow::distApplied(TransformHistory::distortion_effect settings) {
    cv::Mat cvImage = QPixmapToCvMat(original);
    cv::Mat distImage = transform->distort(settings.type,cvImage,settings.level);

    preview = cvMatToQPixmap(distImage);
    repaintPreview();
}

void MainWindow::distAccepted(bool result, TransformHistory::distortion_effect settings) {
    if(!result) {
        preview = original;
    }
    else {
        distApplied(settings);
        transform->updateHistory(settings);
    }
    repaintPreview();
}

void MainWindow::on_actionArtifacts_triggered()
{
    ui->progressBar->setValue(0);
    //Open a window where user selects type of artifacts to add (probably will have a template that scales to image size?)

    //Apply button puts the change on the image in the mainwindow
    //(make sure apply doesn't stack, i.e. it only puts transform on the very first image)

    //Accept button appends it to history of transforms

    //Cancel returns with no changes done
}

void MainWindow::on_actionLighting_triggered()
{
    ui->progressBar->setValue(0);
    //Open a window where user selects level of contrast and brightness

    //Apply button puts the change on the image in the mainwindow
    //(make sure apply doesn't stack, i.e. it only puts transform on the very first image)

    //Accept button appends it to history of transforms

    //Cancel returns with no changes done
}

void MainWindow::on_actionResolution_triggered()
{
    ui->progressBar->setValue(0);
    //Open a window where user selects new resolution

    //Apply button puts the change on the image in the mainwindow (stretch the image to fit the label)
    //(make sure apply doesn't stack, i.e. it only puts transform on the very first image)

    //Accept button appends it to history of transforms

    //Cancel returns with no changes done
}

void MainWindow::on_actionUndo_triggered()
{
    ui->progressBar->setValue(0);
    //Removes latest transformation from list and recalculates preview image
    if(raw.isNull()) {
        return;
    }
    transform->undo();
    cv::Mat recalculatedImage = transform->recalculateAll(QPixmapToCvMat(raw));
    preview = cvMatToQPixmap(recalculatedImage);
    repaintPreview();
}

void MainWindow::on_actionSave_triggered()
{
    ui->progressBar->setValue(0);
    //Opens up file browser and asks for directory to save in
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    //Applies transform to each image in the same directory as the preview image and saves with the original name in the new directory
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.bmp";
    QStringList images = directory->entryList(filters,QDir::Files,QDir::Name);
    ui->progressBar->setMaximum(images.size());
    save = new saveThread(images,directory,transform,path);
    QObject::connect(save,SIGNAL(updateProgress(int)),ui->progressBar,SLOT(setValue(int)));
    save->start();
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox message;
    message.information(0,"Help","We all need help sometimes.");
}

