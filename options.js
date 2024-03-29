// options.js -- script to implement help in options.html
// Author: Geoff Jarman
// Started: 16-Jun-2022
// Log: 
//    16-Jun-2022 start and test
//    18-Jun-2022 move fSetCornerImage() to common.js
//    18-Jun-2022 add fSetCornerImageChoice()
//    23-Jun-2022 rename fSetCornerImageChoice() to fSetCornerImageBehaviourButton()
//    29-Jun-2022 expand fSetShowAccessLog()
//    01-Jul-2022 add fSetShowAccessLog() to fSetOptions()
//    24-Jul-2022 implement fClearOptionsFields() {

// define uti's for cgi database fetches and updates

const uri01 = "http://gjarman2020.com/cgi-bin/utilitiesUpdateOptions.cgi";
const uri02 = "http://gjarman2020.com/cgi-bin/utilitiesFetchOptions.cgi";
const uri03 = "http://gjarman2020.com/cgi-bin/utilitiesFetchAccessLog.cgi";
const uri04 = "http://gjarman2020.com/cgi-bin/utilitiesFetchSessionLog.cgi";

// const uri03 = "http://gjarman2020.com/cgi-bin/showText.cgi";

function fSetCornerImageBehaviourButton() {
    let cic = document.getElementById("cornerimagechoice-select");
    if (cic.value == "Locked") {
        document.getElementById("cornerimagepolicyrandom-button").disabled = true;
        document.getElementById("cornerimagepolicysequenced-button").disabled = true;
        document.getElementById("cornerimagepolicydecremented-button").disabled = true;
        document.getElementById("cornerimagepolicyfirst-button").disabled = true;
    }
    else if (cic.value == "Randomized") {
        document.getElementById("cornerimagepolicyrandom-button").disabled = false;
        document.getElementById("cornerimagepolicysequenced-button").disabled = true;
        document.getElementById("cornerimagepolicydecremented-button").disabled = true;
        document.getElementById("cornerimagepolicyfirst-button").disabled = true;
    } else if (cic.value == "Sequenced") {
        document.getElementById("cornerimagepolicyrandom-button").disabled = true;
        document.getElementById("cornerimagepolicysequenced-button").disabled = false;
        document.getElementById("cornerimagepolicydecremented-button").disabled = false;
        document.getElementById("cornerimagepolicyfirst-button").disabled = false;
    }
}

// function to ajax fetch the current option settings and set them on the options page

async function fSetOptions() {
    let response = await fetch(uri02);
    if (response.ok) {
        var options= [];
        let text = await response.text();
        let array = text.split("\n");
        array.pop();                      // remove the last element (empty element) created by the split("\n")
        for (let i = 0; i < array.length; i++)
        {
            options[i] = array[i].split(",");
        }
        let x = options.length;
        let y = options[1].length;
        for (let i = 0; i < x; i++) {
            for (let j = 0; j < y; j++) {
                options[i][j] = options[i][j].trim();
            }
        }
        options[4][2] = parseInt(options[4][2], 10);
    } else {
        alert("HttpError: " + response.status);
    }
    document.getElementById("cornerimagechoice-select").value = options[0][2];
    document.getElementById("backgroundimagechoice-select").value = options[1][2];
    document.getElementById("showserverlog-select").value = options[2][2];
    document.getElementById("showsessionlog-select").value = options[3][2];
    document.getElementById("showleftlinks-select").value = options[5][2];
    fcSetFooterOptions();
    fSetCornerImageBehaviourButton();
    fSetShowServerLog();                    // show the access log if the user saved this setting as 'Yes'
    fSetShowSessionLog();                   // show the session log if the user saved this setting as 'Yes'
    return;
}

// function to save web options when submitted from options.html

async function fSaveOptions() {

    let sBehaviour = encodeURIComponent(document.getElementById("cornerimagechoice-select").value);
    let sBackground = encodeURIComponent(document.getElementById("backgroundimagechoice-select").value);
    let sShowSession = encodeURIComponent(document.getElementById("showsessionlog-select").value);
    let sShowServerLog = encodeURIComponent(document.getElementById("showserverlog-select").value);
    let sLeftLinks = encodeURIComponent(document.getElementById("showleftlinks-select").value);

    let sRequest = uri01 + '?' + 'behaviour=' + sBehaviour +
                            '&' + 'background=' + sBackground +
                            '&' + 'showuser=' + sShowSession +
                            '&' + 'showlog=' + sShowServerLog +
                            '&' + 'leftlinks=' + sLeftLinks;
    let response = await fetch(sRequest);
    if (response.ok) {
        ;
    } else {
        alert("HttpError: " + response.status);
    }

    document.getElementById("footeroptionscichoice").innerHTML = decodeURIComponent(sBehaviour);
    document.getElementById("footeroptionsbackgroundimage").innerHTML = decodeURIComponent(sBackground);
    document.getElementById("footeroptionsshowserverlog").innerHTML = decodeURIComponent(sShowServerLog);
    document.getElementById("footeroptionsshowsession").innerHTML = decodeURIComponent(sShowSession);
    document.getElementById("footeroptionsleftlinks").innerHTML = decodeURIComponent(sLeftLinks);
}

async function fSetBackgroundImageChoice() {
    let sBackgroundImageChoice = document.getElementById("backgroundimagechoice-select").value + ".jpg";
        document.getElementById("MAIN1").style.backgroundImage = "url(" + sBackgroundImageChoice + ")";
    }

function fSetLeftLinks() {
    const shortLeftLinks = document.querySelector('#NAV1UL');
    const longLeftLinks = document.querySelector('NAV1UL2');
    if (document.getElementById("showleftlinks-select").value === "Short") {
        document.getElementById("NAV1UL").style.display = "block";
        document.getElementById("NAV1UL2").style.display = "none";
    } else {
        document.getElementById("NAV1UL").style.display = "none";
        document.getElementById("NAV1UL2").style.display = "block";
    }
}

async function fSetShowUserChoice() {
}

// function to ajax consolidate the Apache2 access logs and fetch text from the server and paste it to a textarea

async function fSetShowServerLog() {
    if (document.getElementById("showserverlog-select").value === "Yes") {
        document.getElementById("serverlog-div").style.display = "block";
        let response = await fetch(uri03);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("serverlog-textarea").value=text;
        } else {
            alert("HttpError: " + response.status);
        }
    } else {
        document.getElementById("serverlog-div").style.display = "none";
    }
}

// function to show the session log from mySQL sessions table

async function fSetShowSessionLog() {
    if (document.getElementById("showsessionlog-select").value === "Yes") {
        document.getElementById("sessionlog-div").style.display = "block";
        let response = await fetch(uri04);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("sessionlog-textarea").value=text;
        } else {
            alert("HttpError: " + response.status);
        }
    } else {
        document.getElementById("sessionlog-div").style.display = "none";
    }
}

// function for button to clear page .................................................................................

function fClearOptionsFields() {
    var x = document.getElementById("HELPDIV");
    x.style.display = "";

    var ala = document.getElementById("serverlog-textarea");
    ala.value = "";

    var sla = document.getElementById("sessionlog-textarea");
    sla.value = "";

    var ald = document.getElementById("serverlog-div");
    ald.style.display = "none";

    var sld = document.getElementById("sessionlog-div");
    sld.style.display = "none";

    document.getElementById("showserverlog-select").value = "No";
    document.getElementById("showsessionlog-select").value = "No";

    fcClearExtras();
}

