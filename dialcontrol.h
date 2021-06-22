#ifndef DIALCONTROL_H
#define DIALCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QUndoStack>
#include "undo_commands/adddialvaluecommand.h"

class DialControl : public QWidget
{
    Q_OBJECT

public:
    DialControl(QWidget *parent = nullptr);
    ~DialControl();
    void setDialNumber(const double&);
    double getDialNumber()const;
    void setTitle(const QString);
    QString getTitle()const;
    void setEnabled(bool);
    bool isEnabled();
    void setBackgroundColor(QString);
    void setUndoStack(QUndoStack*);
    void setPhysicalDimension();
    void setDialInitialValue();

private slots:
    void on_dial_valueChanged(int value);
    void on_dial_actionTriggered(int action);
    void on_dial_sliderPressed();
    void on_dial_sliderReleased();
    void on_dial_sliderMoved(int position);
    void on_undoStack_indexChanged();

signals:
    void valueChanged(int);

private:
    double dialNumber;

    QWidget *dialWidget;
    QGridLayout *gridLayout;
    QDial *dial;
    QLabel *dialNumberLabel;
    QLabel *titleLabel;
    QWidget *scientificNotationWidget;

    QUndoStack *undoStack = nullptr;
    int dialOldValue;
    int dialPressedValue;
    QUndoCommand *dialCommandPressed;
};
#endif // DIALCONTROL_H
