// Copyright (C) 2020 twyleg
import QtQuick 2.0

Image {

	id: logoStartscreen

	width: 500
	height: 500

	fillMode: Image.PreserveAspectFit
	sourceSize: Qt.size(width, height)
	smooth: true

	source: "qrc:/common_ui/images/svg/open_dashboard_logo_0.svg"

	signal clicked()

	MouseArea {
		anchors.fill: parent

		onClicked: logoStartscreen.clicked()

	}
}
