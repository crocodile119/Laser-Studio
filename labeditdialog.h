#ifndef LABEDITDIALOG_H
#define LABEDITDIALOG_H

#include "ui_labeditdialog.h"
#include "labroom.h"
#include <QGraphicsRectItem>

namespace Ui {
class LabEditDialog;
}

class LabEditDialog : public QDialog, private Ui::LabEditDialog
{
    Q_OBJECT

public:
    LabEditDialog(LabRoom *,QWidget *parent = 0);
    ~LabEditDialog();
    Ui::LabEditDialog *ui;

private slots:

private:

    LabRoom *myLabRoom;

};

#endif
