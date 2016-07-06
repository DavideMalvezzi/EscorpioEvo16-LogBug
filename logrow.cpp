#include "logrow.h"

LogRow::LogRow(){

}


QString LogRow::getLogLevelString(){
    if(level == Info){
        return "Info";
    }
    else if(level == Warning){
        return "Warning";
    }
    return "Error";
}


