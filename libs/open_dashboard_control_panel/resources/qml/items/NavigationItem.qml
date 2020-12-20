// Copyright (C) 2020 twyleg
import QtQuick 2.0

Item {
	id: navigationItem

	enum Direction {
		LEFT,
		RIGHT
	}

	property int direction
	property alias backgroundOpacity: backgroundRectangle.opacity
	signal clicked()

	Rectangle {
		id: backgroundRectangle

		width: parent.width * 0.9
		height: parent.height
		anchors.centerIn: parent
		color: "black"
		radius: 10
		visible: mouseArea.containsMouse
	}

	Image {
		id: arrowLeft

		source: direction === NavigationItem.Direction.LEFT ? "qrc:/svg-multilayer-extracted/icons_arrow_left.svg" : "qrc:/svg-multilayer-extracted/icons_arrow_right.svg"
		width: parent.width
		height: width
		fillMode: Image.PreserveAspectFit
		anchors.centerIn: parent
	}

	MouseArea {
		id: mouseArea

		hoverEnabled: true
		anchors.fill: parent
		onClicked: navigationItem.clicked()
	}
}
