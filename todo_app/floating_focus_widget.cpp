#include "floating_focus_widget.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>

FloatingFocusWidget::FloatingFocusWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(218, 218);
    setCursor(Qt::OpenHandCursor);

    auto *container = new QWidget(this);
    container->setObjectName("FloatingContainer");
    container->setGeometry(rect());
    auto *shadow = new QGraphicsDropShadowEffect(container);
    shadow->setBlurRadius(34);
    shadow->setOffset(0, 12);
    shadow->setColor(QColor(88, 96, 160, 48));
    container->setGraphicsEffect(shadow);

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(10);

    auto *header = new QHBoxLayout();
    header->setSpacing(8);
    auto *caption = new QLabel("专注中");
    caption->setObjectName("FloatingCaption");
    header->addWidget(caption);
    header->addStretch(1);
    auto *restoreButton = new QPushButton("关");
    restoreButton->setObjectName("FloatingButton");
    header->addWidget(restoreButton);
    layout->addLayout(header);

    taskLabel = new QLabel("选择一个任务开始专注");
    taskLabel->setObjectName("FloatingTask");
    taskLabel->setAlignment(Qt::AlignCenter);
    taskLabel->setWordWrap(true);
    taskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(taskLabel);

    timeLabel = new QLabel("25:00");
    timeLabel->setObjectName("FloatingTimer");
    timeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(timeLabel);

    auto *state = new QLabel("专注模式");
    state->setObjectName("FloatingState");
    state->setAlignment(Qt::AlignCenter);
    layout->addWidget(state);

    auto *timerRow = new QHBoxLayout();
    timerRow->setSpacing(14);
    timerRow->addStretch(1);
    auto *restoreMiniButton = new QPushButton("展");
    restoreMiniButton->setObjectName("FloatingRoundButton");
    timerRow->addWidget(restoreMiniButton);
    toggleButton = new QPushButton("开");
    toggleButton->setObjectName("FloatingPrimaryButton");
    timerRow->addWidget(toggleButton);
    auto *resetHintButton = new QPushButton("重");
    resetHintButton->setObjectName("FloatingRoundButton");
    timerRow->addWidget(resetHintButton);
    layout->addLayout(timerRow);

    // Transparency knobs: the last rgba value is alpha (0-255).
    // Lower values make the floating card more water-like; keep text alpha high
    // so the timer remains readable over busy backgrounds.
    setStyleSheet(R"(
        #FloatingContainer {
            background: rgba(255, 255, 255, 58);
            border: 1px solid rgba(220, 225, 245, 72);
            border-radius: 22px;
        }
        #FloatingCaption {
            color: rgba(84, 96, 210, 238);
            font-size: 12px;
            font-weight: 800;
        }
        #FloatingTask {
            color: rgba(46, 52, 82, 238);
            font-size: 12px;
            font-weight: 700;
        }
        #FloatingTimer {
            color: rgba(38, 43, 70, 248);
            font-size: 38px;
            font-weight: 800;
        }
        #FloatingState {
            background: rgba(230, 250, 240, 54);
            border: 1px solid rgba(193, 237, 213, 82);
            border-radius: 10px;
            color: rgba(42, 157, 106, 235);
            font-size: 11px;
            font-weight: 800;
            padding: 4px 10px;
        }
        #FloatingButton {
            background: transparent;
            border: none;
            color: rgba(85, 93, 120, 220);
            font-size: 11px;
            font-weight: 800;
            min-width: 22px;
            min-height: 22px;
            padding: 0;
        }
        #FloatingRoundButton, #FloatingPrimaryButton {
            background: rgba(248, 249, 255, 78);
            border: 1px solid rgba(226, 231, 247, 86);
            border-radius: 18px;
            color: rgba(90, 99, 132, 235);
            font-size: 13px;
            font-weight: 800;
            min-width: 36px;
            min-height: 36px;
            max-width: 36px;
            max-height: 36px;
            padding: 0;
        }
        #FloatingPrimaryButton {
            background: rgba(88, 101, 242, 150);
            border-color: rgba(88, 101, 242, 118);
            color: #ffffff;
            font-size: 13px;
        }
    )");

    connect(restoreButton, &QPushButton::clicked, this, [this] {
        if (restoreHandler) restoreHandler();
    });
    connect(restoreMiniButton, &QPushButton::clicked, this, [this] {
        if (restoreHandler) restoreHandler();
    });
    connect(toggleButton, &QPushButton::clicked, this, [this] {
        if (toggleHandler) toggleHandler();
    });
    connect(resetHintButton, &QPushButton::clicked, this, [this] {
        if (resetHandler) resetHandler();
    });
}

void FloatingFocusWidget::setTaskText(const QString &text) {
    taskLabel->setText(text);
}

void FloatingFocusWidget::setTimeText(const QString &text) {
    timeLabel->setText(text);
}

void FloatingFocusWidget::setRunning(bool running) {
    toggleButton->setText(running ? "停" : "开");
}

void FloatingFocusWidget::setRestoreHandler(std::function<void()> handler) {
    restoreHandler = std::move(handler);
}

void FloatingFocusWidget::setToggleHandler(std::function<void()> handler) {
    toggleHandler = std::move(handler);
}

void FloatingFocusWidget::setResetHandler(std::function<void()> handler) {
    resetHandler = std::move(handler);
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

void FloatingFocusWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void FloatingFocusWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - dragOffset);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void FloatingFocusWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && dragging) {
        dragging = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}
