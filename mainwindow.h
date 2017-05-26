#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>


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

private slots:

    void on_actionQuit_triggered();

    void on_actionBlur_triggered();

    void on_actionDistortion_Noise_triggered();

    void on_actionArtifacts_triggered();

    void on_actionLighting_triggered();

    void on_actionResolution_triggered();

    void on_actionImport_triggered();

    void on_actionUndo_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    QDir *directory;
    QPixmap preview;
};

#endif // MAINWINDOW_H
