#include "floating_focus_widget.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

FloatingFocusWidget::FloatingFocusWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(286, 118);

    auto *container = new QWidget(this);
    container->setObjectName("FloatingContainer");
    container->setGeometry(rect());

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(6);

    auto *header = new QHBoxLayout();
    auto *caption = new QLabel("专注中");
    caption->setObjectName("FloatingCaption");
    header->addWidget(caption);
    header->addStretch(1);
    auto *restoreButton = new QPushButton("展开");
    restoreButton->setObjectName("FloatingButton");
    header->addWidget(restoreButton);
    layout->addLayout(header);

    taskLabel = new QLabel("选择一个任务开始专注");
    taskLabel->setObjectName("FloatingTask");
    taskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(taskLabel);

    auto *timerRow = new QHBoxLayout();
    timeLabel = new QLabel("25:00");
    timeLabel->setObjectName("FloatingTimer");
    timerRow->addWidget(timeLabel);
    timerRow->addStretch(1);
    toggleButton = new QPushButton("开始");
    toggleButton->setObjectName("FloatingPrimaryButton");
    timerRow->addWidget(toggleButton);
    layout->addLayout(timerRow);

    setStyleSheet(R"(
        #FloatingContainer {
            background: rgba(255, 255, 255, 172);
            border: 1px solid rgba(226, 230, 244, 150);
            border-radius: 18px;
        }
        #FloatingCaption {
            color: rgba(72, 78, 104, 210);
            font-size: 12px;
            font-weight: 700;
        }
        #FloatingTask {
            color: rgba(34, 39, 61, 230);
            font-size: 13px;
            font-weight: 700;
        }
        #FloatingTimer {
            color: rgba(55, 67, 210, 235);
            font-size: 26px;
            font-weight: 800;
        }
        #FloatingButton, #FloatingPrimaryButton {
            background: rgba(245, 247, 255, 150);
            border: 1px solid rgba(225, 229, 251, 150);
            border-radius: 9px;
            color: rgba(69, 78, 168, 230);
            font-size: 12px;
            font-weight: 700;
            padding: 5px 9px;
        }
        #FloatingPrimaryButton {
            background: rgba(88, 101, 242, 210);
            border-color: rgba(88, 101, 242, 180);
            color: #ffffff;
        }
    )");

    connect(restoreButton, &QPushButton::clicked, this, [this] {
        if (restoreHandler) restoreHandler();
    });
    connect(toggleButton, &QPushButton::clicked, this, [this] {
        if (toggleHandler) toggleHandler();
    });
}

void FloatingFocusWidget::setTaskText(const QString &text) {
    taskLabel->setText(text);
}

void FloatingFocusWidget::setTimeText(const QString &text) {
    timeLabel->setText(text);
}

void FloatingFocusWidget::setRunning(bool running) {
    toggleButton->setText(running ? "暂停" : "开始");
}

void FloatingFocusWidget::setRestoreHandler(std::function<void()> handler) {
    restoreHandler = std::move(handler);
}

void FloatingFocusWidget::setToggleHandler(std::function<void()> handler) {
    toggleHandler = std::move(handler);
}

void FloatingFocusWidget::moveToScreenCorner() {
    const QRect available = QGuiApplication::primaryScreen()->availableGeometry();
    move(available.right() - width() - 22, available.bottom() - height() - 22);
}

void FloatingFocusWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && restoreHandler) {
        restoreHandler();
    }
    QWidget::mouseDoubleClickEvent(event);
}
