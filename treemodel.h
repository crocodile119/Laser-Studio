#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    const static int COLUMNS;
    enum class GraphicsItem{ENVIRONMENT, LASERPOINT, REFLECTOR, BINOCULAR, BEAM_INSPECTOR};
    enum class ReflectorKind{NO_REFLECTOR, GLASS, MIRROR, WET, LAMBERTIAN, INDENT, LAB};
    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    void insertGraphicElement(int position, QVariant data);

    void insertDataHeader(GraphicsItem graphicItem, const QString& dataString);
    void setUpTree();
    void myDataHasChanged();

private:
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;
};

#endif // TREEMODEL_H
