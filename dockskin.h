#ifndef DOCKSKIN_H
#define DOCKSKIN_H

#include <QDockWidget>

namespace Ui {
class DockSkin;
}

class DockSkin : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockSkin(QWidget *parent = nullptr);
    ~DockSkin();
    Ui::DockSkin *ui;
private:

};

#endif // DOCKSKIN_H
