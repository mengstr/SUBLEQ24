'use strict';

const G = require('./G.js');

let currentLogLevel = 0;

const logger = {
    setLogLevel: (logLevel) => {
        currentLogLevel = logLevel;
    },
    info: (message) => {
        if (currentLogLevel >= 1) {
            console.log(`\x1b[34m${message}\x1b[0m`); // Blue text
        }
    },
    debug: (message) => {
        if (currentLogLevel >= 2) {
            console.log(`\x1b[32m${message}\x1b[0m`); // Green text
        }
    },
    verbose: (message) => {
        if (currentLogLevel >= 3) {
            console.log(`\x1b[33m${message}\x1b[0m`); // Yellow text
        }
    }
};


function die(msg) {
    let { filename, line, lineno } = G.fileStack[G.fileStack.length - 1];
    console.log()
    console.log(line)
    console.log(`Error at ${filename}:${lineno} - ${msg}`);
    console.error(new Error('Stack trace').stack);
    process.exit(1);
    process.exit(1);
}

exports.logger = logger;
exports.die = die;
