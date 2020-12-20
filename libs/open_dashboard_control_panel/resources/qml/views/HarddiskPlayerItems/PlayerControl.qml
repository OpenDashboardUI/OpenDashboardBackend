// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

RowLayout {
	width: parent.width
	height: Screen.pixelDensity * 5

	property bool playing: false

	signal playPauseClicked()
	signal forewardClicked()
	signal backwardClicked()
	signal fastForewardClicked()
	signal fastBackwardClicked()

	Button {
		Layout.fillWidth: true
		text: "<<"
		onClicked: fastBackwardClicked()
	}

	Button {
		Layout.fillWidth: true
		text: "<"
		onClicked: harddiskPlayerController.seek(-1);
	}

	Button {
		Layout.fillWidth: true
		text: playing ? "Pause" : "Play"
		onClicked: playPauseClicked()
	}

	Button {
		Layout.fillWidth: true
		text: ">"
		onClicked: harddiskPlayerController.seek(1);
	}

	Button {
		Layout.fillWidth: true
		text: ">>"
		onClicked: harddiskPlayerController.seek(100);
	}
}
