import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ProgressBar {
	id: pedalPogressBar

	property string text
	property color color

	height: parent.height

	orientation: Qt.Vertical

	minimumValue: 0
	maximumValue: 100

	value: 50

	style: ProgressBarStyle {
		background: Rectangle {
			radius: 2
			color: "transparent"
			border.color: "white"
			border.width: 1
			implicitWidth: 200
			implicitHeight: 35
		}
		progress: Rectangle {
			color: pedalPogressBar.color
			border.color: "white"
		}
	}

	Text {
		text: parent.text + ": " + parent.value.toPrecision(2) + "%"
		rotation: -90
		anchors.centerIn: parent
		color: "white"
		font.pixelSize: parent.width/2
	}

	MouseArea {
		anchors.fill: parent
		onPositionChanged: {
			if (pressed) {
				pedalPogressBar.value = (1 - (mouseY / height)) * 100
				pedalPogressBar.focus = true
			}
		}
	}

	Keys.onUpPressed: pedalPogressBar.value++
	Keys.onDownPressed: pedalPogressBar.value--



}
