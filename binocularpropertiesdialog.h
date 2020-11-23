#ifndef BINOCULARPROPERTIESDIALOG_H
#define BINOCULARPROPERTIESDIALOG_H

#include "ui_binocularpropertiesdialog.h"
#include "binocular.h"


class BinocularPropertiesDialog : public QDialog, private Ui::BinocularPropertiesDialog
{
    Q_OBJECT

public:
    BinocularPropertiesDialog(Binocular *binocular, double, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();

private:
    Binocular *binocular;
    double wavelength;
    bool thermicWavelength;
    bool ampOpticWavelenght;
};

#endif
