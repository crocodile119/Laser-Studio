#ifndef DIALCONTROL_H
#define DIALCONTROL_H

#include <QWidget>
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QScrollBar>


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
    void setBackgroundColor(QString);

private slots:
    void on_dial_valueChanged(int value);

signals:
    void valueChanged(int);

private:
    double dialNumber;

    QString dialLabelSyle;
    QString dialLabelSyleOff;
    QWidget *dialWidget;
    QGridLayout *gridLayout;
    QDial *dial;
    QLabel *dialNumberLabel;
    QLabel *titleLabel;
};
#endif // DIALCONTROL_H
