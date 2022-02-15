#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QDialog>

namespace Ui {
class Description;
}

class Description : public QDialog
{
    Q_OBJECT

public:
    explicit Description( bool _dark, QWidget *parent = nullptr);
    ~Description();
    Ui::Description *ui;
    void setStyleSheet();

private:
    bool dark;

};

#endif // DESCRIPTION_H
