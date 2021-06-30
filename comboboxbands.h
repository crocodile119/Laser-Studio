#ifndef COMBOBOXBANDS_H
#define COMBOBOXBANDS_H

#include <QWidget>
#include <QComboBox>
#include <QUndoStack>
#include <QGridLayout>
#include "mycombobox.h"
#include "undo_commands/addcomboboxvaluecommand.h"

class ComboBoxBands : public QWidget
{
    Q_OBJECT

public:
    ComboBoxBands(QWidget *parent = nullptr);
    void setCurrentIndex(const int&);
    int currentIndex()const;
    void setUndoStack(QUndoStack* _undoStack);
    void setComboBoxInitialValue();

public slots:
    void on_comboBoxBands_currentIndexChanged(int index);

signals:
void currentIndexChanged(int index);

private:
    QGridLayout *gridLayout;
    MyComboBox *comboBox;
    int comboBoxOldValue;
    QUndoStack *undoStack = nullptr;

};
#endif // COMBOBOXBANDS_H
