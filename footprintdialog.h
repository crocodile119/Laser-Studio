#ifndef FOOTPRINTDIALOG_H
#define FOOTPRINTDIALOG_H

#include "ui_footprintdialog.h"
#include "footprintobject.h"

namespace Ui {
class FootprintDialog;
}

class FootprintDialog : public QDialog, private Ui::FootprintDialog
{
    Q_OBJECT

public:
    FootprintDialog(FootprintObject *,QWidget *parent = 0);
    ~FootprintDialog();
    Ui::FootprintDialog *ui;

private slots:

private:

    FootprintObject *myFootprint;
};

#endif
