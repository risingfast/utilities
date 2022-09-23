// autheticateUser.js -- script to implement CG call in authenticateUser.html
// Author: Geoff Jarman
// Started: 24-Sep-2021
// Log: 
//    24-Sep 2021 start and test
//    24-Sep 2021 add fAuthenticateUser() and fClear()
//    25-Sep-2021 add idResult field to fClear()
//    25-Sep-2021 set focus() on username in fClear()
//    16-Oct-2021 add to fClearHTML to clear help()
//    11-May-2022 change helpDiv to uppercase
//    12-May-2022 change "none" to "" on x.style.display
//    15-May-2022 remove function showHelp() moved to common.js
//    01-Jun-2022 add cornerimage rotation
//    18-Jun-2022 move fSetCornerImage() to common.js
//    16-Sep-2022 change www.risingfast.com to gjarman.com

const uri1 = "http://gjarman2020.com/cgi-bin/authenticateUser.cgi";

// functions for action buttons to display and hide help ..........................................
  
function resetShows() {
    var x = document.getElementById("HELPDIV");
    x.style.display = "";
}

async function fAuthenticateUser() {
    
    var sUsername = document.getElementById("idUsername").value;
    var sPassword = document.getElementById("idPassword").value;

    fClearAuthenticateUserPage();
    document.getElementById("ResultArea").disabled = false;

    const uri2 = uri1 + '?Username=' + sUsername + '&Password=' + sPassword;
    let response = await fetch(uri2);

    if (response.ok) {
        let text = await response.text();
        document.getElementById("ResultArea").value= text;
        fcCheckAuthentication();
    } else {
        document.getElementById("ResultArea").value= "CGI Call Failed";
    }
    document.getElementById("ResultArea").disabled = true;
}

function fClearAuthenticateUserPage() {
    document.getElementById("idUsername").value = "";
    document.getElementById("idPassword").value = "";
    document.getElementById("ResultArea").value = "";
    document.getElementById("idUsername").focus();
    var x = document.getElementById("HELPDIV");
    x.style.display = "";

    fcClearExtras();
}

