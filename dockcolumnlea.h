#ifndef DOCKLEA_H
#define DOCKLEA_H

#include <QDockWidget>

namespace Ui {
class DockLea;
}

class DockLea : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockLea(QWidget *parent = nullptr);
    ~DockLea();
    Ui::DockLea *ui;

private:

};

#endif // DOCKLEA_H
