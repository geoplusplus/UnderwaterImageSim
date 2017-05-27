#ifndef RESOLUTIONDIALOG_H
#define RESOLUTIONDIALOG_H

#include <QDialog>
#include "transformhistory.h"

namespace Ui {
class ResolutionDialog;
}

class ResolutionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResolutionDialog(QWidget *parent = 0);
    ~ResolutionDialog();

Q_SIGNALS:

    void resolutionButtonAccepted(bool, TransformHistory::resolution_effect);

    void resolutionButtonApplied(TransformHistory::resolution_effect);

private slots:
    void on_apply_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ResolutionDialog *ui;
};

#endif // RESOLUTIONDIALOG_H
