// here.js -- functions for here.html showing location data on a webpage
// Author: Geoffrey Jarman
// Started: 23-Aug-2023
// Log: 
//    23-Aug-2023 start by copying index.js
//    24-Aug-2023 change from API1 to API2 to work around CORS on API1
//    25-Aug-2023 add google map
//    26-Aug-2023 add google map reverse lookup with IPAddressAPI
//    26-Aug-2023 add Emma and Ross and Max Jarman to the choice list
// References:
//    https://www.youtube.com/watch?v=B4p3A00uXAs -- Youtube Google Maps
//    https://ipapi.co/api/#introduction -- ip location API
//    https://www.c-sharpcorner.com/blogs/getting-client-ip-address-or-local-ip-address-in-javascript
//    https://sunrise-sunset.org/api
  
// function to fetch the local public ip address for the client host ---------------------------------------------------

let IPLocationAPI = "https://ipapi.co/";
let IPAddressAPI = "https://maps.googleapis.com/maps/api/geocode/json?latlng=";
let SunrisetunsetAPI1 = "https://api.sunrisesunset.io/json?";                      // eg suffix lat=32.8009&lng=-96.7884
let SunriseSunsetAPI2 = "https://api.sunrise-sunset.org/json?";                    // eg suffix lat=32.8009&lng=-96.7884
let weatherAPI = "https://api.open-meteo.com/v1/forecast?";  // eg suffix latitude=34.1&longitude=-96.11&current_weather=true
let IPAddress;
let latitude;
let longitude;

// funcion to fetch the IP address -------------------------------------------------------------------------------------

function getIP(json) {
    IPAddress = json.ip;
    document.querySelector("#address-span").innerHTML += IPAddress;
}

// function to fetch location data based in IP address -----------------------------------------------------------------

async function fGetIPLatLongData() {

    IPLocationAPI = "https://ipapi.co/";
    IPLocationAPI = IPLocationAPI + IPAddress  + "\/json\/";
    const IPLocationData = await fetch(IPLocationAPI);
    const IPLocationJson = await IPLocationData.json();
    document.querySelector("#ipprovider-span").innerHTML ="IP Provider: " + IPLocationJson.org;
    document.querySelector("#iplatitude-span").innerHTML = "Latitude: " + IPLocationJson.latitude;
    document.querySelector("#iplongitude-span").innerHTML = "Longitude: " + IPLocationJson.longitude;
    latitude = IPLocationJson.latitude;
    longitude = IPLocationJson.longitude;
}

// Initialize and add the google map -----------------------------------------------------------------------------------
 
let map;

async function initMap() {

    const position = { lat: latitude, lng: longitude };                                                  // the location
    
    const { Map } = await google.maps.importLibrary("maps");
    const { AdvancedMarkerElement } = await google.maps.importLibrary("marker");

    map = new Map(document.getElementById("googlemap-div"), {                               // the map, centered on here
                  zoom: 11,
                  center: position,
                  mapId: "DEMO_MAP_ID",
    });

    const marker = new AdvancedMarkerElement({                                                             // the marker
                   map: map,
                   position: position,
                   title: "Here",
    });
}

// function to change the displayed location based on latitude and longitude -------------------------------------------

async function fChangeLocation() {

    choice = document.querySelector("#choices-select").value;

    if (choice === "susancook") {
        latitude = -37.83603;
        longitude = 145.04532;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    } else if (choice === "evanjarman") {
        latitude = -37.82657;
        longitude = 145.15515;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    } else if (choice === "maxjarman") {
        latitude = 41.43820;
        longitude = -81.99358;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    } else if (choice === "emmajarman") {
        latitude = 36.11599;
        longitude = -97.05171;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    } else if (choice === "rossjarman") {
        latitude = 39.60999;
        longitude = -105.96207;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    } else if (choice === "ipdefault") {
        document.querySelector("#address-span").innerHTML = "IP Address: " + IPAddress;
        await fGetIPLatLongData();
        await fGetLLLocationData();
    } else if (choice === "geoffreyjarman") {
        latitude = 32.85727;
        longitude = -96.76353;
        document.querySelector("#address-span").innerHTML = "IP Address: Unknown";
        document.querySelector("#ipprovider-span").innerHTML = "IP Provider: Unknown";
        await fGetLLLocationData();
    }
}

// function to fetch location data using latitude and longitude geolocation --------------------------------------------

async function fGetLLLocationData() {
    document.querySelector("#iplatitude-span").innerHTML = "Latitude: " + latitude;
    document.querySelector("#iplongitude-span").innerHTML = "Longitude: " +  longitude;
    SunriseSunsetAPI2 = "https://api.sunrise-sunset.org/json?";  
    SunriseSunsetAPI2 = SunriseSunsetAPI2 + "lat=" + latitude + "lng=" + longitude;
    sunriseSunset = await fetch(SunriseSunsetAPI2);
    SunriseSunsetJson = await sunriseSunset.json();
    document.querySelector("#ipsunrise-span").innerHTML = "Sunrise: " + SunriseSunsetJson.results.sunrise;
    document.querySelector("#ipsunset-span").innerHTML = "Sunset: " + SunriseSunsetJson.results.sunset;
    document.querySelector("#ipdaylength-span").innerHTML = "Day Length: " + SunriseSunsetJson.results.day_length;
    document.querySelector("#ipsolarnoon-span").innerHTML = "Solar Noon: " + SunriseSunsetJson.results.solar_noon;
    document.querySelector("#iptwilight-span").innerHTML = "Twilight AM & PM: " + SunriseSunsetJson.results.civil_twilight_begin + " & " + SunriseSunsetJson.results.civil_twilight_end;
    weatherAPI = "https://api.open-meteo.com/v1/forecast?";
    weatherAPI = weatherAPI + "latitude=" + latitude + "&longitude=" + longitude + "&current_weather=true";
    weather = await fetch(weatherAPI);
    weatherJson = await weather.json();
    const degreesC = weatherJson.current_weather.temperature;
    const degreesF = Math.round(degreesC * 9/5 + 32);
    document.querySelector("#iptemperature-span").innerHTML = "Temperature: " + degreesC + " C, " + degreesF + " F";
    const winddirectiondegrees = weatherJson.current_weather.winddirection;
    let winddirectionstr;
    if ((winddirectiondegrees > 22) && (winddirectiondegrees <= 77)) { winddirectionstr = "N-Easterly";}
    if ((winddirectiondegrees > 77) && (winddirectiondegrees <= 112)) { winddirectionstr = "Northerly";}
    if ((winddirectiondegrees > 122) && (winddirectiondegrees <= 167)) { winddirectionstr = "N-Westerly";}
    if ((winddirectiondegrees > 167) && (winddirectiondegrees <= 203)) { winddirectionstr = "Westerly";}
    if ((winddirectiondegrees > 203) && (winddirectiondegrees <= 248)) { winddirectionstr = "S-Westerly";}
    if ((winddirectiondegrees > 248) && (winddirectiondegrees <= 292)) { winddirectionstr = "Southerly";}
    if ((winddirectiondegrees > 292) && (winddirectiondegrees <= 338)) { winddirectionstr = "S-Easterly";}
    if ((winddirectiondegrees > 338) && (winddirectiondegrees <= 360)) { winddirectionstr = "Easterly";}
    if ((winddirectiondegrees > 0) && (winddirectiondegrees <= 22)) { winddirectionstr = "Easterly";}
    document.querySelector("#ipwinddirection-span").innerHTML = "Wind Direction: " + winddirectiondegrees + " deg , " + winddirectionstr;
    const windspeedkph = weatherJson.current_weather.windspeed;
    const windspeedmph = Math.round(windspeedkph * .621371);
    document.querySelector("#ipwindspeed-span").innerHTML = "Wind Speed: " + windspeedkph + " kph, " + windspeedmph + " mph";
    const elevationm = weatherJson.elevation;
    const elevationft = Math.round(elevationm * 3.28084);
    document.querySelector("#ipelevation-span").innerHTML = "Elevation: " + elevationm + " m, " + elevationft + " ft";
    await initMap();
    IPAddressAPI = "https://maps.googleapis.com/maps/api/geocode/json?latlng=";
    IPAddressAPI = IPAddressAPI + latitude + "," + longitude + "&key=AIzaSyC-T2eJcuVVXM4x2cKg_CdbPRU0IAYFhQQ";
    address = await fetch(IPAddressAPI);
    addressJson = await address.json();
    let streetAddress = addressJson.results[0].formatted_address;
    document.querySelector("#address-input").value = streetAddress;
    ;
    return;
}
