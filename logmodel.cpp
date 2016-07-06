#include "logmodel.h"


LogModel::LogModel(QObject* parent): QAbstractTableModel(parent){

}

Qt::ItemFlags LogModel::flags(const QModelIndex & /*index*/) const{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
}

QVariant LogModel::data(const QModelIndex &index, int role) const{
    int row = index.row();
    int column = index.column();

    if(row < logRows.size()){
        LogRow* logRow = logRows.at(row);
        if (role == Qt::DisplayRole){
            switch (column){
                case 0:
                    return logRow->getTime();
                case 1:
                    return logRow->getLogLevelString();
                case 2:
                    return logRow->getTag();
                case 3:
                    return logRow->getMessage();
            }
        }

        else if (role == Qt::TextColorRole){
            switch (logRow->getLogLevel()) {
                case Info:
                    return QColor(255, 255, 255);
                case Warning:
                    return QColor(255, 255, 0);
                case Error:
                    return QColor(255, 0, 0);
            }


        }
        else if(role == Qt::TextAlignmentRole && column < 3){
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }

    return QVariant();
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section){
                case 0:
                    return QString("Time");
                case 1:
                    return QString("Level");
                case 2:
                    return QString("Tag");
                case 3:
                    return QString("Message");
            }
        }
    }
    return QVariant();
}

void LogModel::addRow(LogRow* row){
    emit layoutAboutToBeChanged();
    logRows.append(row);
    emit layoutChanged();
}

void LogModel::clear(){
    emit layoutAboutToBeChanged();
    while(!logRows.isEmpty()){
        delete logRows.takeFirst();
    }
    emit layoutChanged();
}



