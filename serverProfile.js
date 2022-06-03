// serverProfile.js -- script to implement help in about.htm
// Author: Geoff Jarman
// Started: 09-Oct-2021
// Log: 
//    09-Oct-2021 start and test
//    09-Oct-2021 remove uneeded function to hide internals
//    11-May-2022 capitalize helpDiv id
//    12-May-2022 change "none" to "" on x.style.display
//    15-May=2022 remove fShowHelp()
//    01-Jun=2022 add cornerimage rotation

const uri2 = "http://www.risingfast.com/cgi-bin/setCornerImage.cgi";

function fClearServerProfilePage() {
    fClearExtras();
}

// function to ajax fetch the current corner image and captiona

async function fSetCornerImage() {
    let response = await fetch(uri2);
    if (response.ok) {
        let text = await response.text();
        let array = text.split("\n");
        array.pop();                      // remove the last element (empty element) created by the split("\n")
        let intRecords = array.length/3;
        let intRecordSelected = Math.trunc(Math.random() * intRecords);
        document.getElementById("ASIDE2IMG").src=array[intRecordSelected * 3]
        document.getElementById("ASIDE3-PARA").innerHTML=array[(intRecordSelected * 3) + 1];
    } else {
        alert("HttpError: " + response.status);
    }
}

