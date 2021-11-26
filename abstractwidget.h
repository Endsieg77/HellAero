#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)

class abstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit abstractWidget(QWidget *_son, QWidget *parent = nullptr);

protected:
    TitleBar *_title_bar;
    QBoxLayout *_main_layout;
};

#endif // ABSTRACTWIDGET_H
