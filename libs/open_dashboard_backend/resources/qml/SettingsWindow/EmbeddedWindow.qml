// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

import "../"

Rectangle {
	id: embeddedWindow

	color: "transparent"
	border.color: "white"
	border.width: 1
	radius: 10
	clip: true
	enabled: visible

	property real fonSizePx: Screen.pixelDensity * 2.5
	property alias title: titleLabel.text
	property real backgroundOpacity: 0.9

	default property alias content: contentItem.data

	Rectangle {
		id: header

		color: "#161618"
		opacity: backgroundOpacity
		x: 1
		y: 1
		radius: 10

		width: parent.width - 2
		height: Screen.pixelDensity * 10

		Label {
			id: titleLabel
			anchors.centerIn: parent
			font.pixelSize: parent.height * 0.25
		}

		MouseArea {
			anchors.fill: parent
			cursorShape: Qt.SizeAllCursor
			drag.target: embeddedWindow
			drag.axis: Drag.XandYAxis
			drag.minimumX: 0
			drag.maximumX: embeddedWindow.parent.width - embeddedWindow.width
			drag.minimumY: 0
			drag.maximumY: embeddedWindow.parent.height - embeddedWindow.height
		}

		Rectangle {

			height: parent.height * 0.5
			width: height
			radius: 5

			color: closeButtonMouseArea.containsMouse ? "black" : "transparent"

			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: height/2

			Svg {
				source: "qrc:/svg-multilayer-extracted/icons_cancel.svg"
			}

			MouseArea {
				id: closeButtonMouseArea

				hoverEnabled: true
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor

				onClicked: embeddedWindow.visible = false
			}
		}
	}

	Rectangle {
		id: contentBackground

		color: "#262626"
		opacity: backgroundOpacity

		anchors.top: header.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		anchors.topMargin: 1
		anchors.bottomMargin: 1
		anchors.leftMargin: 1
		anchors.rightMargin: 1
	}

	Item {
		id: contentItem

		anchors.top: header.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
	}
}

