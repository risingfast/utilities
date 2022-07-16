// about.js -- script to implement help in about.html
// Author: Geoff Jarman
// Started: 19-Sep-2021
// Log: 
//    19-Sep 2021 start and test
//    10-May-2022 chang case on HELPDIV element id
//    12-May-2022 change "none" to "" on x.style.display 
//    16-May-2022 add fClearAboutFields()
//    31-May-2022 add fSetCornerImage();
//    18=Jun-2022 move fSetCornerImage() to common.js;


function fClearAboutFields() {
    x = document.getElementById("about-area");
    x.value = "";

    fcClearExtras();
}

