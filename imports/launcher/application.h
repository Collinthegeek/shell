/*
 * QML Desktop - Set of tools written in C++ for QML
 *
 * Copyright (C) 2017 Michael Spencer <sonrisesoftware@gmail.com>
 *               2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QStringList>

#include <LiriCore/DesktopFile>

class QWaylandClient;

using namespace Liri;

class ApplicationManager;

class Application : public QObject
{
    Q_OBJECT

    // Properties from the desktop file

    Q_PROPERTY(QString appId READ appId CONSTANT)
    Q_PROPERTY(DesktopFile *desktopFile READ desktopFile CONSTANT)
    Q_PROPERTY(bool valid READ isValid CONSTANT)

    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(QString genericName READ genericName NOTIFY dataChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY dataChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY dataChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY dataChanged)

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY stateChanged)
    Q_PROPERTY(bool starting READ isStarting NOTIFY stateChanged)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
    Q_PROPERTY(bool pinned READ isPinned WRITE setPinned NOTIFY pinnedChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)

    friend class ApplicationManager;

public:
    enum State
    {
        //! The application is not running
        NotRunning,
        //! The application was launched and it's starting up.
        Starting,
        //! The application is currently running.
        Running,
        //! The application is in the background and it was suspended by
        //! the system to save resources.
        Suspended,
        //! The application is in the background and its activity was
        //! stopped by the system in order to save resources, acts as
        //! being closed but the state is saved on disk and can be
        //! restored.
        Stopped
    };
    Q_ENUM(State)

    Application(const QString &appId, const QStringList &categories, QObject *parent = nullptr);

    bool isValid() const { return m_desktopFile != nullptr && m_desktopFile->isValid(); }

    bool hasCategory(const QString &category) const;

    QString name() const { return m_desktopFile->name(); }
    QString genericName() const { return m_desktopFile->genericName(); }
    QString comment() const { return m_desktopFile->comment(); }
    QString iconName() const { return m_desktopFile->iconName(); }
    QStringList categories() const { return m_categories; }

    /*!
     * \brief Application state.
     *
     * Holds the current state of the application.
     */
    State state() const { return m_state; }

    /*!
     * \brief Application identifier.
     *
     * Holds the application identifier according to the Freedesktop
     * Desktop Entry specification.
     */
    QString appId() const { return m_appId; }

    /*!
     * \brief Desktop entry file name.
     *
     * Returns the desktop entry file name.
     */
    DesktopFile *desktopFile() const { return m_desktopFile; }

    /*!
     * \brief Application active state.
     *
     * Returns whether the application is currently active or not.
     */
    bool isActive() const { return m_active; }

    bool isRunning() const { return m_state == Application::Running; }
    bool isStarting() const { return m_state == Application::Starting; }

    int count() const { return m_count; }
    int progress() const { return m_progress; }

    bool isPinned() const { return m_pinned; }

public Q_SLOTS:
    void setPinned(bool pinned);

    Q_INVOKABLE bool launch(const QStringList &urls);
    Q_INVOKABLE bool quit();

    Q_INVOKABLE bool launch() { return launch(QStringList()); }

protected Q_SLOTS:
    void setState(State state);
    void setActive(bool active);

Q_SIGNALS:
    void stateChanged();
    void activeChanged();
    void pinnedChanged();
    void countChanged();
    void progressChanged();
    void launched();
    void dataChanged();

protected:
    QSet<pid_t> m_pids;
    QVector<QWaylandClient *> m_clients;

private:
    QString m_appId;
    QStringList m_categories;
    DesktopFile *m_desktopFile = nullptr;
    bool m_active = false;
    bool m_pinned = false;
    int m_count = 0;
    int m_progress = 0;
    State m_state = NotRunning;

    void addClient(QWaylandClient *client);
};
