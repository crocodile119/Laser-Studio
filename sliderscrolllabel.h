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
    QString getTitle()const;
    void setValue(const double&);
    double getMantissa()const;
    int getExponent()const;
    void setExponent(const int);
    void setMantissa(const double);
    void setEnabled(bool _enabled);
    void setStatusTipHelp(QString);

private slots:
    void on_slider_valueChanged(int value);
    void on_scrollBar_valueChanged(int value);

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
};

#endif // SLIDERSCROLLLABEL_H
