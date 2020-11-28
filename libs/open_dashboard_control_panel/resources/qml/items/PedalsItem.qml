import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {

	id: pedalsItem

	property real throttle
	property real brake
	property real clutch

	RowLayout {

		height: parent.height
		anchors.horizontalCenter: parent.horizontalCenter
		spacing: 20

		PedalProgressBar {
			id: throttleProgressBar

			text: "Throttle"
			color: "green"

			onValueChanged: throttle = value / 100.0
		}

		PedalProgressBar {
			id: brakeProgressBar

			text: "Brake"
			color: "red"

			onValueChanged: brake = value / 100.0
		}

		PedalProgressBar {
			id: clutchProgressBar

			text: "Clutch"
			color: "blue"

			onValueChanged: clutch = value / 100.0
		}
	}
}
