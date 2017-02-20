QT += core
QT -= gui

CONFIG += c++11

TARGET = threads_start
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.C \
    sync_threads.C \
    async_threads.C \
    threads_start.C

HEADERS += \
    sync_threads.h \
    async_threads.h \
    threads_start.h
