// autheticateUser.js -- script to implement CG call in authenticateUser.html
// Author: Geoff Jarman
// Started: 24-Sep-2021
// Log: 
//    24-Sep 2021 start and test
//    24-Sep 2021 add fAuthenticateUser() and fClear()
//    25-Sep-2021 add idResult field to fClear()
//    25-Sep-2021 set focus() on username in fClear()
//    16-Oct-2021 add to fClearHTML to clear help()

const uri1 = "http://www.risingfast.com/cgi-bin/authenticateUser.cgi";

// functions for action buttons to display and hide help ..........................................
  
function showHelp() {
    var x = document.getElementById("helpDiv");
    if (x.style.display === "none") {
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }
}

function resetShows() {
    var x = document.getElementById("helpDiv");
    x.style.display = "none";
}

async function fAuthenticateUser() {
    
    var sUsername = document.getElementById("idUsername").value;
    var sPassword = document.getElementById("idPassword").value;

    fClearPage();
    document.getElementById("idResult").disabled = false;

    const uri2 = uri1 + '?Username=' + sUsername + '&Password=' + sPassword;
    let response = await fetch(uri2);

    if (response.ok) {
        let text = await response.text();
        document.getElementById("idResult").value= text;
    } else {
        document.getElementById("idResult").value= "CGI Call Failed";
    }
    document.getElementById("idResult").disabled = true;
}

function fClearPage() {
    document.getElementById("idUsername").value = "";
    document.getElementById("idPassword").value = "";
    document.getElementById("idResult").value = "";
    document.getElementById("idUsername").focus();
    var x = document.getElementById("helpDiv");
    x.style.display = "none";
}
