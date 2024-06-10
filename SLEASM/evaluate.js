'use strict';

const fs = require('fs');
const G = require('./G.js');
const { die } = require('./logger.js');

function Evaluate(expression) {
    // console.log(`Evaluate('${expression}')`);
//    try {
        let rpn = shuntingYard(expression);
         return evaluateRPN(rpn);
 //   } catch (err) {
 //       if (G.pass==2) {
 //           console.error(`Error in pass ${G.pass}: ${err.message}`);
 //           process.exit(1);
 //       }
        return 16777216;
//    }
}


function shuntingYard(expression) {
    let outputQueue = [];
    let operatorStack = [];
    let operators = {
        '+': 1,
        '-': 1,
        '*': 2,
        '/': 2,
        '>>': 3,
        '<<': 3,
        '&': 4,
        '^': 5,
        '|': 6,
    };

    // console.log(`ShuntingYard('${expression})`);

    let tokens = expression.split(' ');

    for (let token of tokens) {
        // Skip empty tokens
        if (token === '') continue;

        // If the token is a number, add it to the output queue
        if (!isNaN(parseInt(token))) {
            outputQueue.push(token);
            continue;
        }

        // If the token is an operator
        if (token in operators) {
            while (operatorStack.length > 0 && operators[token] <= operators[operatorStack[operatorStack.length - 1]]) {
                outputQueue.push(operatorStack.pop());
            }
            operatorStack.push(token);
            continue;
        } 
        
        // If the token is a left parenthesis, push it onto the operator stack
        if (token === '(') {
            operatorStack.push(token);
            continue;
        } 
        
        if (token === ')') {
            while (operatorStack.length > 0 && operatorStack[operatorStack.length - 1] !== '(') {
                outputQueue.push(operatorStack.pop());
            }
            if (operatorStack.length === 0 || operatorStack.pop() !== '(') {
                throw new Error('Mismatched parentheses');
            }
            continue;
        }

        // console.log(`token='${token}'`);

        // If the token is a local label, add the procedure name to it
        // So @FOO becomes PROCNAME_FOO
        if (token[0] == '@') {
            if (G.procName == '') {
                die(`Local token ${token} without procedure name`);
            }
            token = G.procName + '_'+token.substring(1);
        }

        // If the token is a symbol, add it to the output queue
        if (G.symbols.has(token)) {
            outputQueue.push(G.symbols.get(token).toString());
            continue;
        }

        if (G.pass == 2) {
            // console.table(G.symbols);
            die(`Unknown token: ${token}`);
        }
    }

    while (operatorStack.length > 0) {
        let operator = operatorStack.pop();
        if (operator === '(' || operator === ')') {
            throw new Error('Mismatched parentheses');
        }
        outputQueue.push(operator);
    }

    // console.log(`outputQueue='${outputQueue}`);
    return outputQueue;
}



function evaluateRPN(tokens) {
    // console.log(`evaluateRPN('${tokens}')`);
    let stack = [];

    for (let token of tokens) {
        if (!isNaN(parseInt(token))) {
            stack.push(parseInt(token));
        } else {
            let b = stack.pop();
            let a = stack.pop();

            switch (token) {
                case '+':
                    stack.push(a + b);
                    break;
                case '-':
                    stack.push(a - b);
                    break;
                case '*':
                    stack.push(a * b);
                    break;
                case '/':
                    stack.push(a / b);
                    break;
                case '>>':
                    stack.push(a >> b);
                    break;
                case '<<':
                    stack.push(a << b);
                    break;
                case '&':
                    stack.push(a & b);
                    break;
                case '^':
                    stack.push(a ^ b);
                    break;
                case '|':
                    stack.push(a | b);
                    break;

                default:
                    throw new Error(`Invalid operator: ${token}`);
            }
        }
    }

    let result=stack.pop();
    //console.log(`='${result}'`);
    return result;
}

exports.Evaluate = Evaluate;
