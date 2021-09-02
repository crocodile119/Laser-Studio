#include <QAbstractItemDelegate>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QApplication>
#include <QDebug>

class HtmlDelegate : public QStyledItemDelegate
{
public:
    HtmlDelegate();
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
private:
};

