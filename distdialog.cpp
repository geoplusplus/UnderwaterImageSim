#include "distdialog.h"
#include "ui_distdialog.h"
#include "mainwindow.h"
#include <QMessageBox>

DistDialog::DistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistDialog)
{
    ui->setupUi(this);
    setWindowTitle("Set Noise");
    ui->noiseLine->setValidator(new QDoubleValidator(0,200,4,this));
}

DistDialog::~DistDialog()
{
    delete ui;
}

void DistDialog::on_applyButton_clicked()
{
    TransformHistory::distortion_effect settings;
    settings.type = ui->typeBox->currentText();
    bool ok = true;
    settings.level = ui->noiseLine->text().toDouble(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Empty Noise Level","Please enter a noise level!");
        return;
    }
    Q_EMIT distButtonApplied(settings);
}

void DistDialog::on_buttonBox_accepted()
{
    TransformHistory::distortion_effect settings;
    settings.type = ui->typeBox->currentText();
    bool ok = true;
    settings.level = ui->noiseLine->text().toDouble(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Empty Noise Level","Please enter a noise level!");
        return;
    }
    Q_EMIT distButtonAccepted(true,settings);
}

void DistDialog::on_buttonBox_rejected()
{
    TransformHistory::distortion_effect settings;
    Q_EMIT distButtonAccepted(false,settings);
}
