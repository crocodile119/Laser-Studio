#ifndef FOOTPRINTDIALOG_H
#define FOOTPRINTDIALOG_H

#include "ui_footprintdialog.h"
#include "footprintobject.h"


class FootprintDialog : public QDialog, private Ui::FootprintDialog
{
    Q_OBJECT

public:
    FootprintDialog(FootprintObject *,QWidget *parent = 0);
    ~FootprintDialog();

private slots:
    void on_buttonBox_accepted();

private:

    FootprintObject *myFootprint;
};

#endif
