#ifndef SCROLLBARCONTROL_H
#define SCROLLBARCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QLabel>
#include <QUndoStack>
#include "undo_commands/addscrollbarvaluecommand.h"

class ScrollBarControl : public QWidget
{
    Q_OBJECT

public:
    ScrollBarControl(QWidget *parent = nullptr);
    ~ScrollBarControl();
    void setValue(const int&);
    int value()const;
    void setTitle(const QString);
    QString getTitle()const;
    void setEnabled(bool);
    bool isEnabled();
    void setMinimum(int);
    void setMaximum(int);
    void setBackgroundColor(QString);
    void setUndoStack(QUndoStack*);
    void setPhysicalDimension();
    void setScrollBarInitialValue();

private slots:
    void on_scrollBar_valueChanged(int value);
    void on_scrollBar_actionTriggered(int action);
    void on_scrollBar_sliderPressed();
    void on_scrollBar_sliderReleased();
    void on_scrollBar_sliderMoved(int position);
    void on_undoStack_indexChanged();

signals:
    void valueChanged(int);

private:
    int scrollBarNumber;

    QWidget *dialWidget;
    QGridLayout *gridLayout;
    QScrollBar *scrollBar;
    QLabel *scrollBarNumberLabel;
    QLabel *titleLabel;
    QWidget *scientificNotationWidget;
    QUndoStack *undoStack = nullptr;
    int scrollBarOldValue;
    int scrollBarPressedValue;   
    QString scrollBarNumberLabelSyle;
    QString scrollBarNumberLabelSyleOff;

    QUndoCommand *scrollBarCommandPressed;
};
#endif // SCROLLBARCONTROL_H
