var backend;

window.onload = function()
{
	new QWebChannel(qt.webChannelTransport, function(channel) {
		backend = channel.objects.backend;
		backend.updatePosition.connect(function(lon, lat) {
			center(lon, lat);
		});
	});
}
