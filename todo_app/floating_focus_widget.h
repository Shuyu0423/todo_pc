#pragma once

#include <QPoint>
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
    void setResetHandler(std::function<void()> handler);
    void moveToScreenCorner();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLabel *taskLabel = nullptr;
    QLabel *timeLabel = nullptr;
    QPushButton *toggleButton = nullptr;
    std::function<void()> restoreHandler;
    std::function<void()> toggleHandler;
    std::function<void()> resetHandler;
    bool dragging = false;
    QPoint dragOffset;
};
