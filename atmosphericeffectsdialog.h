#ifndef ATMOSPHERICEFFECTSDIALOG_H
#define ATMOSPHERICEFFECTSDIALOG_H

#include "ui_atmosphericeffectsdialog.h"
#include <QGraphicsRectItem>
#include "centralwidget.h"

class AtmosphericEffectsDialog : public QDialog, private Ui::AtmosphericEffectsDialog
{
    Q_OBJECT

public:
    AtmosphericEffectsDialog(QWidget *parent, CentralWidget *_laserWindow, double);
    ~AtmosphericEffectsDialog();

private slots:
    void on_buttonBox_accepted();
    void on_meteoRangeSlider_valueChanged(int value);

private:
    double wavelength;
    int V;
    double A;
    double atmAttCoeff;
    CentralWidget *laserWindow;
};

#endif
