#ifndef DOCKEFFECTS_H
#define DOCKEFFECTS_H

#include <QDockWidget>

namespace Ui {
class DockEffects;
}

class DockEffects : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockEffects(QWidget *parent = nullptr);
    ~DockEffects();
    Ui::DockEffects *ui;

private:

};

#endif // DOCKEFFECTS_H
