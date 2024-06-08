'use strict';

const G = require('./G.js');

// Function to strip comments from a string
function stripComment(arg) {
    let insideQuotes = false;
    let s = '';
    for (let i = 0; i < arg.length; i++) {
        if (arg[i] === '"') {
            insideQuotes = !insideQuotes;
        }
        if (arg[i] === ';' && !insideQuotes) {
            break;
        }
        s += arg[i];
    }
    
    // Return new string with trailing whitespace removed
    return s.replace(/\s+$/, '');
}


// Split line on commas, taking care of quoted strings
function splitAtCommas(line) {
    let values = [];
    let value = '';
    let inQuotes = false;

    for (let i = 0; i < line.length; i++) {
        if (line[i] === '"') {
            inQuotes = !inQuotes;
        }

        if (!inQuotes && line[i] === ',') {
            values.push(value.trim());
            value = '';
        } else {
            value += line[i];
        }
    }
    if (value.trim() !== '') values.push(value.trim());

    return values;
}


// convert tabs to spaces in a string, preserving column alignment using settable tab stops as a parameter
function expandTabs(s, tabStops) {
    let t = '';
    let col = 0;
    for (let i = 0; i < s.length; i++) {
        if (s[i] === '\t') {
            let tabStop = tabStops - col % tabStops;
            t += ' '.repeat(tabStop);
            col += tabStop;
        } else {
            t += s[i];
            col++;
        }
    }
    return t;
}

function parseNumber(str) {
    str = str.trim().toLowerCase();

    if (str.startsWith('0x')) {                     // Hexadecimal
        return parseInt(str.substring(2), 16);
    } else if (str.startsWith('0b')) {              // Binary
        return parseInt(str.substring(2), 2);
    } else {                                        // Decimal
        return parseInt(str, 10);
    }
}


exports.stripComment = stripComment;
exports.splitAtCommas = splitAtCommas;
exports.expandTabs = expandTabs;
exports.parseNumber = parseNumber;
