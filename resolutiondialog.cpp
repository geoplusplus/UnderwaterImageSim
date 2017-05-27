#include "resolutiondialog.h"
#include "ui_resolutiondialog.h"
#include <QMessageBox>

ResolutionDialog::ResolutionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResolutionDialog)
{
    ui->setupUi(this);
    setWindowTitle("Set Resolution");
    ui->horizontalLine->setValidator(new QIntValidator(1,10000,this));
    ui->verticalLine->setValidator(new QIntValidator(1,10000,this));
}

ResolutionDialog::~ResolutionDialog()
{
    delete ui;
}

void ResolutionDialog::on_apply_clicked()
{
    TransformHistory::resolution_effect settings;
    bool ok = true;
    settings.horizontal = ui->horizontalLine->text().toInt(&ok);
    settings.vertical = ui->verticalLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Incorrect Range","Keep numbers between 0 and 10000!");
        return;
    }
    Q_EMIT resolutionButtonApplied(settings);
}

void ResolutionDialog::on_buttonBox_accepted()
{
    TransformHistory::resolution_effect settings;
    bool ok = true;
    settings.horizontal = ui->horizontalLine->text().toInt(&ok);
    settings.vertical = ui->verticalLine->text().toInt(&ok);
    if(!ok) {
        QMessageBox message;
        message.critical(0,"Incorrect Range","Keep numbers between 0 and 10000!");
        return;
    }
    Q_EMIT resolutionButtonAccepted(true,settings);
}

void ResolutionDialog::on_buttonBox_rejected()
{
    TransformHistory::resolution_effect settings;
    Q_EMIT resolutionButtonAccepted(false,settings);
}
