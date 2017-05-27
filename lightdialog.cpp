#include "lightdialog.h"
#include "ui_lightdialog.h"
#include <QMessageBox>

LightDialog::LightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightDialog)
{
    ui->setupUi(this);
    setWindowTitle("Adjust Lighting");
    ui->contrastLine->setValidator(new QDoubleValidator(1.0,3.0,3,this));
    ui->brightnessLine->setValidator(new QIntValidator(0,100,this));
}

LightDialog::~LightDialog()
{
    delete ui;
}

void LightDialog::on_applyButton_clicked()
{
    TransformHistory::lighting_effect settings;
    bool ok = true;
    settings.contrast = ui->contrastLine->text().toDouble(&ok);
    settings.brightness = ui->brightnessLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Incorrect Range","Please check range of values!");
        return;
    }
    Q_EMIT lightButtonApplied(settings);
}

void LightDialog::on_buttonBox_accepted()
{
    TransformHistory::lighting_effect settings;
    bool ok = true;
    settings.contrast = ui->contrastLine->text().toDouble(&ok);
    settings.brightness = ui->brightnessLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Incorrect Range","Please check range of values!");
        return;
    }
    Q_EMIT lightButtonAccepted(true,settings);
}

void LightDialog::on_buttonBox_rejected()
{
    TransformHistory::lighting_effect settings;
    Q_EMIT lightButtonAccepted(false,settings);
}
