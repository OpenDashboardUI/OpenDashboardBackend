// Copyright (C) 2020 twyleg
import QtQuick 2.0

import "./RightSideBarItems"

ControlBar {
	id: rightSideBar

	layout: ControlBar.Layout.RIGHT
	widthRelative: 0.15
	heightRelative: 1.00

	enabled: !controlDataStaticModel.sidebarsDisabled
	visible: !controlDataStaticModel.sidebarsDisabled

	Column {

		width: parent.width * 0.9
		height: parent.height * 0.9
		anchors.centerIn: parent
		spacing: 20

		property int currentState: controlDataDynamicModel.state

		onCurrentStateChanged: {

			switch (currentState)
			{
			case 0:
			case 2:
				videoChannel0.seek(0);
				videoChannel0.stop();
				videoChannel1.seek(0);
				videoChannel1.stop();
				videoChannel2.seek(0);
				videoChannel2.stop();
				break;
			case 1:
				videoChannel0.play();
				videoChannel1.play();
				videoChannel2.play();
				break;
			}
		}


		VideoChannel {
			id: videoChannel0

			autoPlay: false
			width: parent.width
			source: controlDataStaticModel.videoChannelOnePath
		}

		VideoChannel {
			id: videoChannel1

			autoPlay: false
			width: parent.width
			source: controlDataStaticModel.videoChannelTwoPath
		}

		VideoChannel {
			id: videoChannel2

			autoPlay: false
			width: parent.width
			source: controlDataStaticModel.videoChannelThreePath
		}

		Loader {
			id: mapViewLoader

			onLoaded: {
				item.width = Qt.binding(function() { return rightSideBar.width });
				item.height = Qt.binding(function() { return rightSideBar.width });
			}

			Component.onCompleted: {
				if (controlDataStaticModel.webViewAvailable) {
					source = "qrc:/qml/ControlBars/RightSideBarItems/MapView.qml"
				}
			}
		}
	}
}
