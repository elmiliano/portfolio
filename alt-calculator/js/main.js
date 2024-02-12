import {calculate} from './alt-calculator.js';

function getExpression() {
    let temp = document.getElementById("expression").value;
    return temp;
};

document.getElementById("eq_btn").onclick = function () {
    let expression = getExpression();
    let result = calculate(expression);
    console.log(result);
};