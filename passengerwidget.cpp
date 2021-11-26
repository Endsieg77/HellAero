#include "widgetsfwd.h"
#include <QtWidgets>

passengerWidget::passengerWidget(queryHelper *_Qh, QWidget *caller, QWidget *parent)
    : QWidget(parent), _query_helper {_Qh}, _caller {caller}
{

}
