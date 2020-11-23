#ifndef DOCKRESULTS_H
#define DOCKRESULTS_H

#include <QDockWidget>

namespace Ui {
class DockResults;
}

class DockResults : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockResults(QWidget *parent = nullptr);
    ~DockResults();
    Ui::DockResults *ui;
private:

};

#endif // DOCKRESULTS_H
