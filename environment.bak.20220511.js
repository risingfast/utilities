// Comments region .....................................................................................................
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
//                                                                                                                      
// .....................................................................................................................


// globals

const uri1 = "http://www.risingfast.com/cgi-bin/environment.cgi";

// function to set document model values to match action chosen by the user ............................................

function fSetAction() {
    var sChoice = document.getElementById("action").value;

    var s = document.getElementById("submitButton");
    var r = document.getElementById("resultsDiv");
    if (sChoice == "Choose") {
        var s = document.getElementById("submitButton");
        s.disabled = true;
        r.style.display = "none";
    } else {
        s.disabled = false;
        r.style.display = "none";
    }
}

// functions for buttons to clear page .................................................................................

function fClearPage() {
    var x = document.getElementById("helpDiv");
    x.style.display = "none";

    var a = document.getElementById("action");
    a.value = "Choose";

    var s = document.getElementById("submitButton");
    s.disabled = true;

    var r = document.getElementById("resultsDiv");
    r.style.display = "none";
}

// functions for button to display help ................................................................................

function fShowHelp() {
    var x = document.getElementById("helpDiv");
    if (x.style.display === "none") {
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }
}

// function to ajax fetch the environment from the server

async function fGetResults() {
    
    var r = document.getElementById("resultsDiv");
    r.style.display = "block";
    
    let response = await fetch(uri1 + '?' + document.getElementById("action").value);
    if (response.ok) {
        let text = await response.text();
        console.log(text.slice(0, 80));
        document.getElementById("ResultArea").style.display="inline-block";
        document.getElementById("ResultArea").value=text;
    } else {
        alert("HttpError: " + response.status);
    }
}
