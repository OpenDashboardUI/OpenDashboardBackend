// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12


ColumnLayout {

	id: playerSlider

	property int currentFrame: 0
	property int totalFrames: 0

	property double currentTimestamp: 0
	property int endTimestamp: 0

	signal sliderGrabbed()
	signal frameRequest(int requestedFrame)

	Item {
		Layout.fillWidth: true
		height: Screen.pixelDensity * 5

		Label {
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			text: "0"
		}

		Label {
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.bottom
			text: currentFrame
		}

		Label {
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			text: totalFrames
		}
	}

	Slider {
		id: slider

		Layout.fillWidth: true
		from: 0
		to: totalFrames-1
		stepSize: 1
		value: currentFrame
		live: true

		onPressedChanged: {
			if(pressed) {
				sliderGrabbed()
			} else {
				frameRequest(value)
			}
		}

		onValueChanged: {
			if(pressed)
				timer.start()
		}

		Timer {
			id: timer
			interval: 100
			running: false
			repeat: false
			onTriggered: frameRequest(slider.value)
		}
	}

	Item {
		Layout.fillWidth: true
		height: Screen.pixelDensity * 5

		Label {
			anchors.left: parent.left
			anchors.top: parent.top
			text: "0"
		}

		Label {
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.top: parent.top
			text: currentTimestamp
		}

		Label {
			anchors.right: parent.right
			anchors.top: parent.top
			text: endTimestamp
		}
	}

}
