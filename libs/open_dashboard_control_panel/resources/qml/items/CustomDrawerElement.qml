// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Window 2.15

MouseArea {
	id: customDrawerElement

	property alias text: textField.text

	width: parent === null ? 200 : parent.width
	height: textField.font.pixelSize * 2

	hoverEnabled: true

	Rectangle {
		anchors.fill: parent
		color: containsMouse ? "#141414" : "black"
	}

	Text {
		id: textField

		anchors.fill: parent
		color: "white"
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter

		font.pointSize: 12
	}
}
