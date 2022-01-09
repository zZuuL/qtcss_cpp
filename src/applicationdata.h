#ifndef __application_data_H
#define __application_data_H

#include <QString>

namespace qtcss {

struct Files {
    QString ui;
    QString css;
    bool isEmpty() const {
        return ui.isEmpty() || css.isEmpty();
    }
};

}

#endif /*__application_data_H*/
