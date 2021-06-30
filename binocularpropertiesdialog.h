#ifndef BINOCULARPROPERTIESDIALOG_H
#define BINOCULARPROPERTIESDIALOG_H

#include "ui_binocularpropertiesdialog.h"
#include "binocular.h"

namespace Ui {
class BinocularPropertiesDialog;
}

class BinocularPropertiesDialog : public QDialog, private Ui::BinocularPropertiesDialog
{
    Q_OBJECT

public:
    BinocularPropertiesDialog(Binocular *binocular, double, QWidget *parent = 0);
    Ui::BinocularPropertiesDialog *ui;
private slots:


private:
    Binocular *binocular;
    double wavelength;
    bool thermicWavelength;
    bool ampOpticWavelength;
};

#endif
