// alternate calculator
// by Emiliano Machado

operators = ['+', '-', '*', '/'];
operands = [];                                             // for storing the equation's operands

function calculate(expression) {                    
  expression = expression.split(' ');                     // seperate all characters in the string 
  if (!operators.includes(expression[0])) {
    return expression;
  
  } else {
    return checkOperator(expression, 0);                  // begin recursive function (checkOperator)
  }
}

function getOperands(expression, index) {                 // takes an expression and an index, returns the next two operands
  if (operators.includes(expression[index+1])) {           
    operands.push(checkOperator(expression, index+1));    // recursive function (checkOperator) pushes operands
    index += 2;                                           // skips the operator's next two operands
  } else {
    operands.push(expression[index+1]);                   // pushes next character
  }

  if (operators.includes(expression[index+2])) {
    operands.push(checkOperator(expression, index+2));
  } else {
    operands.push(expression[index+2]);
  }
  
  return [Number.parseInt(operands.pop()), Number.parseInt(operands.pop())]; // returns numbers as a list of ints
}

function checkOperator(expression, index) {               // gets an expression's operands and returns the result
  temp = getOperands(expression, index);                  // gets an expression's operands (a, b)
  
  switch (expression[index]) {                            // performs operation depending on operator
    case '+':
      return temp[1] + temp[0];
    
    case '-':
      return temp[1] - temp[0]; 

    case '*':
      return temp[1] * temp[0];

    case '/':
      return temp[1] / temp[0]; 

    default:
      return expression;
    }
}

console.log(calculate('* + 3 4 5'));