#pragma once

#include <QWidget>

#include <functional>

class QLabel;
class QPushButton;

class FloatingFocusWidget : public QWidget {
public:
    explicit FloatingFocusWidget(QWidget *parent = nullptr);

    void setTaskText(const QString &text);
    void setTimeText(const QString &text);
    void setRunning(bool running);
    void setRestoreHandler(std::function<void()> handler);
    void setToggleHandler(std::function<void()> handler);
    void moveToScreenCorner();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel *taskLabel = nullptr;
    QLabel *timeLabel = nullptr;
    QPushButton *toggleButton = nullptr;
    std::function<void()> restoreHandler;
    std::function<void()> toggleHandler;
};
