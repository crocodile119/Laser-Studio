#ifndef GOTOPOINTDIALOG_H
#define GOTOPOINTDIALOG_H

#include "ui_gotopointdialog.h"

class GoToPointDialog : public QDialog, private Ui::GoToPointDialog
{
    Q_OBJECT

public:
    GoToPointDialog(QWidget *parent, bool _dark);
    QPointF getViewCenter();
    void setViewCenter();
    void setStyleSheet();

private slots:
    void on_xSpinBox_valueChanged(int arg1);
    void on_ySpinBox_valueChanged(int arg1);

private:
    QPointF center;
    bool dark;
    double xPosition;
    double yPosition;
};

#endif
