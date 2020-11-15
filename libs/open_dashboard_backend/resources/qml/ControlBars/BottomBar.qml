// Copyright (C) 2020 twyleg
import QtQuick 2.0

import "./BottomBarItems"

ControlBar {
	id: bottombar

	layout: ControlBar.Layout.BOTTOM

	widthRelative: 0.5
	heightRelative: 0.1

	LogItem {

		width: parent.width - parent.radius
		height: parent.height - parent.radius

		anchors.centerIn: parent
	}
}
