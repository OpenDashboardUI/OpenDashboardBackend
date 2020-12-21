// Copyright (C) 2020 twyleg
import QtQuick 2.0

Rectangle {
	id: logoStartscreen

	width: 500
	height: 500
	color: "black"

	signal clicked()

	Svg {
		id: logoStartscreenImage

		source: "qrc:/common_ui/images/svg/open_dashboard_logo_0.svg"
		MouseArea {
			anchors.fill: parent
			onClicked: logoStartscreen.clicked()
		}
	}
}
