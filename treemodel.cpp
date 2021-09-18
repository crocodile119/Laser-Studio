#include "treemodel.h"
#include "treeitem.h"

#include <QtWidgets>

const int TreeModel::COLUMNS=1;
TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootItem = new TreeItem({tr("Titolo")});
    setUpTree();
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        TreeItem *item = getItem(index);
        QList<QVariant> myData= item->data(index.column()).toList();
        return myData.at(0);
    }

    if (role == Qt::DecorationRole)
    {
        QIcon icon;
        TreeItem *item = getItem(index);
        QList<QVariant> myData= item->data(index.column()).toList();

        if((myData.at(1)==0)&&(myData.at(2)==5))
        {
            icon=QIcon(":/images/polygon.png");
        }
        else if((myData.at(1)==0)&&(myData.at(2)==6))
        {
            icon=QIcon(":/images/room.png");
        }
        else if((myData.at(1)==1)&&(myData.at(2)==5))
        {
            icon=QIcon(":/images/laserpix.png");
        }
        else if((myData.at(1)==2)&&(myData.at(2)==1))
        {
            icon=QIcon(":/images/window.png");
        }
        else if((myData.at(1)==2)&&(myData.at(2)==2))
        {
            icon=QIcon(":/images/mirror1.png");
        }
        else if((myData.at(1)==2)&&(myData.at(2)==3))
        {
            icon=QIcon(":/images/wet.png");
        }
        else if((myData.at(1)==2)&&(myData.at(2)==4))
        {
            icon=QIcon(":/images/lambertian.png");
        }
        else if((myData.at(1)==3)&&(myData.at(2)==5))
        {
            icon=QIcon(":/images/binocular.png");
        }
        else if((myData.at(1)==4)&&(myData.at(2)==5))
        {
            icon=QIcon(":/images/inspector.png");
        }
        else
            icon=QIcon();

        return icon;
    }
    return QVariant();

}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

void TreeModel::insertGraphicElement(int position, QVariant data)
{
    QList<QVariant> myList=data.toList();
    int parentIndex=myList.at(1).toInt();
    QModelIndex graphicModelIndex=index(parentIndex, COLUMNS-1);
    const int ROWS=1;
    insertRows(position, ROWS, graphicModelIndex);
    QModelIndex myIndex=index(position, COLUMNS-1, graphicModelIndex);
    setData(myIndex, QVariant(data), Qt::EditRole);
}

void TreeModel::insertDataHeader(GraphicsItem graphicItem, const QString& dataString)
{
    TreeItem* treeItemHeader=rootItem;
    int position=static_cast<int>(graphicItem);
    switch(graphicItem)
    {
    case(GraphicsItem::ENVIRONMENT):
    {
        QList<QVariant> environmentList={dataString, (0), (0)};
        QVariant environmentData=QVariant(environmentList);
        treeItemHeader->setData(COLUMNS-1, environmentData);
        treeItemHeader->child(position)->setData(COLUMNS-1, environmentData);
    }
    break;
    case(GraphicsItem::LASERPOINT):
    {
        QList<QVariant> laserpointList={dataString, (1), (0)};
        QVariant laserpointData=QVariant(laserpointList);
        treeItemHeader->child(position)->setData(COLUMNS-1, laserpointData);
    }
    break;
    case(GraphicsItem::REFLECTOR):
    {
        QList<QVariant> reflectorList={dataString, (2), (0)};
        QVariant reflectorData=QVariant(reflectorList);
        treeItemHeader->child(position)->setData(COLUMNS-1, reflectorData);
    }
    break;
    case(GraphicsItem::BINOCULAR):
    {
        QList<QVariant> binocularList={dataString, (3), 0};
        QVariant binocularData=QVariant(binocularList);
        treeItemHeader->child(position)->setData(COLUMNS-1, binocularData);
    }
    break;
    case(GraphicsItem::BEAM_INSPECTOR):
    {
        QList<QVariant> beamInspectorList={dataString, (4), (0)};
        QVariant beamInspectorData=QVariant(beamInspectorList);
        treeItemHeader->child(position)->setData(COLUMNS-1, beamInspectorData);
    }
    break;
    default:
    setUpTree();
    break;
    }
}

void TreeModel::setUpTree()
{
    QList<QVariant> environmentList={"Ambiente", (0), (0)};
    QVariant environmentData=QVariant(environmentList);
    TreeItem* environment=rootItem;
    environment->insertChildren(environment->childCount(), 1, COLUMNS);
    environment->child(environment->childCount()-1)->setData(COLUMNS-1, environmentData);

    QList<QVariant> laserpointList={"Punto laser", (1), (0)};
    QVariant laserpointData=QVariant(laserpointList);
    TreeItem*laserpoint=rootItem;
    laserpoint->insertChildren(laserpoint->childCount(), 1, COLUMNS);
    laserpoint->child(laserpoint->childCount()-1)->setData(COLUMNS-1, laserpointData);

    QList<QVariant> reflectorList={"Riflettori", (2), (0)};
    QVariant reflectorData=QVariant(reflectorList);
    TreeItem*reflector=rootItem;
    reflector->insertChildren(reflector->childCount(), 1, COLUMNS);
    reflector->child(reflector->childCount()-1)->setData(COLUMNS-1, reflectorData);

    QList<QVariant> binocularList={"Strumenti ottici", (3), 0};
    QVariant binocularData=QVariant(binocularList);
    TreeItem*binocular=rootItem;
    binocular->insertChildren(binocular->childCount(), 1, COLUMNS);
    binocular->child(binocular->childCount()-1)->setData(COLUMNS-1, binocularData);

    QList<QVariant> beamInspectorList={"Segnaposti", (4), (0)};
    QVariant beamInspectorData=QVariant(beamInspectorList);
    TreeItem*beamInspector=rootItem;
    beamInspector->insertChildren(beamInspector->childCount(), 1, COLUMNS);
    beamInspector->child(beamInspector->childCount()-1)->setData(COLUMNS-1, beamInspectorData);
}

void TreeModel::myDataHasChanged()
{
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, 0);

    emit dataChanged(topLeft, bottomRight);
}
