#ifndef LIGHTDIALOG_H
#define LIGHTDIALOG_H

#include <QDialog>
#include "transformhistory.h"

namespace Ui {
class LightDialog;
}

class LightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LightDialog(QWidget *parent = 0);
    ~LightDialog();

Q_SIGNALS:

    void lightButtonAccepted(bool, TransformHistory::lighting_effect);

    void lightButtonApplied(TransformHistory::lighting_effect);

private slots:
    void on_applyButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LightDialog *ui;
};

#endif // LIGHTDIALOG_H
