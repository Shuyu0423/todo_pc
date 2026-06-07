#include "schedule_table.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QStyledItemDelegate>
#include <QTableWidgetSelectionRange>
#include <QDateTime>

#include <algorithm>

namespace {
class ScheduleItemDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        const int id = index.data(Qt::UserRole).toInt();
        if (id <= 0) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        auto hasSameId = [&](int row, int column) {
            if (row < 0 || column < 0 || row >= index.model()->rowCount() || column >= index.model()->columnCount()) {
                return false;
            }
            return index.model()->index(row, column).data(Qt::UserRole).toInt() == id;
        };

        const bool sameLeft = hasSameId(index.row(), index.column() - 1);
        const bool sameRight = hasSameId(index.row(), index.column() + 1);
        const bool sameTop = hasSameId(index.row() - 1, index.column());
        const bool sameBottom = hasSameId(index.row() + 1, index.column());

        QRectF block = option.rect.adjusted(sameLeft ? -1 : 5, sameTop ? -1 : 5, sameRight ? 1 : -5, sameBottom ? 1 : -5);
        const qreal radius = 10.0;
        const bool roundTopLeft = !sameLeft && !sameTop;
        const bool roundTopRight = !sameRight && !sameTop;
        const bool roundBottomRight = !sameRight && !sameBottom;
        const bool roundBottomLeft = !sameLeft && !sameBottom;

        QPainterPath path;
        path.moveTo(block.left() + (roundTopLeft ? radius : 0), block.top());
        path.lineTo(block.right() - (roundTopRight ? radius : 0), block.top());
        if (roundTopRight) {
            path.quadTo(block.right(), block.top(), block.right(), block.top() + radius);
        } else {
            path.lineTo(block.right(), block.top());
        }
        path.lineTo(block.right(), block.bottom() - (roundBottomRight ? radius : 0));
        if (roundBottomRight) {
            path.quadTo(block.right(), block.bottom(), block.right() - radius, block.bottom());
        } else {
            path.lineTo(block.right(), block.bottom());
        }
        path.lineTo(block.left() + (roundBottomLeft ? radius : 0), block.bottom());
        if (roundBottomLeft) {
            path.quadTo(block.left(), block.bottom(), block.left(), block.bottom() - radius);
        } else {
            path.lineTo(block.left(), block.bottom());
        }
        path.lineTo(block.left(), block.top() + (roundTopLeft ? radius : 0));
        if (roundTopLeft) {
            path.quadTo(block.left(), block.top(), block.left() + radius, block.top());
        } else {
            path.lineTo(block.left(), block.top());
        }
        path.closeSubpath();

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(Qt::NoPen);
        painter->setBrush((option.state & QStyle::State_Selected) ? QColor("#dfe8ff") : QColor("#e6efff"));
        painter->drawPath(path);

        if (!index.data(Qt::DisplayRole).toString().isEmpty()) {
            QFont font = option.font;
            font.setWeight(QFont::DemiBold);
            painter->setFont(font);
            painter->setPen(QColor("#4050c8"));
            painter->drawText(option.rect.adjusted(10, 0, -8, 0), Qt::AlignVCenter | Qt::AlignLeft, index.data(Qt::DisplayRole).toString());
        }
        painter->restore();
    }
};
}

ScheduleTable::ScheduleTable(QWidget *parent) : QTableWidget(parent) {
    setItemDelegate(new ScheduleItemDelegate(this));
}

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
