// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
	id: detailsColumnLayout

	property string filename: ""
	property int sampleRate: 0
	property string dataSource: ""
	property string trackName: ""
	property string vehicleName: ""
	property string comment: ""

	Label{
		text: "Record details:"
		font.bold: true
	}

	Label {
		text: "Filename: " + filename
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}

	Label {
		text: "Samplerate: " + sampleRate
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}

	Label {
		text: "Data source: " + dataSource
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}

	Label {
		text: "Track: " + trackName
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}

	Label {
		text: "Vehicle: " + vehicleName
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}

	Label {
		text: "Comment: " + comment
		elide: Text.ElideMiddle
		Layout.fillWidth: true
	}
}
