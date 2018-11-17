# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# Application version.
VERSION = 0.7.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT += network

# The name of your application
TARGET = harbour-sailTrivia

CONFIG += sailfishapp

SOURCES += src/harbour-sailTrivia.cpp \
    src/dataloader.cpp \
    src/categorymodel.cpp \
    src/category.cpp \
    src/question.cpp \
    src/questionmodel.cpp \
    src/statistics.cpp \
    src/stat.cpp

DISTFILES += qml/harbour-sailTrivia.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-sailTrivia.changes.in \
    rpm/harbour-sailTrivia.changes.run.in \
    rpm/harbour-sailTrivia.spec \
    rpm/harbour-sailTrivia.yaml \
    translations/*.ts \
    harbour-sailTrivia.desktop \
    qml/pages/StatisticsPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/QuestionPage.qml \
    qml/pages/EndGamePage.qml \
    qml/js/statistics.js

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-sailTrivia-de.ts

HEADERS += \
    src/dataloader.h \
    src/difficulty.h \
    src/categorymodel.h \
    src/category.h \
    src/question.h \
    src/questionmodel.h \
    src/jsonconstants.h \
    src/statistics.h \
    src/stat.h
