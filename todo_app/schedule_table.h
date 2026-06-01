#pragma once

#include <QTableWidget>

#include <functional>

enum class ScheduleDragOperation {
    NewItem,
    MoveItem,
    CopyItem,
    ResizeItem
};

struct ScheduleSelection {
    int startColumn = -1;
    int endColumn = -1;
    int startRow = -1;
    int endRow = -1;
    int anchorColumn = -1;
    int anchorRow = -1;
    int dropColumn = -1;
    int dropRow = -1;
    int sourceItemId = -1;
    QString sourceTitle;
    ScheduleDragOperation operation = ScheduleDragOperation::NewItem;
};

class ScheduleTable : public QTableWidget {
public:
    using DragFinishedHandler = std::function<void(const ScheduleSelection &)>;

    explicit ScheduleTable(QWidget *parent = nullptr);

    void setDragFinishedHandler(DragFinishedHandler handler);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void updateDragSelection(int column, int row);

    DragFinishedHandler dragFinishedHandler;
    bool dragging = false;
    bool startedOnItem = false;
    int dragStartColumn = -1;
    int dragEndColumn = -1;
    int dragStartRow = -1;
    int dragEndRow = -1;
    int sourceItemId = -1;
    QString sourceTitle;
    qint64 pressTime = 0;
};
