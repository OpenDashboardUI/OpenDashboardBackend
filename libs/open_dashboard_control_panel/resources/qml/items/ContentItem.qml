// Copyright (C) 2020 twyleg
import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

import QtQml.Models 2.2

Item {
	id: contentItem

	property alias items: stackView.items
	property alias backgroundOpactity: backgroundRectangle.opacity
	property int activeViewIndex

	clip: true

	Rectangle {
		id: backgroundRectangle

		anchors.fill: parent
		color: "black"
		radius: 10
	}

	StackView {
		id: stackView

		property variant items: []

		focus: true

		anchors.fill: parent
		anchors.leftMargin: parent.width * 0.01
		anchors.rightMargin: parent.width * 0.01
		anchors.topMargin: parent.width * 0.01
		anchors.bottomMargin: parent.width * 0.01

		replaceEnter: Transition {
			XAnimator {
				id: enterAnimator

				from: stackView.width
				to: 0
				duration: 400
				easing.type: Easing.OutCubic
			}
		}

		replaceExit: Transition {
			XAnimator {
				id: exitAnimator

				from: 0
				to: -stackView.width
				duration: 400
				easing.type: Easing.OutCubic
			}
		}
	}

	function changeView(viewIndex) {
		if (viewIndex !== activeViewIndex && viewIndex < stackView.items.length) {
			stackView.replace(stackView.items[viewIndex])
			activeViewIndex = viewIndex
		}
	}

	function moveRight() {
		enterAnimator.from = stackView.width
		enterAnimator.to = 0

		exitAnimator.from = 0
		exitAnimator.to = -stackView.width

		var requestedViewIndex = activeViewIndex+1
		if (requestedViewIndex >= stackView.items.length) {
			requestedViewIndex = 0
		}
		changeView(requestedViewIndex)
	}

	function moveLeft() {
		enterAnimator.from = -stackView.width
		enterAnimator.to = 0

		exitAnimator.from = 0
		exitAnimator.to = stackView.width

		var requestedViewIndex = activeViewIndex-1
		if (requestedViewIndex < 0) {
			requestedViewIndex = stackView.items.length-1
		}
		changeView(requestedViewIndex)
	}

	function initView(viewIndex) {
		stackView.push(stackView.items[viewIndex])
		activeViewIndex = viewIndex
	}
}
