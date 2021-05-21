#include "dialcontrol.h"
#include <cmath>
#include <QDebug>

DialControl::DialControl(QWidget *parent)
    : QWidget(parent)
{
        setMaximumSize(QSize(100, 110));

        gridLayout = new QGridLayout(this);
        gridLayout->setObjectName(tr("gridLayout"));

        titleLabel = new QLabel(this);
        titleLabel->setObjectName(tr("label"));
        titleLabel->setStyleSheet(tr("QLabel {background-color: #00c800}"
                                   "QLabel {color: #fafafa}"
                                   "QLabel {border-radius: 8px}"
                                   "QLabel {padding: 3px}"
                                   "QLabel {border: 0px}"));

        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setText(tr("title"));

        gridLayout->addWidget(titleLabel, 0, 0, 1, 1);

        dial = new QDial(this);
        dial->setObjectName(tr("dial"));
        dial->setMinimum(1);
        dial->setMaximum(2000);

        gridLayout->addWidget(dial, 1, 0, 1, 1);

        dialNumberLabel = new QLabel(this);
        dialNumberLabel->setObjectName(tr("dialNumberLabel"));
        dialNumberLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(dialNumberLabel, 2, 0, 1, 1);


        setDialNumber(10.0);

        QFont font;
        font.setPointSize(8);
        dialNumberLabel->setFont(font);
        font.setBold(true);
        titleLabel->setFont(font);

        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(on_dial_valueChanged(int)));
}

DialControl::~DialControl()
{

}

void DialControl::on_dial_valueChanged(int value)
{
    dialNumber= value/10.0;
    dialNumberLabel->setText(QString::number(dialNumber, 'f', 1));
    emit valueChanged(value);
}

void DialControl::setDialNumber(const double& _value)
{
    dialNumber=_value;
    dialNumberLabel->setText(QString::number(dialNumber, 'f', 1));
    int myDialNumber=10.0*_value;
    dial->setValue(myDialNumber);
}

double DialControl::getDialNumber()const
{
    return dialNumber;
}

void DialControl::setTitle(const QString _title)
{
    titleLabel->setText(_title);
}

QString DialControl::getTitle()const
{
    return titleLabel->text();
}

void DialControl::setEnabled(bool _enabled)
{
    dial->setEnabled(_enabled);
}

bool DialControl::isEnabled()
{
    return dial->isEnabled();
}

void DialControl::setBackgroundColor(QString htmlColor)
{
    setStyleSheet("QDial {background-color:"+ htmlColor +";\n}");
}
