/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
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

 import QtQuick 2.0
 import QtQuick.Window 2.0
 import GreenIsland 1.0 as GreenIsland

Window {
    id: window

    property alias screenViewComponent: screenViewLoader.sourceComponent
    property alias screenView: screenViewLoader.item

    property var output

    x: nativeScreen.position.x
    y: nativeScreen.position.y
    width: nativeScreen.size.width
    height: nativeScreen.size.height
    flags: Qt.FramelessWindowHint

    Binding {
        target: screenView
        property: "output"
        value: window.output
    }

    GreenIsland.WaylandMouseTracker {
        id: localPointerTracker
        anchors.fill: parent
        windowSystemCursorEnabled: false

        Loader {
            id: screenViewLoader
            anchors.fill: parent
        }

        GreenIsland.WaylandCursorItem {
            id: cursor

            seat: output.compositor.defaultSeat
            x: localPointerTracker.mouseX - hotspotX
            y: localPointerTracker.mouseY - hotspotY
            visible: localPointerTracker.containsMouse && screenView.cursorVisible
        }
    }
}
