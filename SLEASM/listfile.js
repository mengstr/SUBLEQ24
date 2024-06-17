'use strict';

const fs = require('fs');
const G = require('./G.js');
const { logger,die } = require('./logger.js');

function WriteInfoToListfile(fdListfile) {
    // Create a file having the same name as the input with extension .lst
    // and write the symbol table to the file

    // Find the maximum length of the symbol names
    let maxSymbolLength = 0;
    for (let key of G.symbols.keys()) {
        maxSymbolLength = Math.max(maxSymbolLength, key.length);
    }

    fs.writeSync(fdListfile, '------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, 'Symbol table - ordered by name:\n');
    fs.writeSync(fdListfile, '------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, '\n');
    
    // Convert the symbols Map to an array of [key, value] pairs
    let symbolsArray = Array.from(G.symbols);
    
    // Sort the array by the keys (i.e., the first element of each pair)
    symbolsArray.sort((a, b) => a[0].localeCompare(b[0]));
    
    // Iterate over the sorted array and write each key-value pair to the file
    for (let [key, value] of symbolsArray) {
        fs.writeSync(fdListfile, `${key.padEnd(maxSymbolLength)} = ${value} (0x${value.toString(16)})\n`);
    }
    fs.writeSync(fdListfile, '\n\n');

    fs.writeSync(fdListfile, '------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, 'Symbol table - ordered by value:\n');
    fs.writeSync(fdListfile, '------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, '\n');

    // Sort the array by the values (i.e., the second element of each pair)
    symbolsArray.sort((a, b) => a[1] - b[1]);
    // Iterate over the sorted array and write each key-value pair to the file
    for (let [key, value] of symbolsArray) {
        fs.writeSync(fdListfile, `${key.padEnd(maxSymbolLength)} = ${value} (0x${value.toString(16)})\n`);
    }
    fs.writeSync(fdListfile, '\n\n');

    // Dump the memory array to the file, 16 bytes per line. Each value is written as a 6-digit hexadecimal number
    fs.writeSync(fdListfile, '-------------------------------------------------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, 'Memory dump:\n');
    fs.writeSync(fdListfile, '-------------------------------------------------------------------------------------------------------------------------\n');
    fs.writeSync(fdListfile, '\n');

    for (let i = 0; i < G.memory.length; i += 16) {
        let line = `${i.toString(16).padStart(6, '0')}: `;
        for (let j = 0; j < 16; j++) {
            if (j == 8) line += '  ';
            if (i + j < G.memory.length) {
                line += (G.memory[i + j] !== undefined ? ((G.memory[i + j] << 8) >>> 8).toString(16).padStart(6, '0') : '      ') + ' ';
            }
        }
        // Don't output lines with just the address part
        if (line.trim().length > 8) {
            fs.writeSync(fdListfile, line + '\n');
        }
    }
    fs.writeSync(fdListfile, '\n\n');
}
    
    
function WriteV20RawFile(fdV20RawFile) {
    fs.writeSync(fdV20RawFile, "v2.0 raw\n");
    for (let i = 0; i < G.memory.length; i++) {
        fs.writeSync(fdV20RawFile, G.memory[i] !== undefined ? ((G.memory[i] << 8) >>> 8).toString(16)+ '\n' : '0\n');
    }   
    logger.info(`Generated ${G.memory.length} triple-bytes`);
}
            
            
            exports.WriteInfoToListfile = WriteInfoToListfile;
            exports.WriteV20RawFile = WriteV20RawFile;
            