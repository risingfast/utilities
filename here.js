// here.js -- functions for here.html
// Author: Geoff Jarman
// Started: 23-Aug-2023
// Log: 
//    23-Aug-2023 start by copying index.js
//    24-Aug-2023 change from API1 to API2 to work around CORS on API1
  
// function to fetch the local public ip address for the client host ---------------------------------------------------

let IPLocationAPI = "https://ipapi.co/";
let SunrisetunsetAPI1 = "https://api.sunrisesunset.io/json?";                      // eg suffix lat=32.8009&lng=-96.7884
let SunriseSunsetAPI2 = "https://api.sunrise-sunset.org/json?";                    // eg suffix lat=32.8009&lng=-96.7884
let weatherAPI = "https://api.open-meteo.com/v1/forecast?";  // eg suffix latitude=34.1&longitude=-96.11&current_weather=true
let IPAddress;
let latitude;
let longitude;


function getIP(json) {
    IPAddress = json.ip;
    document.querySelector("#ipaddress-span").innerHTML += IPAddress;
}

// function to fetch location data based in IP address -----------------------------------------------------------------

async function fGetIPLocationData() {

    IPLocationAPI = IPLocationAPI + IPAddress  + "\/json\/";
    const IPLocationData = await fetch(IPLocationAPI);
    const IPLocationJson = await IPLocationData.json();
    document.querySelector("#ipcity-span").innerHTML += IPLocationJson.city;
    document.querySelector("#ipregion-span").innerHTML += IPLocationJson.region;
    document.querySelector("#ippostal-span").innerHTML += IPLocationJson.postal;
    document.querySelector("#iptimezone-span").innerHTML += IPLocationJson.timezone;
    document.querySelector("#iputcoffset-span").innerHTML += IPLocationJson.utc_offset;
    document.querySelector("#ipcountry-span").innerHTML += IPLocationJson.country_name;
    document.querySelector("#ipcurrency-span").innerHTML += IPLocationJson.currency;
    document.querySelector("#ipprovider-span").innerHTML += IPLocationJson.org;
    document.querySelector("#iplatitude-span").innerHTML += IPLocationJson.latitude;
    document.querySelector("#iplongitude-span").innerHTML += IPLocationJson.longitude;
    latitude = IPLocationJson.latitude;
    longitude = IPLocationJson.longitude;
    SunriseSunsetAPI2 = SunriseSunsetAPI2 + "lat=" + latitude + "lng=" + longitude;
    sunriseSunset = await fetch(SunriseSunsetAPI2);
    SunriseSunsetJson = await sunriseSunset.json();
    document.querySelector("#ipsunrise-span").innerHTML += SunriseSunsetJson.results.sunrise;
    document.querySelector("#ipsunset-span").innerHTML += SunriseSunsetJson.results.sunset;
    document.querySelector("#ipdaylength-span").innerHTML += SunriseSunsetJson.results.day_length;
    document.querySelector("#ipsolarnoon-span").innerHTML += SunriseSunsetJson.results.solar_noon;
    document.querySelector("#iptwilight-span").innerHTML += SunriseSunsetJson.results.civil_twilight_begin + " - " + SunriseSunsetJson.results.civil_twilight_end;
    weatherAPI = weatherAPI + "latitude=" + latitude + "&longitude=" + longitude + "&current_weather=true";
    weather = await fetch(weatherAPI);
    weatherJson = await weather.json();
    const degreesC = weatherJson.current_weather.temperature;
    const degreesF = Math.round(degreesC * 9/5 + 32);
    document.querySelector("#iptemperature-span").innerHTML += degreesC + " C, " + degreesF + " F";
    const winddirectiondegrees = weatherJson.current_weather.winddirection;
    let winddirectionstr;
    if ((winddirectiondegrees > 22) && (winddirectiondegrees <= 77)) { winddirectionstr = "N-Easterly";}
    if ((winddirectiondegrees > 77) && (winddirectiondegrees <= 112)) { winddirectionstr = "Northerly";}
    if ((winddirectiondegrees > 122) && (winddirectiondegrees <= 167)) { winddirectionstr = "N-Westerly";}
    if ((winddirectiondegrees > 167) && (winddirectiondegrees <= 203)) { winddirectionstr = "Westerly";}
    if ((winddirectiondegrees > 203) && (winddirectiondegrees <= 248)) { winddirectionstr = "S-Westerly";}
    if ((winddirectiondegrees > 248) && (winddirectiondegrees <= 292)) { winddirectionstr = "Southerly";}
    if ((winddirectiondegrees > 292) && (winddirectiondegrees <= 338)) { winddirectionstr = "S-Easterly";}
    if ((winddirectiondegrees > 338) && (winddirectiondegrees <= 22)) { winddirectionstr = "Easterly";}
    document.querySelector("#ipwinddirection-span").innerHTML += winddirectiondegrees + " deg , " + winddirectionstr;
    const windspeedkph = weatherJson.current_weather.windspeed;
    const windspeedmph = Math.round(windspeedkph * .621371);
    document.querySelector("#ipwindspeed-span").innerHTML += windspeedkph + " kph, " + windspeedmph + " mph";
    const elevationm = weatherJson.elevation;
    const elevationft = Math.round(elevationm * 3.28084);
    document.querySelector("#ipelevation-span").innerHTML += elevationm + " m, " + elevationft + " ft";
    ;
    return;

}
