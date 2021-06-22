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
        connect(dial, SIGNAL(actionTriggered(int)), this, SLOT(on_dial_actionTriggered(int)));
        connect(dial, SIGNAL(sliderMoved(int)), this, SLOT(on_dial_sliderMoved(int)));
        connect(dial, SIGNAL(sliderPressed()), this, SLOT(on_dial_sliderPressed()));
        connect(dial, SIGNAL(sliderReleased()), this, SLOT(on_dial_sliderReleased()));
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

void DialControl::on_dial_actionTriggered(int action)
{
    if((action==1)||(action==2)||(action==3)||(action==4))
    {
        QUndoCommand *scrollCommand = new AddDialValueCommand(dial, dialOldValue, "Te[s]",
                                                              AddDialValueCommand::movement::TRIGGER_ACTION);
        undoStack->push(scrollCommand);
        dialOldValue=dial->sliderPosition();
        qDebug()<<"valore del dial: "<<dial->sliderPosition();
    }
}

void DialControl::on_dial_sliderMoved(int position)
{
    Q_UNUSED(position);
    QUndoCommand *scrollCommand = new AddDialValueCommand(dial, dialOldValue, "Te[s]",
                                                               AddDialValueCommand::movement::SLIDER_MOVED);
    undoStack->push(scrollCommand);
    dialOldValue=dial->sliderPosition();
}

void DialControl::on_dial_sliderPressed()
{
    dialCommandPressed= new AddDialValueCommand(dial, dialOldValue, "Te[s]",
                                                               AddDialValueCommand::movement::TRIGGER_ACTION);
    dialPressedValue=dial->sliderPosition();
    undoStack->push(dialCommandPressed);
    qDebug()<<"valore del dial: "<<dial->sliderPosition();
}

void DialControl::on_dial_sliderReleased()
{
    if((dialPressedValue-dial->sliderPosition()==0))
    {
        dialCommandPressed->undo();
        dialCommandPressed->setObsolete(true);
        undoStack->undo();
    }
}

void DialControl::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(on_undoStack_indexChanged()));
}

void DialControl::setDialInitialValue()
{
    dialOldValue=dial->value();
}

void DialControl::on_undoStack_indexChanged()
{
    dialOldValue=dial->sliderPosition();
}
