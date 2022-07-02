// options.js -- script to implement help in options.html
// Author: Geoff Jarman
// Started: 16-Jun-2022
// Log: 
//    16-Jun-2022 start and test
//    18-Jun-2022 move fSetCornerImage() to common.js
//    18-Jun-2022 add fSetCornerImageChoice()
//    23-Jun-2022 rename fSetCornerImageChoice() to fSetCornerImageBehaviourButton()
//    29-Jun-2022 expand fSetShowAccessLog()

// define uti's for cgi database fetches and updates

const uri01 = "http://www.risingfast.com/cgi-bin/utilitiesUpdateOptions.cgi";
const uri02 = "http://www.risingfast.com/cgi-bin/utilitiesFetchOptions.cgi";
const uri03 = "http://www.risingfast.com/cgi-bin/utilitiesFetchAccessLog.cgi";
// const uri03 = "http://www.risingfast.com/cgi-bin/showText.cgi";

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
    document.getElementById("showuser-select").value = options[2][2];
    document.getElementById("showaccesslog-select").value = options[3][2];
    fcSetFooterOptions();
    fSetCornerImageBehaviourButton();
    return;
}

// function to save web options when submitted from options.html

async function fSaveOptions() {

    let sBehaviour = encodeURIComponent(document.getElementById("cornerimagechoice-select").value);
    let sBackground = encodeURIComponent(document.getElementById("backgroundimagechoice-select").value);
    let sShowUser = encodeURIComponent(document.getElementById("showuser-select").value);
    let sShowLog = encodeURIComponent(document.getElementById("showaccesslog-select").value);

    let sRequest = uri01 + '?' + 'behaviour=' + sBehaviour +
                            '&' + 'background=' + sBackground +
                            '&' + 'showuser=' + sShowUser +
                            '&' + 'showlog=' + sShowLog;
    let response = await fetch(sRequest);
    if (response.ok) {
        ;
    } else {
        alert("HttpError: " + response.status);
    }

    document.getElementById("footeroptionscichoice").innerHTML = decodeURIComponent(sBehaviour);
    document.getElementById("footeroptionsbackgroundimage").innerHTML = decodeURIComponent(sBackground);
    document.getElementById("footeroptionsshowuser").innerHTML = decodeURIComponent(sShowUser);
    document.getElementById("footeroptionsshowlog").innerHTML = decodeURIComponent(sShowLog);
}

async function fSetBackgroundImageChoice() {
    let sBackgroundImageChoice = document.getElementById("backgroundimagechoice-select").value + ".jpg";
        document.getElementById("MAIN1").style.backgroundImage = "url(" + sBackgroundImageChoice + ")";
    }

async function fSetShowUserChoice() {
}

// function to ajax consolidate the Apache2 access logs and fetch text from the server and paste it to a textarea

async function fSetShowAccessLog() {
    if (document.getElementById("showaccesslog-select").value === "Yes") {
        document.getElementById("accesslog-textarea").style.display = "block";
        let response = await fetch(uri03);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("accesslog-textarea").value=text;
        } else {
            alert("HttpError: " + response.status);
        }
    } else {
        document.getElementById("accesslog-textarea").style.display = "none";
    }
}
