// G.js

var pass=0;
var fileStack = [];         // Stack of file descriptors, also holds the current line number and the current line
var memory = [];            // Memory array
var symbols = new Map();    // Symbol table
var macros = new Map();     // Macro table
var macroname = '';         // Current macro name
var macroLines = [];        // FIFO queue of macro lines
var localNo = 0;            // Number for local labels
var procName = '';          // Current procedure name used for local labels
var pc = 0;                 // Program counter
var debug = false;          // Debug flag

module.exports = {
    fileStack,
    memory,
    symbols,
    macros,
    macroname,
    macroLines,
    localNo,
    procName,
    pc
};

