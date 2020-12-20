var map;
var marker;

function center(lon, lat) {
	map.panTo(new google.maps.LatLng(lat, lon));
	if (marker === undefined) {
		marker = new google.maps.Marker({
			map: map
		});
	}
	marker.setPosition({lng: lon, lat: lat});
}

function initMap() {
	map = new google.maps.Map(document.getElementById('map'), {
		center: {lat: 50.338346, lng: 6.9492118},
		zoom: 15,
		mapTypeId: 'satellite',
		disableDefaultUI: true
	});
}

