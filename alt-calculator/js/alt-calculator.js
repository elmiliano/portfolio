// alternate calculator
// by Emiliano Machado

operators = ['+', '-', '*', '/'];
operands = [];                                            // for storing the equation's operands

function calculate(expression) {                    
  expression = expression.split(' ');                     // seperate all characters in the string 
  if (!operators.includes(expression[0])) {
    return expression;
  
  } else {
    return checkOperator(expression, 0);                  // begin recursive function (checkOperator)
  }
}

function handleOperator(expression, index) {                 // takes an expression and an index, returns the next two operands

  flag = false;

  for (i = 0; i < 2; i++) {
    flag = getOperands(expression, index);

    if (flag) {
      index += 2;
    }
  }

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

function getOperands(expression, index) {
  if (operators.includes(expression[index+1])) {           
    operands.push(checkOperator(expression, index+1));    // recursive function (checkOperator) pushes operands
    return true;                                          // skips the operator's next two operands
  } else {
    operands.push(expression[index+1]);    
    return false;                                         // pushes next character
  }
}

function checkOperator(expression, index) {               // gets an expression's operands and returns the result
  temp = handleOperator(expression, index);               // gets an expression's operands (a, b)
  
  switch (expression[index]) {                            // performs operation depending on operator (+, -, *, /)
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

console.log(calculate('* + 3 4 5'));                      // = (3 + 4) * 5 = 35