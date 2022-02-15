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
    ReflectorPropertiesDialog(Reflector *reflector, bool _dark, QWidget *parent = 0);
    ~ReflectorPropertiesDialog();
    Ui::ReflectorPropertiesDialog *ui;
    static double MAX_DEFLECTION;
    void setStyleSheet();

private slots:
    void on_positioningSlider_valueChanged(int value);
    void on_xSpinBox_valueChanged(double arg1);
    void on_ySpinBox_valueChanged(double arg1);
    void on_ZRLcheckBox_clicked(bool checked);

private:
    Reflector *reflector;
    bool lambertian;
    double semiLaserAperture;
    bool dark;
};

#endif
