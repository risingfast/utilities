// canvas.js -- script to implement canvas.html
// Author: Geoff Jarman
// Started: 02-Nov-2023
// Log: 
//    02-Nov-2023 start
//    08-Nov-2023 add circle object and multiple circles
//    09-Nov-2023 add defaults to displayed controls
//    12-Nov-2023 adjust code for fixed canvas size
//    13-Nov-2023 
// References
//     https://www.youtube.com/watch?v=EO6OkltgudE -- HTML5 Canvas Tutorial for Complete Beginners
//     https://www.youtube.com/watch?v=83L6B13ixQ0 -- Drawing On HTML5 Canvas for Complete Beginners

// define a canvas and a canvas context object -------------------------------------------------------------------------

let canvas = document.querySelector("canvas");
let ctx = canvas.getContext("2d");
let dfltCircles = 50;
let dfltSpeed = 1;
let dfltRadius = 20;
let intCount = document.querySelector("#count-input").value = dfltCircles;
let intRadius = document.querySelector("#size-input").value = dfltRadius;
let intSpeed = document.querySelector("#speed-input").value = dfltSpeed;
let mouse = {
    x: 3000,
    y: 3000
}

// define a circle constructor -----------------------------------------------------------------------------------------

function Circle(x, y, dx, dy, r) {
    this.x = x;
    this.y = y;
    this.dx = dx;
    this.dy = dy;
    this.r = r;

    this.draw = function() {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.r, 0, Math.PI * 2, false);
        ctx.strokeStyle = "green";
        ctx.stroke();

        if (this.r/intRadius < 1) {
            ctx.fillStyle = "red";
        } else if (this.r/intRadius < 2) {
            ctx.fillStyle = "green";
        } else if (this.r/intRadius < 3) {
            ctx.fillStyle = "blue";
        } else if (this.r/intRadius < 4) {
             ctx.fillStyle = "yellow";
        } else {
             ctx.fillStyle = "orange";
        }
        ctx.fill();
    }

// function to calculate the location and size of each cirle in cirleArray[] and draw on the canvas --------------------

    this.update = function() {
        if (this.x + this.r > 600) {
            this.dx = -this.dx;
        } else if ( this.x < this.r) {
            this.dx = this.dx * -1;
        }
        if (this.y + this.r > 300) {
            this.dy = -this.dy;
        } else if (this.y < this.r) {
            this.dy = this.dy * -1;
        }

        this.x += this.dx;
        this.y += this.dy;

        if ((mouse.x - this.x < 20) && (mouse.x - this.x > -20) 
            && (mouse.y - this.y < 20) && (mouse.y - this.y > -20 )) {
            this.r += 10;
            console.log(mouse.x, mouse.y);
            mouse.x = undefined;
            mouse.y = undefined;
        }

        this.draw();
    }
}

// define a function to build an array of circles with the current control settings ------------------------------------

function fCreateCircleArray() {

    circleArray.length = 0;

    for (let i = 0; i < intCount; i++) {

        let r = Math.random() * intRadius;
        let x = r + (Math.random() * (canvas.width - r));
        let y = r + (Math.random() * (canvas.height - r));
        let dx = Math.random() * intSpeed;
        let dy = Math.random() * intSpeed;
        circleArray.push(new Circle(x, y, dx, dy, r));
    }
}

// define a function to warn the message area is read-only -------------------------------------------------------------

function fShowMessageReadOnly() {
    document.querySelector("#message-input").value = "This field is read only";
}

// animation function --------------------------------------------------------------------------------------------------

function animate() {

    ctx.clearRect(0, 0, 600, 400);
    
    for (let i = 0; i < intCount; i++) {
        circleArray[i].update();
    }
    requestAnimationFrame(animate);
}

// submit button function to apply changes in controls for size, speed and count ---------------------------------------

function fSubmit() {

    intCount = document.querySelector("#count-input").value;
    intRadius = document.querySelector("#size-input").value;
    intSpeed = document.querySelector("#speed-input").value;
    fCreateCircleArray();
    animate();
}

// function to clear fields and reset default valuse -------------------------------------------------------------------

function fClearCanvasFields()
{
    intCount = document.querySelector("#count-input").value = dfltCircles;
    intRadius = document.querySelector("#size-input").value = dfltRadius;
    intSpeed = document.querySelector("#speed-input").value = dfltSpeed;
    fcClearExtras();
    document.querySelector("#message-input").value = "All values cleared and reset to defaults";

    circleArray = [];
    fCreateCircleArray();
    animate();
}

// function to click the submit button when Enter is clicked in an input field -----------------------------------------

function fClickSubmitOnEnter() {
    if (event.key === 'Enter') {
        document.getElementById("submit-button").click();
    }
    document.querySelector("#message-input").value = "Control values applied to canvas animation"
}

// function to listen for a mouse movement in the canvas and update the mouse position ---------------------------------

canvas.addEventListener("click", function(event) {
    if (event.offsetX <= 600) {
        mouse.x = event.offsetX;
    } else {
        mouse.x = undefined;
    }
    if (event.offsetY <= 300) {
        mouse.y = event.offsetY;
    } else {
        mouse.y = undefined;
    }
})

let circleArray = [];
fCreateCircleArray();
animate();


