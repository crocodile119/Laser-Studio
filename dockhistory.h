#ifndef DOCKHISTORY_H
#define DOCKHISTORY_H

#include <QDockWidget>

namespace Ui {
class DockHistory;
}

class DockHistory : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockHistory(QWidget *parent = nullptr);
    ~DockHistory();
    Ui::DockHistory *ui;

private:

};

#endif // DOCKHISTORY_H
