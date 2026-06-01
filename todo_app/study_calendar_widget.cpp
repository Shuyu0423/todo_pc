#include "study_calendar_widget.h"

#include <QPainter>
#include <QTextCharFormat>

StudyCalendarWidget::StudyCalendarWidget(QWidget *parent) : QCalendarWidget(parent) {
    setGridVisible(false);
    setFirstDayOfWeek(Qt::Monday);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setNavigationBarVisible(true);
}

void StudyCalendarWidget::setStudiedDates(const QSet<QDate> &dates) {
    studiedDates = dates;
    updateCells();
}

void StudyCalendarWidget::paintCell(QPainter *painter, const QRect &rect, QDate date) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    const bool currentMonth = date.month() == monthShown() && date.year() == yearShown();
    const bool studied = studiedDates.contains(date);
    const bool today = date == QDate::currentDate();
    const bool selected = date == selectedDate();

    QRect cell = rect.adjusted(5, 4, -5, -4);
    QColor background = currentMonth ? QColor("#ffffff") : QColor("#f7f8fc");
    QColor foreground = currentMonth ? QColor("#343958") : QColor("#b7bdcc");

    if (studied) {
        background = QColor("#5865f2");
        foreground = QColor("#ffffff");
    } else if (selected) {
        background = QColor("#eef1ff");
        foreground = QColor("#4d59ca");
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(background);
    painter->drawRoundedRect(cell, 12, 12);

    if (today) {
        QPen pen(studied ? QColor("#ffffff") : QColor("#5865f2"));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(cell.adjusted(1, 1, -1, -1), 12, 12);
    }

    QFont font = painter->font();
    font.setBold(studied || today);
    painter->setFont(font);
    painter->setPen(foreground);
    painter->drawText(cell, Qt::AlignCenter, QString::number(date.day()));

    painter->restore();
}
