// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

RowLayout {
	id: apiKeyItem

	property font font
	property alias value: apiKeyTextEdit.text

	width: parent.width
	height: font.pixelSize * 1.5

	spacing: 10

	signal apply()

	Label {
		text: "API Key:"
		font: apiKeyItem.font
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
	}

	Rectangle {
		Layout.fillWidth: true
		height: parent.height
		color: "black"
		border.color: "white"
		border.width: 1

		TextEdit {
			id: apiKeyTextEdit

			color: "white"
			text: "API_KEY"

			anchors.fill: parent
			verticalAlignment: Text.AlignVCenter
			font: apiKeyItem.font
		}
	}


	Button {
		height: parent.height
		text: "Apply"
		font: apiKeyItem.font
		focusPolicy: Qt.NoFocus
		onClicked: apply()
	}
}
