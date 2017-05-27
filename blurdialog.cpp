#include "blurdialog.h"
#include "ui_blurdialog.h"
#include <QMessageBox>

BlurDialog::BlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlurDialog)
{
    ui->setupUi(this);
    setWindowTitle("Set Blur");
    ui->kernelLine->setValidator( new QIntValidator(0, 100, this) );
}

BlurDialog::~BlurDialog()
{
    delete ui;
}

void BlurDialog::on_buttonBox_accepted()
{
    TransformHistory::blur_effect settings;
    settings.type = ui->typeBox->currentText();
    bool ok = true;
    settings.kernel = ui->kernelLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Empty Kernel Size","Please enter a kernel size!");
        return;
    }
    Q_EMIT blurButtonAccepted(true,settings);
}

void BlurDialog::on_buttonBox_rejected()
{
    TransformHistory::blur_effect settings;
    Q_EMIT blurButtonAccepted(false,settings);
}

void BlurDialog::on_apply_clicked()
{
    TransformHistory::blur_effect settings;
    settings.type = ui->typeBox->currentText();
    bool ok = true;
    settings.kernel = ui->kernelLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Empty Kernel Size","Please enter a kernel size!");
        return;
    }
    Q_EMIT blurButtonApplied(settings);
}
