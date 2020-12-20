// Copyright (C) 2020 twyleg
import QtQuick 2.0

Image {
	id: backgroundImage

	source: "qrc:/control_panel/images/external/background_1.jpg"

	fillMode: Image.PreserveAspectFit

	anchors.bottom: parent.bottom
	anchors.right: parent.right

	Component.onCompleted: adjust()

	Connections {
		target: parent
		function onWidthChanged() { adjust() }
		function onHeightChanged() { adjust() }
	}

	function adjust() {

		var windowRatio = parent.width / parent.height
		var backgroundRatio = sourceSize.width / sourceSize.height

		if (windowRatio > backgroundRatio) {
			width = parent.width
			height = parent.width / backgroundRatio
		} else {
			width = parent.height * backgroundRatio
			height = parent.height
		}
	}
}
