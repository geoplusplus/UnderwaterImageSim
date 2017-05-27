#ifndef DISTDIALOG_H
#define DISTDIALOG_H

#include <QDialog>
#include "transformhistory.h"

namespace Ui {
class DistDialog;
}

class DistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DistDialog(QWidget *parent = 0);
    ~DistDialog();

Q_SIGNALS:

    void distButtonAccepted(bool, TransformHistory::distortion_effect);

    void distButtonApplied(TransformHistory::distortion_effect);

private slots:
    void on_applyButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DistDialog *ui;
};

#endif // DISTDIALOG_H
