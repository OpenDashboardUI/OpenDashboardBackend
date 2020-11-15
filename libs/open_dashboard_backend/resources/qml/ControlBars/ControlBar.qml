// Copyright (C) 2020 twyleg
import QtQuick 2.0

import "../"

Rectangle {

	id: controlBar

	enum Layout {
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	}

	property bool controlBarVisible: false
	property real widthRelative: 0.2
	property real heightRelative: 0.2
	property int animationDuration: 500
	property real extendedPercent: 0
	property alias resizeable: resizeMouseArea.enabled
	property int layout

	width: parent.width * widthRelative
	height: parent.height * heightRelative

	color: "#161618"
	radius: parent.height * 0.03
	border.color: "white"
	border.width: resizeMouseArea.focused ? 1 : 0

	Component.onCompleted: {

		if (layout === ControlBar.Layout.LEFT) {
			x = Qt.binding(function() { return width * (extendedPercent - 1.00)})
			anchors.verticalCenter = parent.verticalCenter
		} else if(layout === ControlBar.Layout.RIGHT) {
			x = Qt.binding(function() { return parent.width - width * extendedPercent})
			anchors.verticalCenter = parent.verticalCenter
		} else if(layout === ControlBar.Layout.TOP) {
			y = Qt.binding(function() { return height * ( extendedPercent - 1.00)})
			anchors.horizontalCenter = parent.horizontalCenter
		} else {
			y = Qt.binding(function() { return parent.height - height * extendedPercent})
			anchors.horizontalCenter = parent.horizontalCenter
		}
	}

	PropertyAnimation {
		id: sidebarAnimation
		target: controlBar
		property: "extendedPercent"
		to: 0
		easing.type: Easing.InOutQuint
		duration: animationDuration
		onStopped: controlBarVisible = !controlBarVisible
	}

	MouseArea {
		id: resizeMouseArea

		property bool focused: pressed || containsMouse

		hoverEnabled: enabled

		Component.onCompleted: {
			if (controlBar.layout === ControlBar.Layout.LEFT) {
				anchors.right = controlBar.right
				cursorShape = Qt.SizeHorCursor
				height = Qt.binding(function(){ return parent.height })
				width = 10
			} else if (controlBar.layout === ControlBar.Layout.RIGHT) {
				anchors.left = controlBar.left
				cursorShape = Qt.SizeHorCursor
				height = Qt.binding(function(){ return parent.height })
				width = 10
			} else if (controlBar.layout === ControlBar.Layout.TOP) {
				anchors.bottom = controlBar.bottom
				cursorShape = Qt.SizeVerCursor
				width = Qt.binding(function(){ return parent.width })
				height = 10
			} else {
				anchors.top = controlBar.top
				cursorShape = Qt.SizeVerCursor
				width = Qt.binding(function(){ return parent.width })
				height = 10
			}

			// Reset cursor shape if disabled
			if (!enabled) {
				cursorShape = Qt.ArrowCursor
			}
		}

		onPositionChanged: {
			if (resizeMouseArea.pressed) {
				if (controlBar.layout === ControlBar.Layout.LEFT) {
					controlBar.widthRelative += mouseX / controlBar.parent.width
				} else if (controlBar.layout === ControlBar.Layout.RIGHT){
					controlBar.widthRelative += -mouseX / controlBar.parent.width
				} else if (controlBar.layout === ControlBar.Layout.TOP) {
					controlBar.heightRelative += mouseY / controlBar.parent.height
				} else {
					controlBar.heightRelative += -mouseY / controlBar.parent.height
				}
			}
		}
	}

	Item {
		id: button

		z: -1

		Component.onCompleted: {
			if (layout === ControlBar.Layout.LEFT) {
				anchors.left = parent.right
				anchors.verticalCenter = parent.verticalCenter

				width = Qt.binding(function() { return parent.height * 0.05 })
				height = Qt.binding(function() { return width })

				rotation = 0
			} else if (layout === ControlBar.Layout.RIGHT) {
				anchors.right = parent.left
				anchors.verticalCenter = parent.verticalCenter

				width = Qt.binding(function() { return parent.height * 0.05 })
				height = Qt.binding(function() { return width })

				rotation = 180
			} else if (layout === ControlBar.Layout.TOP) {
				anchors.top = parent.bottom
				anchors.horizontalCenter = parent.horizontalCenter

				height = Qt.binding(function() { return parent.width * 0.05 })
				width = Qt.binding(function() { return height })

				rotation = 90
			} else {
				anchors.bottom = parent.top
				anchors.horizontalCenter = parent.horizontalCenter

				height = Qt.binding(function() { return parent.width * 0.05 })
				width = Qt.binding(function() { return height })

				rotation = 270
			}
		}

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
					to: controlBarVisible ? 0 : 180
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
			onClicked: controlBar.toggle()
		}
	}

	function toggle(){
		if (controlBarVisible){
			sidebarAnimation.to = 0.0;
		} else {
			sidebarAnimation.to = 1.0;
		}
		sidebarAnimation.start();
		buttonIconAnimation.start();
	}
}
