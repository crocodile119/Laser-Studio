#ifndef SCROLLBARCONTROL_H
#define SCROLLBARCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QUndoStack>
#include "undo_commands/addscrollbarvaluecommand.h"

class ComboTypeControl : public QWidget
{
    Q_OBJECT

public:
    ComboTypeControl(QWidget *parent = nullptr);
    ~ComboTypeControl();

private slots:
void on_comboBox_currentIndexChanged(int index);

signals:
    void valueChanged(int);

private:
    int comboBoxIndex;
    QGridLayout *gridLayout;
    QComboBox *comboBox;

    QUndoStack *undoStack = nullptr;
    int comboOldIndex;

    QUndoCommand *comboTypeCommand;
};
#endif // SCROLLBARCONTROL_H
