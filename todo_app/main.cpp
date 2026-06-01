#include <QApplication>
#include <QCoreApplication>
#include <QTimer>

#include "todo_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Codex");
    QCoreApplication::setApplicationName("TodoList");

    TodoWindow window;
    window.show();

    if (QCoreApplication::arguments().contains("--smoke-test")) {
        QTimer::singleShot(300, &app, &QApplication::quit);
    }

    return app.exec();
}
