/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Material 1.0

Item {
    id: decoration

    readonly property int marginSize: shellSurface.fullscreen ? 0 : (shellSurface.windowType === Qt.Popup ? 1 : 2)
    readonly property int titleBarHeight: shellSurface.fullscreen || shellSurface.windowType === Qt.Popup ? 0 : 32
    readonly property bool dragging: moveArea.drag.active
    readonly property bool hasDropShadow: !shellSurface.maximized && !shellSurface.fullscreen

    Material.theme: Material.Dark

    Rectangle {
        id: frame

        anchors.fill: parent

        border.color: shellSurface.windowType === Qt.Popup ? "transparent" : Material.color(Material.Blue, Material.Shade500)
        border.width: 2
        color: "transparent"
        radius: 3
        smooth: true
        visible: shellSurface.decorated && !shellSurface.fullscreen

        // FIXME: Transparent backgrounds will be opaque due to shadows
        layer.enabled: hasDropShadow
        layer.effect: ElevationEffect {
            elevation: shellSurfaceItem.focus ? 24 : 8
        }

        MouseArea {
            id: resizeArea

            property int pressX
            property int pressY
            property int startW
            property int startH
            property bool pressed: false

            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeFDiagCursor

            //bitfield: top, left, bottom, right
            property int edges
            onPressed: {
                pressed = true;
                edges = 0;
                pressX = mouse.x;
                pressY = mouse.y;
                startW = chrome.width;
                startH = chrome.height;
                if (mouse.y > chrome.height - titleBarHeight)
                    edges |= 4; //bottom edge
                if (mouse.x > chrome.width - titleBarHeight)
                    edges |= 8; //right edge
            }
            onReleased: pressed = false
            onMouseXChanged: {
                if (pressed) {
                    var w = startW;
                    var h = startH;
                    if (edges & 8)
                        w += mouse.x - pressX;
                    if (edges & 4)
                        h += mouse.y - pressY;
                    shellSurface.requestSize(w, h);
                    console.warn("resize " + chrome + " " + chrome.x + ", ", chrome.y)
                }
            }
        }

        Rectangle {
            id: titleBar

            anchors {
                margins: marginSize
                left: parent.left
                top: parent.top
                right: parent.right
            }
            height: titleBarHeight
            color: Material.color(Material.Blue)
            visible: shellSurface.windowType !== Qt.Popup

            Item {
                anchors.fill: parent

                Icon {
                    id: icon

                    anchors {
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }

                    name: shellSurface.iconName
                    width: 24
                    height: width
                    visible: name != "" && status == Image.Ready
                }

                SubheadingLabel {
                    id: titleBarLabel

                    anchors {
                        left: icon.visible ? icon.right : parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    opacity: shellSurface.activated ? 1.0 : 0.5
                    color: Material.primaryTextColor
                    text: shellSurface.title
                }

                Row {
                    id: windowControls

                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }

                    spacing: 12
                    opacity: shellSurface.activated ? 1.0 : 0.5

                    DecorationButton {
                        source: "window-minimize.svg"
                        onClicked: shellSurface.minimized = true
                    }

                    DecorationButton {
                        source: shellSurface.maximized ? "window-restore.svg" : "window-maximize.svg"
                        onClicked: shellSurface.sendMaximized(Qt.size(100, 100))
                    }

                    DecorationButton {
                        source: "window-close.svg"
                        onClicked: shellSurface.close()
                    }
                }
            }

            MouseArea {
                id: moveArea
                anchors.fill: parent
                enabled: false
                drag.target: chrome
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onPressed: {
                    if (mouse.button === Qt.LeftButton)
                        shellSurfaceItem.takeFocus();
                    else if (mouse.button === Qt.RightButton)
                        chrome.showWindowMenu(mouse.x, mouse.y);
                }
            }
        }
    }
}
