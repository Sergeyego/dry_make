#ifndef MODELCONT_H
#define MODELCONT_H

#include <QObject>
#include "dbtablemodel.h"

class ModelCont : public DbTableModel
{
    Q_OBJECT
public:
    ModelCont(QObject *parent);
    bool insertRow(int row, const QModelIndex &parent=QModelIndex());
    void copyCnt(int srcId, int srcGrp);
public slots:
    void refresh(int id, int id_grp);
private:
    int currentId;
    int currentGrp;
};

#endif // MODELCONT_H
