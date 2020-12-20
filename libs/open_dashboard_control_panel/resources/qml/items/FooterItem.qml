// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

Item {
	id: footerItem

	property alias backgroundOpactity: footerItemBackground.opacity

	Rectangle {
		id: footerItemBackground
		anchors.fill: parent
		color: "black"
	}
}
