// Copyright (C) 2020 twyleg
import QtQuick 2.0
import QtMultimedia 5.15

Video {
	id: videoChannel

	autoPlay: true
	width: parent.width
	height: metaData.resolution ? width * (metaData.resolution.height / metaData.resolution.width) : 0
}
