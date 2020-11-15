// Copyright (C) 2020 twyleg
import QtQuick 2.0

import QtCharts 2.4

import "./LeftSideBarItems"

ControlBar {
	id: leftSideBar

	layout: ControlBar.Layout.LEFT
	widthRelative: 0.09
	heightRelative: 1.00

	enabled: !controlDataStaticModel.sidebarsDisabled
	visible: !controlDataStaticModel.sidebarsDisabled

	Item {

		width: parent.width
		height: parent.height - 2*parent.radius
		anchors.centerIn: parent

		FpsItem {
			id: fpsItem

			property int time: 0

			onUpdated: {
				series1.append(time, fps)
				axisX.min = time - 7.5
				axisX.max = time + 2.5
				time++
			}
		}

		Connections {
			target: controlDataDynamicModel

			function onSerializedQuantitiesChanged() {
				var diff = controlDataDynamicModel.serializedQuantities.length - column.children.length
				if (diff) {
					for (var i=0; i<diff; ++i) {
						column.addLine()
					}
				}
				for (i=0; i<controlDataDynamicModel.serializedQuantities.length; ++i) {
					column.children[i].text = controlDataDynamicModel.serializedQuantities[i]
				}
			}
		}

		Column {
			id: column

			anchors.fill: parent

			function addLine() {
				var newLine = Qt.createQmlObject('import QtQuick 2.0; Text {}', column);
				newLine.width = Qt.binding(function() { return column.width })
				newLine.color = "white"
				newLine.elide = Text.ElideRight
			}
		}

		ChartView {
			title: "FPS: " + fpsItem.fps
			legend.visible: false
			antialiasing: true
			backgroundColor: leftSideBar.color
			titleColor: "white"

			anchors.bottom: parent.bottom

			width: parent.width
			height: 200

			margins.top: 0
			margins.bottom: 0
			margins.left: 0
			margins.right: 0

			ValueAxis {
				id: axisX
				min: 0
				max: 10

				labelsVisible: false
				labelsColor: "white"
			}

			ValueAxis {
				id: axisY
				min: 0
				max: 65

				labelsColor: "white"

				tickCount: 3
				tickInterval: 30
				tickAnchor: 30
			}

			LineSeries {
				id: series1
				axisX: axisX
				axisY: axisY
				color: "white"
				width: 4
			}
		}
	}
}
