// Index.js // 
let x = 0;
document.getElementById("text").textContent = x;

document.getElementById("b1").onclick = function () {
    document.getElementById("text").textContent = ++x;
}

document.getElementById("b2").onclick = function () {
    x = 0;
    document.getElementById("text").textContent = x;
}

document.getElementById("b3").onclick = function () {
    document.getElementById("text").textContent = --x;
}


