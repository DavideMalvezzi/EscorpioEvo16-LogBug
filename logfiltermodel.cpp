#include "logfiltermodel.h"


LogFilterModel::LogFilterModel(QObject* parent) : QSortFilterProxyModel(parent){

}

void LogFilterModel::updateFilter(bool showInfo, bool showWarn, bool showErr, QString tag){
    this->showInfo = showInfo;
    this->showWarn = showWarn;
    this->showErr = showErr;
    this->tag = tag;
    invalidateFilter();
}

bool LogFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    Q_UNUSED(source_parent);

    LogModel* logModel = (LogModel*)sourceModel();
    LogRow* row = logModel->getRow(source_row);

    if(tag != "" && !row->getTag().startsWith(tag)){
        return false;
    }

    if(row->getLogLevel() == Info && !showInfo){
        return false;
    }
    else if(row->getLogLevel() == Warning && !showWarn){
        return false;
    }
    else if(row->getLogLevel() == Error && !showErr){
        return false;
    }

    return true;
}
