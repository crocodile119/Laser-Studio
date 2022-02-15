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
    LabEditDialog(LabRoom *,bool _dark, QWidget *parent = 0);
    ~LabEditDialog();
    Ui::LabEditDialog *ui;
    void setStyleSheet();

private slots:

private:

    LabRoom *myLabRoom;
    bool dark;

};

#endif
