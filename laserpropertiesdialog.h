#ifndef LASERPROPERTIESDIALOG_H
#define LASERPROPERTIESDIALOG_H

#include "ui_laserpropertiesdialog.h"
#include "laserpoint.h"


class LaserPropertiesDialog : public QDialog, private Ui::LaserPropertiesDialog
{
    Q_OBJECT

public:
    LaserPropertiesDialog(LaserPoint *laserpoint, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();
    void on_filterCheckBox_stateChanged(int arg1);

    void on_installationComboBox_currentIndexChanged(int index);

private:
    LaserPoint *laserpoint;
};

#endif
