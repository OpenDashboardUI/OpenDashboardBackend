// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

Item {
	id: headerItem

	property alias backgroundOpactity: headerItemBackground.opacity

	signal burgerMenuButtonClicked()

	Rectangle {
		id: headerItemBackground

		anchors.fill: parent
		color: "black"
	}

	Image {
		id: burgerMenuImage

		height: parent.height * 0.8
		width: height
		anchors.verticalCenter: parent.verticalCenter
		anchors.left: parent.left
		anchors.leftMargin: 10
		source: "qrc:/svg-multilayer-extracted/icons_burger_menu.svg"

		MouseArea {
			anchors.fill: parent
			onClicked: burgerMenuButtonClicked()
		}
	}

	Image {
		id: logoImage

		source: "qrc:/common_ui/images/svg/open_dashboard_logo_1.svg"

		anchors.centerIn: parent
		height: parent.height
		fillMode: Image.PreserveAspectFit

		sourceSize.width: width
		sourceSize.height: height
	}
}
