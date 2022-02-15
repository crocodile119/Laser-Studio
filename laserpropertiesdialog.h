#ifndef LASERPROPERTIESDIALOG_H
#define LASERPROPERTIESDIALOG_H

#include "ui_laserpropertiesdialog.h"
#include "laserpoint.h"

namespace Ui {
class LaserPropertiesDialog;
}

class LaserPropertiesDialog : public QDialog, private Ui::LaserPropertiesDialog
{
    Q_OBJECT

public:
    LaserPropertiesDialog(LaserPoint *_laserpoint, bool _dark, QWidget *parent = 0);
    ~LaserPropertiesDialog();
    Ui::LaserPropertiesDialog *ui;
    void setStyleSheet();

private slots:
    void on_filterCheckBox_stateChanged(int arg1);
    double computePillowAreas(const int &);
    void on_installationComboBox_currentIndexChanged(int index);

private:
    LaserPoint *laserpoint;
    bool dark;
};

#endif
