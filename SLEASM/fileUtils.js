'use strict';

const G = require('./G.js');
const fs = require('fs');
const { logger, die } = require('./logger');


// Open a file and push the file descriptor with name and line number to the stack
function openFileAndPushToStack(filename) {
    logger.info(`Processing file ${filename}`);
    try {
        G.fileStack.push({
            filename: filename,
            fd: fs.openSync(filename, 'r'),
            lineno: 0,
        });
    } catch (err) {
        die(`Can't open file ${filename}`);
    }
}

// Read a line from the file descriptor, return null if EOF
readLine.buffers = {};
const blocksize = 8192;

function readLine(fd) {
    if (!readLine.buffers[fd] || readLine.buffers[fd].length === 0) {
        const block = Buffer.alloc(blocksize);
        const bytesRead = fs.readSync(fd, block, 0, blocksize);
        if (bytesRead === 0) {
            return null; // EOF
        }
        readLine.buffers[fd] = block.slice(0, bytesRead).toString().split('\n');
    }
    let line = readLine.buffers[fd].shift() || '';
    while (readLine.buffers[fd].length === 0) {
        const block = Buffer.alloc(blocksize);
        const bytesRead = fs.readSync(fd, block, 0, blocksize);
        if (bytesRead === 0) {
            break; // EOF
        }
        const newLines = block.slice(0, bytesRead).toString().split('\n');
        line += newLines.shift() || '';
        readLine.buffers[fd] = newLines;
    }

    if (G.debug) {
        if (line.includes(';debug')) {
            if (line[0] === ';') {
                line = line.slice(1);
            }
        }
    }

    return line;
}

exports.readLine = readLine;
exports.openFileAndPushToStack = openFileAndPushToStack;

