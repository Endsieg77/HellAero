#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(loginWidget)

class adminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit adminWidget(loginWidget *caller, QWidget *parent = nullptr);
    ~adminWidget() override;
    loginWidget *getLoginWidget() { return _caller; }

private:
    loginWidget *_caller;
    TitleBar *_window_hint;
    QBoxLayout *_ui_layout,
               *_btn_layout;
    QLabel *_text;
    QAbstractButton *_enter_modification_GUI,
                    *_data_query,
                    *_zuruck_login;
    QTimer *_timer;

private slots:
    void enterModification();
    void enterQuery();
    void zuruckLoginHelper();
    void updateText();
};

#endif // ADMINWIDGET_H
