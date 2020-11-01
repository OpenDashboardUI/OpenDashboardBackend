// Copyright (C) 2020 twyleg
import QtQuick 2.0

Rectangle {

	id: topbar

	property bool topbarVisible: false
	property real widthRelative: 0.2
	property real heightRelative: 0.2
	property int animationDuration: 500
	property int offsetY: 0

	width: parent.width * widthRelative
	height: parent.height * heightRelative

	x: parent.width/2 - width/2
	y: -height - offsetY

	color: "#161618"
	radius: parent.height * 0.03
	border.width: 0

	PropertyAnimation {
		id: topbarAnimation
		target: topbar
		property: "offsetY"
		to: 0
		easing.type: Easing.InOutQuint
		duration: animationDuration
		onStopped: topbarVisible = !topbarVisible
	}

	Item {
		id: button

		z: -1
		anchors.top: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter

		width: parent.height * 0.4
		height: width
		rotation: 90

		Svg {
			source: "qrc:/svg-multilayer-extracted/sidebar_button_background.svg"
		}

		Svg {
			id: icon

			source: "qrc:/svg-multilayer-extracted/sidebar_button_icon.svg"

			transform: Rotation {
				origin.x: icon.width * 0.25
				origin.y: icon.height * 0.5
				angle: 0

				PropertyAnimation on angle {
					running: false
					id: buttonIconAnimation
					to: topbarVisible ? 0 : 180
					easing.type: Easing.InOutQuint
					duration: animationDuration
				}
			}
		}

		Svg {
			source: "qrc:/svg-multilayer-extracted/sidebar_button_halo.svg"

			transformOrigin: Item.Left
			SequentialAnimation on rotation {
				loops: Animation.Infinite
				PropertyAnimation {
					to: 90
					easing.type: Easing.InBounce
					duration: 1500
				}
				PropertyAnimation {
					to: -90
					easing.type: Easing.InBounce
					duration: 1500
				}
			}
		}

		MouseArea {
			anchors.fill: parent
			onClicked: topbar.toggle()
		}
	}

	function toggle(){
		if (topbarVisible){
			topbarAnimation.to = 0;
		} else {
			topbarAnimation.to = -topbar.height;
		}
		topbarAnimation.start();
		buttonIconAnimation.start();
	}
}
