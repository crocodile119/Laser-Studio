#ifndef LABEDITDIALOG_H
#define LABEDITDIALOG_H

#include "ui_labeditdialog.h"
#include "labroom.h"
#include <QGraphicsRectItem>


class LabEditDialog : public QDialog, private Ui::LabEditDialog
{
    Q_OBJECT

public:
    LabEditDialog(LabRoom *,QWidget *parent = 0);
    ~LabEditDialog();

private slots:
    void on_buttonBox_accepted();

private:

    LabRoom *myLabRoom;

};

#endif
