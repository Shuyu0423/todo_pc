#include "schedule_table.h"

#include <QMouseEvent>
#include <QTableWidgetSelectionRange>
#include <QDateTime>

#include <algorithm>

ScheduleTable::ScheduleTable(QWidget *parent) : QTableWidget(parent) {}

void ScheduleTable::setDragFinishedHandler(DragFinishedHandler handler) {
    dragFinishedHandler = std::move(handler);
}

void ScheduleTable::mousePressEvent(QMouseEvent *event) {
    const QModelIndex index = indexAt(event->pos());
    if (event->button() == Qt::LeftButton && index.isValid()) {
        dragging = true;
        pressTime = QDateTime::currentMSecsSinceEpoch();
        dragStartColumn = index.column();
        dragEndColumn = index.column();
        dragStartRow = index.row();
        dragEndRow = index.row();
        auto *pressedItem = item(dragStartRow, dragStartColumn);
        sourceItemId = pressedItem ? pressedItem->data(Qt::UserRole).toInt() : -1;
        sourceTitle = pressedItem ? pressedItem->toolTip() : QString();
        startedOnItem = sourceItemId > 0;
        updateDragSelection(dragEndColumn, dragEndRow);
    }

    QTableWidget::mousePressEvent(event);
}

void ScheduleTable::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        const QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            updateDragSelection(index.column(), index.row());
        }
    }

    QTableWidget::mouseMoveEvent(event);
}

void ScheduleTable::mouseReleaseEvent(QMouseEvent *event) {
    if (dragging && event->button() == Qt::LeftButton && dragFinishedHandler) {
        ScheduleSelection selection;
        selection.startColumn = std::min(dragStartColumn, dragEndColumn);
        selection.endColumn = std::max(dragStartColumn, dragEndColumn);
        selection.startRow = std::min(dragStartRow, dragEndRow);
        selection.endRow = std::max(dragStartRow, dragEndRow);
        selection.anchorColumn = dragStartColumn;
        selection.anchorRow = dragStartRow;
        selection.dropColumn = dragEndColumn;
        selection.dropRow = dragEndRow;
        selection.sourceItemId = sourceItemId;
        selection.sourceTitle = sourceTitle;

        const bool longPress = QDateTime::currentMSecsSinceEpoch() - pressTime >= 550;
        const bool copyDrag = (event->modifiers() & Qt::ControlModifier) != 0;
        if (!startedOnItem) {
            selection.operation = ScheduleDragOperation::NewItem;
        } else if (copyDrag) {
            selection.operation = ScheduleDragOperation::CopyItem;
        } else if (longPress) {
            selection.operation = ScheduleDragOperation::ResizeItem;
        } else {
            selection.operation = ScheduleDragOperation::MoveItem;
        }

        dragFinishedHandler(selection);
    }

    dragging = false;
    startedOnItem = false;
    sourceItemId = -1;
    sourceTitle.clear();
    QTableWidget::mouseReleaseEvent(event);
}

void ScheduleTable::updateDragSelection(int column, int row) {
    dragEndColumn = column;
    dragEndRow = row;

    clearSelection();
    setRangeSelected(
        QTableWidgetSelectionRange(
            std::min(dragStartRow, dragEndRow),
            std::min(dragStartColumn, dragEndColumn),
            std::max(dragStartRow, dragEndRow),
            std::max(dragStartColumn, dragEndColumn)),
        true);
}
