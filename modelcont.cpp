#include "modelcont.h"

ModelCont::ModelCont(QObject *parent) : DbTableModel("dry_prog",parent)
{
    currentId=-1;
    addColumn("id",tr("id"),true,false,TYPE_INT);
    addColumn("id_grp",tr("Группа"),true,false,TYPE_INT);
    addColumn("step",tr("Шаг"),true,false,TYPE_INT,new QIntValidator(0,50,this));
    addColumn("tim",tr("Время, мин."),false,false,TYPE_DOUBLE,new QDoubleValidator(0,1000,1,this));
    addColumn("tra",tr("Темпер., град."),false,false,TYPE_DOUBLE,new QDoubleValidator(0,1000,1,this));
    setSort("dry_prog.step");
}

bool ModelCont::insertRow(int row, const QModelIndex &parent)
{
    int step=1;
    if (rowCount()>0) step=this->data(this->index(rowCount()-1,2),Qt::EditRole).toInt()+1;
    setDefaultValue(2,step);
    return DbTableModel::insertRow(row,parent);
}

void ModelCont::copyCnt(int srcId, int srcGrp)
{
    QSqlQuery query;
    query.prepare("insert into dry_prog (id, step, tra, tim, id_grp) (select :id, p.step, p.tra, p.tim, :id_grp from dry_prog as p where p.id = :srcid and p.id_grp = :srcgrp)");
    query.bindValue(":id",currentId);
    query.bindValue(":id_grp",currentGrp);
    query.bindValue(":srcid",srcId);
    query.bindValue(":srcgrp",srcGrp);
    if (query.exec()){
        refresh(currentId,currentGrp);
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void ModelCont::refresh(int id, int id_grp)
{
    currentId=id;
    currentGrp=id_grp;
    setFilter("dry_prog.id = "+QString::number(id)+" and dry_prog.id_grp= "+QString::number(id_grp));
    setDefaultValue(0,id);
    setDefaultValue(1,id_grp);
    setDefaultValue(2,1);
    select();
}
