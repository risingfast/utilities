// options.js -- script to implement help in options.html
// Author: Geoff Jarman
// Started: 16-Jun-2022
// Log: 
//    16-Jun-2022 start and test

const uri2 = "http://gjarman2020.com/cgi-bin/setCornerImage.cgi";

function fClearTemplateFields() {
    fClearExtras();
}

// function to ajax fetch the current corner image and caption

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

