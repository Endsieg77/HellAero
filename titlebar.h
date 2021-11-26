#ifndef TITLEBAR
#define TITLEBAR

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(const QString &title, QWidget *parent = nullptr);
    void titleConnectionsBuilder(QWidget *widget);
    void setText(const QString &_text);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint last_mouse_position_;
    QLabel *titleIcon;
    QLabel *windowTitle;
    QAbstractButton *close;
    QAbstractButton *minimum;
    QBoxLayout *titleBarLayout;
};
#endif // TITLEBAR_V2_H
