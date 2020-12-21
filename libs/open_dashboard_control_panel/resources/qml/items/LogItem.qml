// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
	id: logItem

	property alias backgroundOpactity: backgroundRectangle.opacity

	clip: true

	Rectangle {
		id: backgroundRectangle

		anchors.fill: parent
		color: "black"
		radius: 10
	}

	Item {
		id: centralItem

		anchors.fill: parent

		anchors.leftMargin: parent.width * 0.01
		anchors.rightMargin: parent.width * 0.01
		anchors.topMargin: parent.width * 0.01
		anchors.bottomMargin: parent.width * 0.01

		Label {
			id: placeholderLabel

			anchors.fill: parent
			text: "placeholder"
			color: "white"
		}
	}
}
