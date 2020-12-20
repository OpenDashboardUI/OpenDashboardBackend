// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Drawer {
	id: customDrawer

	property alias model: listView.model

	width: parent.width / 4
	height: parent.height

	background: Rectangle {
		color: "black"
	}

	ListView {
		id: listView

		anchors.fill: parent
	}


	CustomDrawerElement {

		anchors.bottom: parent.bottom
		text: "<<"
		height: Screen.pixelDensity * 5

		onClicked: customDrawer.close()
	}

	MouseArea {
		id: autoCloseMouseArea

		anchors.fill: parent
		propagateComposedEvents: true

		onClicked: {
			close()
			mouse.accepted = false
		}
	}

}
