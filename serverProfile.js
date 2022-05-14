// serverProfile.js -- script to implement help in about.htm
// Author: Geoff Jarman
// Started: 09-Oct-2021
// Log: 
//    09-Oct-2021 start and test
//    09-Oct-2021 remove uneeded function to hide internals
//    11-May-2022 capitalize helpDiv id
//    12-May-2022 change "none" to "" on x.style.display
  
var x = document.getElementById("helpDiv");

// functions for Help button to display and hide help ..................................................................
  
function fShowHelp() {
    var x = document.getElementById("HELPDIV");
    if (x.style.display === "") {
        x.style.display = "block";
    } else {
        x.style.display = "";
    }
}
