cmake_minimum_required(VERSION 3.20)

project(TodoList LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Sql Widgets)

qt_standard_project_setup()

qt_add_executable(todo_app
    floating_focus_widget.cpp
    floating_focus_widget.h
    main.cpp
    resources.qrc
    schedule_table.cpp
    schedule_table.h
    study_calendar_widget.cpp
    study_calendar_widget.h
    todo_window.cpp
    todo_window.h
)

target_link_libraries(todo_app PRIVATE Qt6::Sql Qt6::Widgets)
