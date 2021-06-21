#ifndef SCIENTIFICNOTATIONCONTROL_H
#define SCIENTIFICNOTATIONCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QScrollBar>
#include <QUndoStack>
#include <QUndoView>
#include "addcontrolvaluecommand.h"
#include "addcontrolvaluecommand.h"

class ScientificNotationControl : public QWidget
{
    Q_OBJECT

public:
    ScientificNotationControl(QWidget *parent = nullptr);
    ~ScientificNotationControl();
    void setScientificNumber();
    double getScientificNumber()const;
    void setMinimumExponent(int value);
    int getMinimumExponent()const;
    void setMaximumExponent(int value);
    int getMaximumExponent()const;
    void setTitle(const QString);
    QString getTitle()const;
    void setValue(const double&);
    double getMantissa()const;
    int getExponent()const;
    void setExponent(const int);
    void setMantissa(const double);
    void setEnabled(bool _enabled);
    void setBackgroundColor(QString);
    void setStatusTipHelp(QString);
    void setUndoStack(QUndoStack*);
    void setPhysicalDimension();
    void setDialInitialValue();
    void setScrollBarInitialValue();

private slots:
    void on_dial_valueChanged(int value);
    void on_dial_actionTriggered(int action);
    void on_dial_sliderPressed();
    void on_dial_sliderReleased();
    void on_dial_sliderMoved(int position);
    void on_verticalScrollBar_valueChanged(int value);
    void on_verticalScrollBar_actionTriggered(int action);
    void on_undoStack_indexChanged();

    //void on_verticalScrollBar_sliderPressed();
    //void on_verticalScrollBar_sliderReleased();

signals:
    void valueChanged(int);

private:
    double scientificNumber;
    double mantissa;
    int exponent;

    QWidget *scientificNotationWidget;
    QGridLayout *gridLayout;
    QDial *dial;
    QLabel *scientNotLabel;
    QLabel *titleLabel;
    QScrollBar *verticalScrollBar;
    QString scientNotLabelSyle;
    QString scientNotLabelSyleOff;

    QUndoStack *undoStack = nullptr;
    QUndoView *undoView = nullptr;
    int scrollBarOldValue;
    int scrollBarPressedValue;
    int dialOldValue;
    int dialPressedValue;
    QUndoCommand *scrollBarCommandPressed;
    QUndoCommand *dialCommandPressed;

};
#endif // SCIENTIFICNOTATIONWIDGET_H
