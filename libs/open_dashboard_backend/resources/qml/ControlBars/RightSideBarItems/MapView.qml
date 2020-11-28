// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtWebEngine 1.1
import QtWebChannel 1.0

Item {
	id: mapView

	focus: false

	property real lat: dataModel.vehicleData.gps.positionLat// * (180.0 / 3.141591653)
	property real lon: dataModel.vehicleData.gps.positionLon// * (180.0 / 3.141591653)

	Timer {
		id: timer

		interval: 100
		running: true
		repeat: true
		onTriggered: {
			backendConnection.updatePosition(mapView.lon, mapView.lat)
		}
	}

	QtObject {
		id: backendConnection

		WebChannel.id: "backend"
		signal updatePosition(real lon, real lat);
	}

	WebChannel {
		id: channel
		registeredObjects: [backendConnection]
	}

	WebEngineView {
		id: webview

		// Necessary because webview doesn't release key focus
		enabled: false

		anchors.fill: parent
		url: "qrc:/maps/map.html"
		webChannel: channel
	}
}
