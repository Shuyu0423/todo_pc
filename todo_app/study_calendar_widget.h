#pragma once

#include <QCalendarWidget>
#include <QSet>

class StudyCalendarWidget : public QCalendarWidget {
public:
    explicit StudyCalendarWidget(QWidget *parent = nullptr);

    void setStudiedDates(const QSet<QDate> &dates);

protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;

private:
    QSet<QDate> studiedDates;
};
