#!/usr/bin/env node
'use strict';

const fs = require('fs');
const G = require('./G.js');

const { logger,die } = require('./logger');
const { Evaluate } = require('./evaluate');
const { WriteInfoToListfile, WriteV20RawFile } = require('./listfile');
const { stripComment,splitAtCommas,expandTabs,parseNumber } = require('./stringUtils');
const { openFileAndPushToStack,readLine } = require('./fileUtils');
const { log } = require('console');



const argFile = process.argv[2];
if (!argFile) {
    console.error('Usage: sleasm.js <filename>');
    process.exit(1);
}


logger.setLogLevel(0);          // 0=Silent, 1=Info, 2=Debug, 3=Verbose
if (process.argv[3] > 0) logger.setLogLevel(process.argv[3]); 
if (process.argv[4] > 0) logger.setLogLevel(process.argv[4]); 
if (process.argv[3]=="debug") G.debug=true;
if (process.argv[4]=="debug") G.debug=true;


// Create a listing file with the same name as the input file but with the extension .lst
let filename = argFile.includes('.') ? argFile.replace(/\.[^/.]+$/, '.lst') : argFile + '.lst';
let fdList = fs.openSync(filename, 'w');

G.pass=1;                   // First pass
G.localNo=1000;
ProcessFile(argFile,0);       

// console.table(G.symbols);

G.pass=2;                    // Second pass
G.localNo=1000;
ProcessFile(argFile,fdList); 
fs.writeSync(fdList,"\n\n\n");

WriteInfoToListfile(fdList);
fs.closeSync(fdList);

filename = argFile.includes('.') ? argFile.replace(/\.[^/.]+$/, '.v20raw') : argFile + '.v20raw';
let fdV20RawFile = fs.openSync(filename, 'w');
WriteV20RawFile(fdV20RawFile);
fs.closeSync(fdV20RawFile);


function ProcessFile(argFile, listFile) {
    logger.info(`Running pass ${G.pass}`);
    openFileAndPushToStack(argFile);

    // Set the program counter to 0, also adds it to the symbol table as $ if not there already
    setPC(0);              
    
    // Read lines until EOF, then close the desciptor and pop it from the stack and continue 
    // reading from the previous file descriptor until the stack is empty

    while (G.fileStack.length > 0) {

        // If there are lines in the macro queue, process them first
        let line='';
        if (G.macroLines.length>0) {
            line=G.macroLines.shift();
        } else {
            line = readLine(G.fileStack[G.fileStack.length - 1].fd);
            // If EOF, close the file descriptor and pop it from the stack
            if (line === null) {
                fs.closeSync(G.fileStack.pop().fd);
                continue;
            }
        }

        // Increment line number for the current file being read
        // Also store the current line so it can be used for logging and error messages
        G.fileStack[G.fileStack.length - 1].lineno++;
        G.fileStack[G.fileStack.length - 1].line = line;

        ProcessLine(line, listFile);
    }
}

function ProcessLine(originalLine, listFile) {
    const MAXDATADUMP = 3;

    logger.debug(`${G.procName}>>>${originalLine}`);

    // If we're in defining a macro, add the line to the macro
    if (G.macroname!='') {
        logger.verbose(`'${originalLine.trim().substring(5)}'`);
        if (originalLine.trim().substring(0,5)=='.endm') {
            logger.verbose(`End of macro ${G.macroname}`);
            G.macroname='';
            return;
        }
        logger.verbose(`Adding line to macro ${G.macroname}`);
        G.macros.get(G.macroname).lines.push(originalLine);
        return;
    }

    let line=stripComment(originalLine);
    let pcAtLineStart=G.pc;

    // If there's a label on the line, parse it and remove it from the line
    let label;
    [ line, label ] = parseLabelFromLine(line);

    // If the first character of the line is a dot, it is a directive
    if (line[0] == '.') {
        let vals=HandleDirective(line, label);
        if (listFile > 0) {
            fs.writeSync(listFile, `${pcAtLineStart.toString(16).padStart(6, '0')}:                     ${originalLine}\n`);
            if (vals && vals.length > 0) {
                let output = `${pcAtLineStart.toString(16).padStart(6, '0')}:`;
                for (let i = 0; i < vals.length; i++) {
                    // Convert negative numbers to 24-bit representation
                    let val = vals[i] < 0 ? 0x1000000 + vals[i] : vals[i];
                    output += val.toString(16).padStart(6, '0').toUpperCase() + ' ';
                    if ((i + 1) % MAXDATADUMP === 0) {
                        fs.writeSync(listFile, output + '\n');
                        output = '       ';
                    }
                }
                if (output.trim().length > 0) {
                    fs.writeSync(listFile, output + '\n');
                }
            }
        }
        return;
    }

    // We now either have a subleq instruction or a macro that needs to be expanded
    let A, B, C, D, E, F;
    [line, A] = getExpression(line);
    [line, B] = getExpression(line);
    [line, C] = getExpression(line);
    [line, D] = getExpression(line);
    [line, E] = getExpression(line);
    [line, F] = getExpression(line);

    if (!A || !A.length) {
        if (listFile > 0) {
            fs.writeSync(listFile, `${pcAtLineStart.toString(16).padStart(6, '0')}:                     ${originalLine}\n`);
        }
        return;
    }

    // if the first token is in the macro table, expand the macro
    if (G.macros.has(A)) {
    //    console.log(`Expanding macro ${A} which have ${G.macros.get(A).params} as parameters`);
    //    console.log(`A=${A} B=${B} C=${C} D=${D} E=${E} F=${F}`)
        B=fixLocalLabel(B);
        C=fixLocalLabel(C);
        D=fixLocalLabel(D);
        E=fixLocalLabel(E);
        F=fixLocalLabel(F);
        // console.log(`A=${A} B=${B} C=${C} D=${D} E=${E} F=${F}`)

    // get a new local number for the macro
        G.localNo++;
        G.macros.get(A).local=G.localNo;

        // console.log(`Expanding macro ${A} G.localNo=${G.localNo} macros.get(A).local=${G.macros.get(A).local}`);

        // insert the expanded macro at beginning of macroLines in case we're already inside another macro
        let tmp = [];
        G.macros.get(A).lines.forEach(s => {
            // console.log(`Expanding s ${s}`);
            // replace the parameters in the macro with the actual args
            let line = s;
            let i = 0;
            if (G.macros.get(A).params) {
                for (let param of G.macros.get(A).params.split(' ')) {
                    // console.log(`${i} Replacing ${param} s now is'${s}' `);
                    param='\\b'+param+'\\b';
                    if (i==0) s = s.replace(new RegExp(param, 'g'), B);
                    if (i==1) s = s.replace(new RegExp(param, 'g'), C);
                    if (i==2) s = s.replace(new RegExp(param, 'g'), D);
                    if (i==3) s = s.replace(new RegExp(param, 'g'), E);
                    if (i==4) s = s.replace(new RegExp(param, 'g'), F);
                    i++;                    
                }
            }
            // replace the local labels with the current local number
            s = s.replace(/@/g, 'LOCAL_'+G.macros.get(A).local+'_');
            // console.log(`Pushing ${s}`);
            tmp.push(s);
            // console.table(tmp);
        });
        G.macroLines = tmp.concat(G.macroLines);
        return;
    }

    // the only remaining possibility is an instruction which can be one, two or three tokens
    // console.log(`A='${A}' B='${B}' C='${C}'`);
    G.memory[G.pc]=Evaluate(A)
    setPC(G.pc+1);

    if (B==undefined) B=A;
    G.memory[G.pc]=Evaluate(B)
    setPC(G.pc+1);

    if (C==undefined) C="$ + 1";
    G.memory[G.pc]=Evaluate(C)
    setPC(G.pc+1);

    if (listFile>0) {
        let s1 = (G.memory[pcAtLineStart+0] & 0xFFFFFF).toString(16).padStart(6, '0');
        let s2 = (G.memory[pcAtLineStart+1] & 0xFFFFFF).toString(16).padStart(6, '0');
        let s3 = (G.memory[pcAtLineStart+2] & 0xFFFFFF).toString(16).padStart(6, '0');
        fs.writeSync(listFile,`${pcAtLineStart.toString(16).padStart(6, '0')}:${s1} ${s2} ${s3} ${originalLine}\n`);
    }

}      

function fixLocalLabel(label) {
    if (label!== undefined && label[0] == '@') {
        if (G.procName == '') {
            die(`Local token ${label} without procedure name`);
        }
        return G.procName + '_'+label.substring(1);
    }
    return label;
}   


// Process a directive. Directives are:
//  .end
//  .proc NAME
//  .endp
//  .include FILENAME
//  .macro MACRONAME [variable] ...
//  .endm
//  .org    ADDRESS
//  .equ 
//  .data [expression]  ...
function HandleDirective(line, label, pass) {
    // Split the line at the first space or EOL to get the directive and args
    let [directive, args] = line.split(/ (.+)/);
    if (!args) args = '';

//    logger.debug(`Directive='${directive}' with argument='${args}'`);
    let vals=[];

    switch (directive.toLowerCase()) {
        //
        // .END directive - treat it as a no-op
        //
        case '.end':
            return;

        //
        // .PROC directive - set the procedure name used for generating local labels
        //
        case '.proc':
            if (G.procName!='') die(`Nested procedures not allowed`);
            G.procName = args;
            return;

        //
        // .ENDP directive - clear the procedure name
        //
        case '.endp':
            if (G.procName=='') die(`Unmatched .endp`);
            G.procName = '';
            return;

        //
        // .MACRO directive - define a macro followed by its name and the argument list
        //
        // When the variable macroname is set, we are in the process of defining a
        // macro which is being handled inside the ProcessLine function.
        //
        case '.macro':
            let parameters = '';
            [G.macroname, parameters] = args.split(/ (.+)/);
            logger.debug(`Defining macro '${G.macroname}' with argument '${parameters}'`);
            // delete(G.macroname);
            G.macros.set(G.macroname,{params: parameters, local: 0,lines: []});
            return;

        //  
        // .INCLUDE directive - open the file and push the file descriptor to the stack
        //
        case '.include':
            openFileAndPushToStack(args);
            line=''; //Kill rest of line
            break;

        //
        // .ORG directive - set the program counter to the value of the expression
        //
        case '.org':
            let value;
            [line, value]=getExpression(args);
            try {
                setPC(Evaluate(value));
            } catch (err) {
                if (G.pass==2) die(`Can't evaluate: ${err}`)
            }
            break;

        //
        // .EQU directive - define a symbol with the value of the expression
        //
        case '.equ':
            [line, value]=getExpression(args);
            // try {
            G.symbols.set(label, Evaluate(value));
            // } catch (err) {
            //     if (G.pass==2) die(`Can't evaluate: ${err}`)
            // }
            break;

        //
        // .DATA directive - define data in memory
        //
        case '.data':
        let values=splitAtCommas(args);
        if (values.length==0) die('No values in .data directive')
            line=''; //Kill rest of line
            for (let value of values) {
            try {
                if (value[0]!='"') {    // Normal expression
                    let [, exp]=getExpression(value);
                    let v=Evaluate(exp);
                    G.memory[G.pc]=v;
                    setPC(G.pc+1);
                    vals.push(v);
                } else {                // ASCII string
                    for (let i = 1; i < value.length - 1; i++) {
                        let charCode;
                        if (value[i] === '\\' && i < value.length - 2) { // Check for escape character
                            i++; // Skip the backslash
                            switch (value[i]) {
                                case '0':
                                    charCode = '\0'.charCodeAt(0);
                                    break;
                                case 'n':
                                    charCode = '\n'.charCodeAt(0);
                                    break;
                                case 't':
                                    charCode = '\t'.charCodeAt(0);
                                    break;
                                case 'r':
                                    charCode = '\r'.charCodeAt(0);
                                    break;
                                case '[':
                                    charCode = '\x1B'.charCodeAt(0);
                                    break;
                                case '\\':
                                    charCode = '\\'.charCodeAt(0);
                                    break;
                                case '"':
                                    charCode = '"'.charCodeAt(0);
                                    break;
                                // Add more cases as needed
                                default:
                                    charCode = value.charCodeAt(i);
                                    break;
                            }
                        } else {
                            charCode = value.charCodeAt(i);
                        }
                        G.memory[G.pc] = charCode;
                        setPC(G.pc + 1);
                        vals.push(charCode);
                    }
                }
            } catch (err) {
                if (G.pass==2) die(`Can't evaluate: ${err}`)
            }
        }  
    }

    if (line.length==0 || line[0]==';') return vals;
    die(`Invalid characters at end of line`);
}


// if the line starts with a letter, it is a label. Collect the label and remove it from the line
// and set the label variable to the label. A label is a symbol followed by a colon or a space. The allowed
// characters for a symbol are letters, digits and underscore.
function parseLabelFromLine(line) {
    let label = '';
    // if the line does not start with a letter or a @-sign, it is not a label, so just return the line
    if (!(line[0] || ' ').match(/[a-zA-Z@_]/)) {
        return [line.trim(), label]
    }

    let i = 0;
    while (i < line.length && line[i].match(/[a-zA-Z0-9@_]/)) {
        label += line[i];
        i++;
    }
    if (i < line.length && line[i] == ':') {
        i++;
    }

    label = fixLocalLabel(label.trim())

    if (G.pass == 1 && G.symbols.has(label)) {
        die(`Label ${label} already defined`);
    }

    G.symbols.set(label, G.pc);
    line = line.substring(i).trim();
    return [ line, label ];
}



// Extract the first complete infix expression from the line and return the value of the expression
// the infix expression can contain hex-digits, the operators +, -, *, /, %, (, ) <<, >>, &, |, ^ or a symbol
function getExpression(line) {

    line=line.trim();
    if (line.length==0) return ['',undefined];

    let s='';
    
    let i=0;
    while (i<line.length ) {
        if (line[i]=='(') {
            s+='( ';
            i++;
        }

        // skip spaces
        while (i<line.length && line[i].match(/\s/)) i++;

        let current='unknown';
        if (line[i].match(/[a-zA-Z_$@]/)) current='symbol';
        if (line[i].match(/[0-9]/)) current='number';
        if ((line[i]).match(/[-]/)) current = 'number';
        if (line[i].match(/"/)) current='string';

        if (current=='unknown') die(`Invalid character '${line[i]}' in expression`);

        s+=line[i];
        i++;

        if (current=='string') {
            while (i<line.length && line[i]!='"') {
                s+=line[i];
                i++;
            }   
            s+='" ';
        }


        if (current=='symbol') {
            while (i<line.length && line[i].match(/[a-zA-Z0-9_]/)) {
                s+=line[i];
                i++;
            }
            s+=' ';
        }

        if (current=='number') {
            while (i<line.length && line[i].match(/[0-9A-Fa-fxb]/)) {
                s+=line[i];
                i++;
            }
            s+=' ';
        }
        // skip spaces
        while (i<line.length && line[i].match(/\s/)) i++;

        // accept the closing parenthesis
        if (line[i]==')') {
            s+=') ';
            i++;
        }
        
        // skip spaces
        while (i<line.length && line[i].match(/\s/)) i++;

        // if we don't have an operator next, we are done
        if (i>=line.length || !line[i].match(/[-+*\/%&|^]/)) break;
        s+=line[i]+' ';
        i++;

        // skip spaces
        while (i<line.length && line[i].match(/\s/)) i++;
    }
    // skip spaces
    while (i<line.length && line[i].match(/\s/)) i++;

    return [line.substring(i).trim(), s.trim()];
}


function buildStringFromTokens(tokens) {
    let depth = 0;
    let result = '';

    for (let token of tokens) {
        if (token === '(') {
            depth++;
        }

        if (depth > 0) {
            result += token + ' ';
        }

        if (token === ')') {
            depth--;
            if (depth === 0) {
                // Remove trailing space and return
                return result.trim();
            }
        }
    }

    // If we get here, there was no matching closing parenthesis
     die('No matching closing parenthesis');
    process.exit(1);
}



function setPC(value) {
    G.pc = value;
    G.symbols.set('$', G.pc);
}


