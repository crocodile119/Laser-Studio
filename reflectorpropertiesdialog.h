#ifndef REFLECTORPROPERTIESDIALOG_H
#define REFLECTORPROPERTIESDIALOG_H

#include "ui_reflectorpropertiesdialog.h"
#include "reflector.h"

namespace Ui {
class ReflectorPropertiesDialog;
}

class ReflectorPropertiesDialog : public QDialog, private Ui::ReflectorPropertiesDialog
{
    Q_OBJECT

public:
    ReflectorPropertiesDialog(Reflector *reflector, QWidget *parent = 0);
    ~ReflectorPropertiesDialog();
    Ui::ReflectorPropertiesDialog *ui;

private slots:
    void on_positioningSlider_valueChanged(int value);

private:
    Reflector *reflector;
};

#endif
