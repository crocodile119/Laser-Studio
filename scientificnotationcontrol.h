#ifndef SCIENTIFICNOTATIONCONTROL_H
#define SCIENTIFICNOTATIONCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QScrollBar>


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

private slots:
    void on_dial_valueChanged(int value);
    void on_verticalScrollBar_valueChanged(int value);

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
};
#endif // SCIENTIFICNOTATIONWIDGET_H
