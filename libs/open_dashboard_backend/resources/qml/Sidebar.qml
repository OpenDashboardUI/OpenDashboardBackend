// Copyright (C) 2020 twyleg
import QtQuick 2.0

Rectangle {

	id: sidebar

	enum Layout {
		LEFT,
		RIGHT
	}

	property bool sidebarVisible: false
	property real widthRelative: 0.2
	property int animationDuration: 500

	property int layout: Sidebar.Layout.LEFT

	width: parent.width * widthRelative
	height: parent.height

	x: layout === Sidebar.Layout.LEFT ? -width: parent.width
	y: 0

	color: "#161618"
	radius: parent.height * 0.03
	border.width: 0

	PropertyAnimation {
		id: sidebarAnimation
		target: sidebar
		property: "x"
		to: 0
		easing.type: Easing.InOutQuint
		duration: animationDuration
		onStopped: sidebarVisible = !sidebarVisible
	}

	Item {
		id: button

		z: -1
		anchors.left: layout === Sidebar.Layout.LEFT ? parent.right : undefined
		anchors.right: layout === Sidebar.Layout.LEFT ?  undefined : parent.left
		anchors.verticalCenter: parent.verticalCenter

		width: parent.height * 0.05
		height: width
		rotation: layout === Sidebar.Layout.LEFT ? 0 : 180

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
					to: sidebarVisible ? 0 : 180
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

			onClicked: {

				if (layout === Sidebar.Layout.LEFT) {
					if (sidebarVisible){
						sidebarAnimation.to = -sidebar.width;
					} else {
						sidebarAnimation.to = 0;
					}
				} else {
					if (sidebarVisible){
						sidebarAnimation.to = sidebar.x + sidebar.width;
					} else {
						sidebarAnimation.to = sidebar.x - sidebar.width;
					}
				}

				sidebarAnimation.start();
				buttonIconAnimation.start();
			}
		}
	}
}
