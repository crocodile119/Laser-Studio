#ifndef REFLECTORPROPERTIESDIALOG_H
#define REFLECTORPROPERTIESDIALOG_H

#include "ui_reflectorpropertiesdialog.h"
#include "reflector.h"


class ReflectorPropertiesDialog : public QDialog, private Ui::ReflectorPropertiesDialog
{
    Q_OBJECT

public:
    ReflectorPropertiesDialog(Reflector *reflector, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();
    void on_positioningSlider_valueChanged(int value);

private:


    Reflector *reflector;
};

#endif
