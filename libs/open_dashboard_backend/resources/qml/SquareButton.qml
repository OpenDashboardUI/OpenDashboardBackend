import QtQuick 2.0


Rectangle {
	id: button

	property int size: 50
	property alias source: icon.source
	property bool pressed: mouseArea.pressed

	signal clicked()

	width: size
	height: size

	color: pressed ? "#101010" : "#323232"

	radius: size * 0.1

	Svg {
		id: icon
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent
	}

	Component.onCompleted: {
		mouseArea.clicked.connect(clicked)
	}

}
