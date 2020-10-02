// Copyright (C) 2020 twyleg
import QtQuick 2.0

MouseArea {

	Svg {
		source: "qrc:/svg-multilayer-extracted/fullscreen_button_background.svg"
	}

	Svg {
		id: icon
		source: "qrc:/svg-multilayer-extracted/fullscreen_button_icon.svg"
	}

	Svg {
		source: "qrc:/svg-multilayer-extracted/fullscreen_button_halo.svg"

		transformOrigin: Item.Top
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
}
