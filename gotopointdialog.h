#ifndef GOTOPOINTDIALOG_H
#define GOTOPOINTDIALOG_H

#include "ui_gotopointdialog.h"

class GoToPointDialog : public QDialog, private Ui::GoToPointDialog
{
    Q_OBJECT

public:
    GoToPointDialog(QWidget *parent, QPointF);
    QPointF getViewCenter();
    void setViewCenter(const QPointF);

private slots:
    void on_xSpinBox_valueChanged(int arg1);
    void on_ySpinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    QPointF center;
    double xPosition;
    double yPosition;

};

#endif
