#include "titlebar.h"
#include <QtWidgets>

TitleBar::TitleBar(const QString &title, QWidget *parent)
    : QWidget(nullptr)
{
    parent ?
       setFixedSize(parent->width(), 100) :
       setFixedSize(QSize(800, 100));

    titleIcon = new QLabel(this);
    titleIcon->setObjectName("TitleIcon");
    titleIcon->setFixedSize(QSize(55, 55));
    titleIcon->setScaledContents(true);
    titleIcon->setPixmap(QPixmap(":/resources/images/titlebar/titleicon.png"));

    windowTitle = new QLabel(this);
    windowTitle->setObjectName("WindowTitle");
    windowTitle->setText(title);
    windowTitle->setAlignment(Qt::AlignCenter);

    close = new QToolButton(this);
    close->setObjectName("titlebar/close");
    close->installEventFilter(this);
    close->setIcon(QPixmap(":/resources/images/titlebar/close.png"));
    close->setToolTip("Close");

    minimum = new QToolButton(this);
    minimum->setObjectName("titlebar/minimum");
    minimum->installEventFilter(this);
    minimum->setIcon(QPixmap(":/resources/images/titlebar/minimum.png"));
    minimum->setToolTip("Minimum");

    titleBarLayout = new QHBoxLayout(this);
    titleBarLayout->setMargin(20);
    titleBarLayout->addWidget(titleIcon);
    titleBarLayout->addWidget(windowTitle);
    titleBarLayout->addWidget(minimum);
    titleBarLayout->addWidget(close);
    titleBarLayout->setStretch(0, 2);
    titleBarLayout->setStretch(1, 4);
    titleBarLayout->setStretch(2, 1);
    titleBarLayout->setStretch(3, 1);
    titleBarLayout->setAlignment(Qt::AlignCenter);

    titleConnectionsBuilder(parent);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        last_mouse_position_ = event->globalPos();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
        return;
    auto ptrParent = static_cast<QWidget *>(parent());
    if (ptrParent)
    {
        const QPoint parent_pos = ptrParent->pos() + event->globalPos() - last_mouse_position_;
        ptrParent->move(parent_pos);
        last_mouse_position_ = event->globalPos();
    }
    else {
        const QPoint position = pos() + event->globalPos() - last_mouse_position_;
        move(position);
        last_mouse_position_ = event->globalPos();
    }
}

void TitleBar::setText(const QString &_text)
{ windowTitle->setText(_text); }

void TitleBar::titleConnectionsBuilder(QWidget *widget)
{
    if (!widget)
    {
        connect(minimum, &QToolButton::clicked, this, &QWidget::showMinimized);
        connect(close, &QToolButton::clicked, this, &QWidget::close);
        return;
    }
    connect(minimum, &QToolButton::clicked, widget, &QWidget::showMinimized);
    connect(close, &QToolButton::clicked, widget, &QWidget::close);
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if(close   == watched ||
       minimum == watched)
    {
        auto thisBtn = static_cast<QToolButton *>(watched);
        if(QEvent::Enter == event->type())
        {
            thisBtn->setIcon(QPixmap(":/resources/images/" + thisBtn->objectName() + "-hover.png"));
        } else if(QEvent::Leave == event->type())
        {
            thisBtn->setIcon(QPixmap(":/resources/images/" + thisBtn->objectName() + ".png"));
        }
    }
    return 0;
}

void TitleBar::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)

    QStyleOption opt;

    opt.init(this);

    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}
