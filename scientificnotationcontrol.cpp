#include "scientificnotationcontrol.h"
#include "scientificnumber.h"
#include <cmath>
#include <QDebug>

ScientificNotationControl::ScientificNotationControl(QWidget *parent)
    : QWidget(parent)
{

    scientNotLabelSyle="QLabel {background-color: #fafafa}\n"
                               "QLabel {border: 1px solid grey}\n"
                               "QLabel {border-radius: 8px}\n"
                               "QLabel {color: #000000}";

    scientNotLabelSyleOff="QLabel {background-color: #f0f0f0}\n"
                                  "QLabel {border: 1px solid #a0a0a0}\n"
                                  "QLabel {border-radius: 8px}\n"
                                  "QLabel {color: #a0a0a0}";

    setMaximumSize(QSize(100, 110));
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(tr("gridLayout"));
    dial = new QDial(this);
    dial->setObjectName(tr("dial"));
    dial->setMinimum(1);
    dial->setMaximum(999);

    gridLayout->addWidget(dial, 1, 0, 1, 2);

    scientNotLabel = new QLabel(this);
    scientNotLabel->setObjectName(tr("scientNotLabel"));
    scientNotLabel->setAlignment(Qt::AlignCenter);
    scientNotLabel->setStyleSheet(scientNotLabelSyle);

    gridLayout->addWidget(scientNotLabel, 2, 0, 1, 1);

    verticalScrollBar = new QScrollBar(this);
    verticalScrollBar->setObjectName(tr("verticalScrollBar"));
    verticalScrollBar->setMinimumSize(QSize(0, 25));
    verticalScrollBar->setMinimum(-13);
    verticalScrollBar->setMaximum(11);
    verticalScrollBar->setValue(0);
    verticalScrollBar->setSliderPosition(0);
    verticalScrollBar->setOrientation(Qt::Vertical);

    gridLayout->addWidget(verticalScrollBar, 2, 1, 1, 1);

    titleLabel = new QLabel(this);
    titleLabel->setObjectName(tr("label"));
    titleLabel->setStyleSheet(tr("QLabel {background-color: #00c800}"
                                     "QLabel {color: #fafafa}"
                                     "QLabel {border-radius: 8px}"
                                     "QLabel {border: 0px}"
                                     "QLabel {padding: 3px}"
                                     "QLabel {margin-left: 4px}"
                                     "QLabel {margin-right: 4px}"));

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setText(tr("title"));

    gridLayout->addWidget(titleLabel, 0, 0, 1, 2);

    dial->setValue(10);
    verticalScrollBar->setValue(-1);

    mantissa=0.1;
    exponent=1;
    setScientificNumber();

    QFont font;
    font.setPointSize(8);
    scientNotLabel->setFont(font);
    font.setBold(true);
    titleLabel->setFont(font);

    connect(dial, SIGNAL(valueChanged(int)), this, SLOT(on_dial_valueChanged(int)));
    connect(verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    connect(verticalScrollBar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));
    connect(dial, SIGNAL(actionTriggered(int)), this, SLOT(on_dial_actionTriggered(int)));
    connect(dial, SIGNAL(sliderMoved(int)), this, SLOT(on_dial_sliderMoved(int)));
    connect(dial, SIGNAL(sliderPressed()), this, SLOT(on_dial_sliderPressed()));
    connect(dial, SIGNAL(sliderReleased()), this, SLOT(on_dial_sliderReleased()));
}

ScientificNotationControl::~ScientificNotationControl()
{
}

void ScientificNotationControl::on_dial_valueChanged(int value)
{
    mantissa= value/100.0;
    setScientificNumber();
    emit valueChanged(value);
}

void ScientificNotationControl::on_verticalScrollBar_valueChanged(int value)
{
    exponent=-value;
    setScientificNumber();
    emit valueChanged(value);

    qDebug()<<"on_scrollBar_valueChanged: "<<scrollBarOldValue;
}

void ScientificNotationControl::setScientificNumber()
{
    scientificNumber=mantissa*std::pow(10, exponent);
    scientNotLabel->setText(QString::number(scientificNumber,'e', 2));
}

void ScientificNotationControl::setValue(const double& _value)
{
    double value=_value;
    scientNotLabel->setText(QString::number(value, 'e', 2));
    ScientificNumber scientificNumberCalcolator(value);
    mantissa=scientificNumberCalcolator.getMantissa();
    qDebug()<< "Mantissa: " << mantissa;

    int minimumExponent=getMinimumExponent();
    int maximumExponent=getMaximumExponent();

    if(value>=1)
    {
        exponent=+scientificNumberCalcolator.getExponent();
        if(exponent>maximumExponent)
        {
            mantissa=mantissa*std::pow(10, exponent-maximumExponent);
            exponent=maximumExponent;
        }
    }
    else
    {
        exponent=-(scientificNumberCalcolator.getExponent());
        if(exponent<minimumExponent)
        {
            mantissa=mantissa*std::pow(10, exponent-minimumExponent);
            exponent=minimumExponent;
        }
    }

    dial->setValue(static_cast<int>(mantissa*100));
    verticalScrollBar->setValue(-exponent);

    scientificNumber=mantissa*std::pow(10, exponent);
}

double ScientificNotationControl::getScientificNumber()const
{
    return scientificNumber;
}

/**********************************************************************************
 * La sliderBar verticale incrementa i valori con il tasto rivolto verso il basso *
 * decrementa i valori con il tasto rivolto verso l'alto. Affinchè il controllo   *
 * funzioni come uno spinBox (decrementndo i valori con il tasto rivolto verso    *
 * l'alto e decrementando i valori azionando il tasto rivolto verso il basso) è   *
 * necessario invertire il segno del valore della sliderBar. In questo modo il    *
 * valore minimo diventa il massimo e viceversa il massimo diventa minimo.        *
 **********************************************************************************/

void ScientificNotationControl::setMaximumExponent(int value)
{
    verticalScrollBar->setMinimum(-value);
}

int ScientificNotationControl::getMaximumExponent()const
{
    return -verticalScrollBar->minimum();
}

void ScientificNotationControl::setMinimumExponent(int value)
{
    verticalScrollBar->setMaximum(-value);
}

int ScientificNotationControl::getMinimumExponent()const
{
    return -verticalScrollBar->maximum();
}

void ScientificNotationControl::setTitle(const QString _title)
{
    titleLabel->setText(_title);
}

QString ScientificNotationControl::getTitle()const
{
    return titleLabel->text();
}

int ScientificNotationControl::getExponent()const
{
    return exponent;
}

double ScientificNotationControl::getMantissa()const
{
    return mantissa;
}

void ScientificNotationControl::setExponent(const int _exponent)
{
    verticalScrollBar->setValue(-_exponent);
    exponent=_exponent;
}

void ScientificNotationControl::setMantissa(const double _mantissa)
{
    dial->setValue(static_cast<int>(_mantissa*100));
    mantissa=_mantissa;
}

void ScientificNotationControl::setEnabled(bool _enabled)
{
    dial->setEnabled(_enabled);
    verticalScrollBar->setEnabled(_enabled);

    if(_enabled)
        scientNotLabel->setStyleSheet(scientNotLabelSyle);
    else
        scientNotLabel->setStyleSheet(scientNotLabelSyleOff);
}

void ScientificNotationControl::setBackgroundColor(QString htmlColor)
{
    setStyleSheet("QDial {background-color:"+ htmlColor +";\n}"
                "QScrollBar {background-color:"+ htmlColor +";\n}");
}

void ScientificNotationControl::setDarkColor(bool dark)
{
    if(dark)
    {
        titleLabel->setStyleSheet(tr("QLabel {background-color: #b5922d;"
            "color: #fafafa;"
            "border: 0px solid grey;"
            "border-radius: 8px;"
            "padding: 3px;"
            "margin-right: 10px;}"));
    }
    else
    {
        titleLabel->setStyleSheet(tr("QLabel {background-color: #00c800;"
            "color: #fafafa;"
            "border: 0px solid grey;"
            "border-radius: 8px;"
            "padding: 3px;"
            "margin-right: 10px;}"));
    }
}

void ScientificNotationControl::setStatusTipHelp(const QString & whatThis)
{
    setStatusTip(whatThis);
}

void ScientificNotationControl::on_verticalScrollBar_actionTriggered(int action)
{
    if((action==1)||(action==2))
    {      
        QUndoCommand *scrollCommand = new AddControlValueCommand(dial, verticalScrollBar, dialOldValue, scrollBarOldValue,
                                                                 titleLabel->text(), AddControlValueCommand::movement::TRIGGER_ACTION,
                                                                 AddControlValueCommand::command::SCROLL_BAR);


        undoStack->push(scrollCommand);
        scrollBarOldValue=verticalScrollBar->sliderPosition();
        dialOldValue=dial->sliderPosition();
    }
}

void ScientificNotationControl::on_dial_actionTriggered(int action)
{
    if((action==1)||(action==2)||(action==3)||(action==4))
    {
        QUndoCommand *scrollCommand = new AddControlValueCommand(dial, verticalScrollBar, dialOldValue, scrollBarOldValue,
                                                                 titleLabel->text(), AddControlValueCommand::movement::TRIGGER_ACTION,
                                                                 AddControlValueCommand::command::DIAL);
        undoStack->push(scrollCommand);
        dialOldValue=dial->sliderPosition();
        scrollBarOldValue=verticalScrollBar->sliderPosition();
        qDebug()<<"valore del dial: "<<dial->sliderPosition();
    }
}

void ScientificNotationControl::on_dial_sliderMoved(int position)
{
    Q_UNUSED(position);
    QUndoCommand *scrollCommand = new AddControlValueCommand(dial, verticalScrollBar, dialOldValue, scrollBarOldValue,
                                                             titleLabel->text(), AddControlValueCommand::movement::SLIDER_MOVED,
                                                             AddControlValueCommand::command::DIAL);
    undoStack->push(scrollCommand);
    dialOldValue=dial->sliderPosition();
    scrollBarOldValue=verticalScrollBar->sliderPosition();
}

void ScientificNotationControl::on_dial_sliderPressed()
{ 
    dialCommandPressed= new AddControlValueCommand(dial, verticalScrollBar, dialOldValue, scrollBarOldValue,
                                                   titleLabel->text(), AddControlValueCommand::movement::TRIGGER_ACTION,
                                                   AddControlValueCommand::command::DIAL);
    dialPressedValue=dial->sliderPosition();
    undoStack->push(dialCommandPressed);
    qDebug()<<"valore del dial: "<<dial->sliderPosition();
}

void ScientificNotationControl::on_dial_sliderReleased()
{
    if((dialPressedValue-dial->sliderPosition()==0))
    {
        dialCommandPressed->undo();
        dialCommandPressed->setObsolete(true);
        undoStack->undo();
    }
}

void ScientificNotationControl::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack; 
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(on_undoStack_indexChanged()));
}

void ScientificNotationControl::setDialInitialValue()
{
    dialOldValue=dial->value();
}

void ScientificNotationControl::setScrollBarInitialValue()
{
    scrollBarOldValue=verticalScrollBar->value();
}

void ScientificNotationControl::on_undoStack_indexChanged()
{
    scrollBarOldValue=verticalScrollBar->sliderPosition();
    dialOldValue=dial->sliderPosition();
}
