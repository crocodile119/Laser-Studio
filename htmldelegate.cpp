#include "htmldelegate.h"

HtmlDelegate::HtmlDelegate(){}

void HtmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;
    initStyleOption(&myOption, index);

    QStyle *style = myOption.widget? myOption.widget->style() : QApplication::style();

    QTextDocument doc;

    QFont arialFont("Arial", 8, QFont::Bold);
    doc.setDefaultFont(arialFont);
    doc.setHtml(myOption.text);

    /// Painting item without text
    myOption.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;

    if (option.state)
        ctx.palette.setColor(QPalette::Text, option.palette.color(QPalette::Active, QPalette::WindowText));

    //Highlighting text if item is selected
    if (option.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, option.palette.color(QPalette::Active, QPalette::HighlightedText));

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &myOption);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();

}

QSize HtmlDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setHtml(optionV4.text);
    doc.setTextWidth(optionV4.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}
