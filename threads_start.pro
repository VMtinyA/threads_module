QT += core
QT -= gui

CONFIG += c++11

TARGET = threads_start
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.C \
    threads_start.C \
    abstract_thread.C \
    fake_signal_emitter.C

HEADERS += \
    abstract_thread.h \
    interfaces_main.h
