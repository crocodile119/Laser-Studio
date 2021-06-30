#include "scrollbarcontrol.h"
#include <cmath>
#include <QDebug>

ScrollBarControl::ScrollBarControl(QWidget *parent)
    : QWidget(parent)
{
    scrollBarNumberLabelSyle="QLabel {background-color: #fafafa}\n"
                                "QLabel {border: 1px solid grey}\n"
                                "QLabel {border-radius: 8px}\n"
                                "QLabel {color: #000000}";

    scrollBarNumberLabelSyleOff="QLabel {background-color: #f0f0f0}\n"
                                "QLabel {border: 1px solid #a0a0a0}\n"
                                "QLabel {border-radius: 8px}\n"
                                "QLabel {color: #a0a0a0}";

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(tr("gridLayout"));

    titleLabel = new QLabel(this);
    titleLabel->setObjectName(tr("label"));
    titleLabel->setStyleSheet(tr("QLabel {background-color: #00c800}"
                                "QLabel {color: #fafafa}"
                                "QLabel {border-radius: 8px}"
                                "QLabel {border: 0px solid grey}"));

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMinimumSize(QSize(0, 20));
    titleLabel->setMaximumSize(QSize(80, 20));
    titleLabel->setText(tr("λ[nm]"));

    gridLayout->addWidget(titleLabel, 0, 0, 1, 1);

    scrollBar = new QScrollBar(Qt::Horizontal, this);
    scrollBar->setObjectName(tr("scrollBar"));
    scrollBar->setMinimumSize(QSize(0, 20));

    gridLayout->addWidget(scrollBar, 0, 1, 1, 1);

    scrollBarNumberLabel = new QLabel(this);
    scrollBarNumberLabel->setObjectName(tr("scrollBarNumberLabel"));
    scrollBarNumberLabel->setAlignment(Qt::AlignCenter);
    scrollBarNumberLabel->setMinimumSize(QSize(50, 30));
    scrollBarNumberLabel->setMaximumSize(QSize(60, 30));
    scrollBarNumberLabel->setStyleSheet(scrollBarNumberLabelSyle);

    gridLayout->addWidget(scrollBarNumberLabel, 0, 2, 1, 1);

    setValue(633);

    QFont font;
    font.setPointSize(8);
    scrollBarNumberLabel->setFont(font);
    font.setBold(true);
    titleLabel->setFont(font);

    connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_scrollBar_valueChanged(int)));
    connect(scrollBar, SIGNAL(actionTriggered(int)), this, SLOT(on_scrollBar_actionTriggered(int)));
    connect(scrollBar, SIGNAL(sliderMoved(int)), this, SLOT(on_scrollBar_sliderMoved(int)));
    connect(scrollBar, SIGNAL(sliderPressed()), this, SLOT(on_scrollBar_sliderPressed()));
    connect(scrollBar, SIGNAL(sliderReleased()), this, SLOT(on_scrollBar_sliderReleased()));
}
ScrollBarControl::~ScrollBarControl()
{

}

void ScrollBarControl::on_scrollBar_valueChanged(int value)
{
    scrollBarNumber= value;
    scrollBarNumberLabel->setText(QString::number(scrollBarNumber));
    emit valueChanged(value);
}

void ScrollBarControl::setValue(const int &_value)
{
    scrollBarNumber=_value;
    scrollBarNumberLabel->setText(QString::number(scrollBarNumber));
    int myScrollBarNumber=_value;
    scrollBar->setValue(myScrollBarNumber);
}

int ScrollBarControl::value()const
{
    return scrollBarNumber;
}

void ScrollBarControl::setTitle(const QString _title)
{
    titleLabel->setText(_title);
}

QString ScrollBarControl::getTitle()const
{
    return titleLabel->text();
}

void ScrollBarControl::setEnabled(bool _enabled)
{
    scrollBar->setEnabled(_enabled);
    if(_enabled)
        scrollBarNumberLabel->setStyleSheet(scrollBarNumberLabelSyle);
    else
        scrollBarNumberLabel->setStyleSheet(scrollBarNumberLabelSyleOff);
}

bool ScrollBarControl::isEnabled()
{
    return scrollBar->isEnabled();
}

void ScrollBarControl::setBackgroundColor(QString htmlColor)
{
    setStyleSheet("QScrollBar {background-color:"+ htmlColor +";\n}");
}

void ScrollBarControl::setMinimum(int value)
{
    scrollBar->setMinimum(value);
}

void ScrollBarControl::setMaximum(int value)
{
    scrollBar->setMaximum(value);
}

void ScrollBarControl::on_scrollBar_actionTriggered(int action)
{
    if((action==1)||(action==2)||(action==3)||(action==4))
    {
        QUndoCommand *scrollCommand = new AddScrollBarValueCommand(scrollBar, scrollBarOldValue, titleLabel->text(),
                                                              AddScrollBarValueCommand::movement::TRIGGER_ACTION);
        undoStack->push(scrollCommand);
        scrollBarOldValue=scrollBar->sliderPosition();
    }
}

void ScrollBarControl::on_scrollBar_sliderMoved(int position)
{
    Q_UNUSED(position);
    QUndoCommand *scrollCommand = new AddScrollBarValueCommand(scrollBar, scrollBarOldValue, titleLabel->text(), AddScrollBarValueCommand::movement::SLIDER_MOVED);
    undoStack->push(scrollCommand);
    scrollBarOldValue=scrollBar->sliderPosition();
}

void ScrollBarControl::on_scrollBar_sliderPressed()
{
    scrollBarCommandPressed= new AddScrollBarValueCommand(scrollBar, scrollBarOldValue, titleLabel->text(), AddScrollBarValueCommand::movement::TRIGGER_ACTION);
    scrollBarPressedValue=scrollBar->sliderPosition();
    undoStack->push(scrollBarCommandPressed);
}

void ScrollBarControl::on_scrollBar_sliderReleased()
{
    if((scrollBarPressedValue-scrollBar->sliderPosition()==0))
    {
        scrollBarCommandPressed->undo();
        scrollBarCommandPressed->setObsolete(true);
        undoStack->undo();
    }
}

void ScrollBarControl::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(on_undoStack_indexChanged()));
}

void ScrollBarControl::setScrollBarInitialValue()
{
    scrollBarOldValue=scrollBar->value();
}

void ScrollBarControl::on_undoStack_indexChanged()
{
    scrollBarOldValue=scrollBar->sliderPosition();
}
