#ifndef SLIDERSCROLLLABEL_H
#define SLIDERSCROLLLABEL_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QUndoStack>
#include <QUndoCommand>
#include "undo_commands/addscrollslidervaluecommand.h"

class SliderScrollLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SliderScrollLabel(QWidget *parent = nullptr);
    ~SliderScrollLabel();

    void setScientificNumber();
    double getScientificNumber()const;
    void setMinimumExponent(int value);
    int getMinimumExponent()const;
    void setMaximumExponent(int value);
    int getMaximumExponent()const;
    void setTitle(const QString);
    void setDarkColor(bool dark);
    QString getTitle()const;
    void setValue(const double&);
    double getMantissa()const;
    int getExponent()const;
    void setExponent(const int);
    void setMantissa(const double);
    void setEnabled(bool _enabled);
    void setStatusTipHelp(QString);
    void setUndoStack(QUndoStack*);
    void setPhysicalDimension();
    void setSliderInitialValue();
    void setScrollBarInitialValue();

private slots:
    void on_slider_valueChanged(int value);
    void on_scrollBar_valueChanged(int value);
    void on_slider_actionTriggered(int action);
    void on_slider_sliderPressed();
    void on_slider_sliderReleased();
    void on_slider_sliderMoved(int position);
    void on_scrollBar_actionTriggered(int action);
    void on_undoStack_indexChanged();

signals:
    void valueChanged(int);

private:
    double scientificNumber;
    double mantissa;
    int exponent;

    QGridLayout *gridLayout;
    QLabel *titleLabel;
    QSlider *slider;
    QLabel *scientNotLabel;
    QScrollBar *scrollBar;
    QString scientNotLabelStyle;
    QString scientNotLabelStyleOff;

    QUndoStack *undoStack = nullptr;
    int scrollBarOldValue;
    int sliderOldValue;
    int sliderPressedValue;
    QUndoCommand *sliderCommandPressed;
};

#endif // SLIDERSCROLLLABEL_H
