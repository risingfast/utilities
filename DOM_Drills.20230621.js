// DOM_Drills.js -- script to implement DOM manipulation drills
// Author: Geoff Jarman
// Started: 04-Aug-2021
// Log: 
//    04-Aug-2022 start and test
//    11-Aug-2022 add functions for text, text color and long list
//    14-Jun-2023 add function to fetch list of people in ancestry files
//    17-Jun-2023 clean up variable names and label and organize functions
//    17-Jun-2023 implement strict-compliant variable declarations
//    19-Jun-2023 expand name prefixes
//    19-Jun-2023 simplify list movements by setting current position then calling  fMoveDownList()
//    20-Jun-2023 simplify fRefreshList()
// Prefixes
//    dm -- DOM object or attribute
//    i  -- integer
//    s  -- string
//    sa -- string array
//    f  -- function

'use strict';

// global variable declarations ----------------------------------------------------------------------------------------

const uri = "http://gjarman2020.com/cgi-bin/mcaListPeople.cgi";
var iListDisplayLength = 25;                                                   // number of entries displayed in the list
var iListDisplayCount = 0;                                                   // count of entries displayed on the webpage
var saPeopleProfiles = ['nothing'];                                                    // string array of people profiles
var iCurrOffset = 0;                                             // current read-position in the array of people profiles
var saInputFields = [];                                                    // string array of input fields on the webpage
var iProfilesCount = 0;                                                             // count of people profiles retrieved
const para1 = document.querySelector("#para1");

// function to change or revert static text ----------------------------------------------------------------------------

function fChangeOrRevertText() {
    const bttn1 = document.querySelector("#bttn1");
    if (bttn1.textContent == "Chnge Text") {
        para1.innerText = "This text has been changed by clicking the 'Chnge Text' button";
        bttn1.textContent = "Revert Text";
    } else {
        para1.innerText = "This text has been reverted by clicking the 'Revert Text' button";
        bttn1.textContent = "Chnge Text";
    } 
}

// function to change or revert the color of static text ---------------------------------------------------------------

function fChangeOrRevertColor() {
    const para1 = document.querySelector("#para1");
    const bttn2 = document.querySelector("#bttn2");
    let dmPara1Color = para1.getAttribute("style");
    if (dmPara1Color == "color:red") {
        para1.setAttribute("style", "color:blue");
        para1.innerText = "The color of this text has been changed by clicking the 'Change Color' button";
        bttn2.textContent =  "Revert Color";
    } else {
        para1.setAttribute("style", "color:red");
        para1.innerText = "The color of this text had been changed again by clicking the 'Change Color' button";
        bttn2.textContent = "Chnge Color";
    } 
}

// function to show a list on a webpage --------------------------------------------------------------------------------

function fShowOrHideList() {
    let dmParaDiv = "";
    bttn4.removeAttribute("disabled");
    let sHtmlString = ``;
    const bttn3 = document.querySelector("#bttn3");
    let dmDiv1 = document.createElement('div');
    if (!document.getElementById('added-div')) {                                    // check if added-div does not exist
        dmParaDiv = document.querySelector('#paragraph-div');
        dmParaDiv.append(dmDiv1);
        dmDiv1.innerText = "New Div Added";
        dmDiv1.setAttribute("id", "added-div");
        bttn3.textContent =  "Hide List";
        for (let i = 0; i < iListDisplayLength; i++) {
            sHtmlString += `<input id="inputOffset${i}" size="75" value=""><button id="inputBttn${i}" onclick="fGetBttnID(this)">Fetch</button><br>`;
        }
        dmDiv1.innerHTML = sHtmlString;
        iCurrOffset = iListDisplayLength;
        iListDisplayCount = iListDisplayLength;
        para1.innerText = "A list of elements is filtered and displayed";
        bttn5.removeAttribute("disabled");
        bttn8.removeAttribute("disabled");
        bttn6.setAttribute("disabled", true);
        bttn7.setAttribute("disabled", true);
        fFetchPeople();
    } else {                                                   // remove added-div if it already exists to hide the list
        bttn4.setAttribute("disabled", true);
        bttn5.setAttribute("disabled", true);
        bttn6.setAttribute("disabled", true);
        bttn7.setAttribute("disabled", true);
        bttn8.setAttribute("disabled", true);
        dmDiv1 = document.querySelector("#added-div");
        dmDiv1.remove();
        bttn3.textContent =  "Show List";
        para1.innerText = "The list is hidden";
    }
}

// function to fetch people profiles from a database into an array ------------------------------------------------------

async function fFetchPeople() {

    let sText1 = "";                                                       // string of people profiles from the database
    let sText2 = "";                                            // string of 'scrubbed' people profiles from the database
    let sFilter = "";                                                                         // filter on people pofiles
    let response = await fetch(uri);
    iListDisplayCount = 0;
    if (response.ok) {
        sText1 = await response.text();
        sText2 = sText1.replace(/\|/g, ", ");
        sText1 = sText2;
        sText2 = sText1.replace(/, , , /g, ", ");
        sText1 = sText2;
        sText2 = sText1.replace(/, ,/g, ", ");
        saPeopleProfiles = sText2.split("\n");
        let saArrayTmp = saPeopleProfiles;                                         // copy the array to apply the filter
        saPeopleProfiles = [];

        sFilter = (document.getElementById("filter-input").value).toUpperCase();
        if(sFilter.length != 0)                                              // apply the filter if there is filter text
        {
             for(let i = 0, j = 0; i < saArrayTmp.length; i++)
             {
                 if((saArrayTmp[i]).toUpperCase().indexOf(sFilter) != -1)   // check filter match for each array element
                 {
                     saPeopleProfiles[j] = saArrayTmp[i];            // copy the array values back if the filter matches
                     j = j + 1;
                     iListDisplayCount++;
                 }
             }
        } else {                                                                // copy all values if there is no filter
             saPeopleProfiles = saArrayTmp;
             iListDisplayCount = iListDisplayLength;
        }
        iProfilesCount = saPeopleProfiles.length;                      // set a count of the number of filtered profiles
        saArrayTmp = [];                                                                // clean out the temporary array
        sText1 = "";                                                                      // clean out the sText1 string
        sText2 = "";                                                                       // clean out the text2 string
    }
    
    saInputFields= Array.from(document.querySelector("#added-div").querySelectorAll("input"));
    iCurrOffset = 0;

    for (let j = 0; j < iListDisplayLength; j++) {                                  // wipe the input field values clean
        saInputFields[j].value="";
    }

    for (let k = 0; (k < iListDisplayLength) && (k <iProfilesCount) ; k++) {
        saInputFields[k].value = saPeopleProfiles[k];
        iCurrOffset = k + 1;                                     // set the current position of the people profile array
    }
    
    if (iListDisplayCount < iListDisplayLength) {
        bttn5.setAttribute("disabled", true);
        bttn6.setAttribute("disabled", true);
        bttn7.setAttribute("disabled", true);
        bttn8.setAttribute("disabled", true);
    } else {
        bttn5.removeAttribute("disabled");
        bttn8.removeAttribute("disabled");
    }
}

// function to refresh the list of people and apply any filter changes -------------------------------------------------

function fRefreshList() {
    iCurrOffset = 0;
    fFetchPeople();
    para1.innerText = "The list of elements is re-filtered and refreshed";
}

// function to move down the list of preople profiles and display the next set of values -------------------------------

function fMoveDownList() {

    iListDisplayCount = 0;
    if (iCurrOffset < iProfilesCount - iListDisplayLength) {
    
        for (let j = 0; j < iListDisplayLength; j++) {                              // wipe the input field values clean
            saInputFields[j].value = "";
        }

        for (let j = 0; (j < iListDisplayLength) && (iCurrOffset < iProfilesCount); j++) {
            if (iCurrOffset < iProfilesCount - 1) {
                saInputFields[j].value = saPeopleProfiles[iCurrOffset++];
                iListDisplayCount++;                                         // increment the count of displayed records
            } else {
                bttn5.setAttribute("disabled", true);
                bttn8.setAttribute("disabled", true);
                break;
            }
        }
        para1.innerText = "The displayed values are moved down the list";
        bttn6.removeAttribute("disabled");
        bttn7.removeAttribute("disabled");
    } else {
        bttn5.setAttribute("disabled", true);
        bttn8.setAttribute("disabled", true);
    }
}

// function to move up the list of people profiles and display the prior set of values ---------------------------------

function fMoveUpList() {

    iCurrOffset = iCurrOffset - iListDisplayCount - iListDisplayLength;

    if (iCurrOffset < 0) {
        iCurrOffset = 0;                                                          // don't go past the start of the list
        bttn6.setAttribute("disabled", true);
        bttn7.setAttribute("disabled", true);
        fRefreshList();
    } else {
        fMoveDownList();
        bttn5.removeAttribute("disabled");
        bttn8.removeAttribute("disabled");
    }
    para1.innerText = "The displayed values are moved up the list";

}

// function to move to the top of the list of people profiles ----------------------------------------------------------

function fMoveToListTop() {

    iCurrOffset = 0;
    fMoveDownList();
    bttn6.setAttribute("disabled", true);
    bttn7.setAttribute("disabled", true);
    bttn5.removeAttribute("disabled");
    bttn8.removeAttribute("disabled");
    para1.innerText = "The displayed values are the top of the list";

}

// function to move to the bottom of the list of people profiles -------------------------------------------------------

function fMoveToListBottom() {

    iCurrOffset = (iProfilesCount - 1) - iListDisplayLength;
    fMoveDownList();
    bttn5.setAttribute("disabled", true);
    bttn8.setAttribute("disabled", true);
    para1.innerText = "The displayed values are the bottom of the list";

}

// function jump to a person's profile from the peopel list jump field

function fonclick_fetch_submit() {
    document.getElementById("profiles-input").checked = true;
    document.getElementById("personid-input").value = document.getElementById("jump-input").value;
    fMcaListPeople();
    fGetAction();

}

// function to fetch the profile id of a person from their profile

function fGetBttnID(btn) {

    var iPerson_ID= Number(btn.id.replace("inputBttn", ""));
    alert(saInputFields[iPerson_ID].value);
}
