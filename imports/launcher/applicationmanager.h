/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2017 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#pragma once

#include <QtCore/QLoggingCategory>
#include <QtCore/QObject>

#include <Qt5GSettings/QGSettings>

class QDomElement;
class Application;

using namespace QtGSettings;

Q_DECLARE_LOGGING_CATEGORY(APPLICATION_MANAGER)

class ApplicationManager : public QObject
{
    Q_OBJECT
public:
    ApplicationManager(QObject *parent = nullptr);

    Application *getApplication(const QString &appId);

    QList<Application *> applications() const;
    QList<Application *> pinnedApps() const;

    Q_INVOKABLE void registerShellSurface(QObject *shellSurface);
    Q_INVOKABLE void unregisterShellSurface(QObject *shellSurface);
    Q_INVOKABLE void focusShellSurface(QObject *shellSurface);

    Q_INVOKABLE QString canonicalizeAppId(const QString &appId);

    Q_INVOKABLE QString getIconName(const QString &appId);

public Q_SLOTS:
    void refresh();
    void quit(const QString &appId);
    void launch(const QString &appId);

Q_SIGNALS:
    void applicationLaunched(Application *app);
    void refreshed();
    void applicationAdded(Application *app);

private:
    QGSettings *m_settings = nullptr;
    QMap<QString, Application *> m_apps;
    QMap<QObject *, QString> m_shellSurfaces;

    void readAppLink(const QDomElement &xml, const QString &categoryName);
};
