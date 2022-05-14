// serverProfile.js -- script to implement help in about.htm
// Author: Geoff Jarman
// Started: 09-Oct-2021
// Log: 
//    09-Oct-2021 start and test
//    09-Oct-2021 remove uneeded function to hide internals
  
var x = document.getElementById("helpDiv");

// functions for Help button to display and hide help ..................................................................
  
function fShowHelp() {
    var x = document.getElementById("helpDiv");
    if (x.style.display === "none") {
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }
}
