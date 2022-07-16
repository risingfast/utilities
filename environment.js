// Comments region
//
// environment.js -  javascript methods for environment.html
// References:
//      https://www.youtube.com/watch?v=qTGbWfEEnKI
//          Easily Structure your Layout with CSS Grid's 'grid-template-areas'
// Templates:
//      none
// Log:
//    22-Sep-2021 - start
//    22-Sep-2021 - change fetch() to include query string for get request
//    09-Oct-2021 - prefix function names with 'f'
//    16-Oct-2021 - add logic for 'Choose' option and clear page
//    11-May-2022 - capitalize helpDiv
//    12-May-2022 - change "none" to "" on x.style.display
//    17-May-2022 - expand Clear function()
//    01-Jun-2022 - add cornerimage rotation()
//    18-Jun-2022 - move fSetCornerImage() to common.js()
//

// globals

const uri1 = "http://www.risingfast.com/cgi-bin/environment.cgi";

// function to set document model values to match action chosen by the user ............................................

function fSetAction() {
    var sChoice = document.getElementById("action").value;

    var s = document.getElementById("submitButton");
    var r = document.getElementById("results-div");
    if (sChoice == "Choose") {
        var s = document.getElementById("submitButton");
        s.disabled = true;
        r.style.display = "";
    } else {
        s.disabled = false;
        r.style.display = "";
    }
}

// function for button to clear page .................................................................................

function fClearEnvironmentPage() {
    var x = document.getElementById("HELPDIV");
    x.style.display = "";

    var a = document.getElementById("action");
    a.value = "Choose";

    var s = document.getElementById("submitButton");
    s.disabled = true;

    var r = document.getElementById("results-area");
    r.value = "";

    fcClearExtras();
}

// function to ajax fetch the environment from the server

async function fGetResults() {
    
    var r = document.getElementById("results-div");
    r.style.display = "block";
    
    let response = await fetch(uri1 + '?' + document.getElementById("action").value);
    if (response.ok) {
        let text = await response.text();
        console.log(text.slice(0, 80));
        document.getElementById("results-area").style.display="inline-block";
        document.getElementById("results-area").value=text;
    } else {
        alert("HttpError: " + response.status);
    }
}

