#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QComboBox>

class MyComboBox: public QComboBox
{
    Q_OBJECT
public:
    MyComboBox(QWidget *parent=nullptr);
    void setProgrammaticallyOperation(bool);
    bool isProgrammaticallyOperation();

    private:
    bool programmatically;
};

#endif // MYCOMBOBOX_H
