#ifndef DOCKGOGGLE_H
#define DOCKGOGGLE_H

#include <QDockWidget>

namespace Ui {
class DockGoggle;
}

class DockGoggle : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockGoggle(QWidget *parent = nullptr);
    ~DockGoggle();
    Ui::DockGoggle *ui;

private:

};

#endif // DOCKGOGGLE_H
