#!/usr/bin/env node
'use strict';

const fs = require('fs');
const readline = require('readline');

const NOKEY = -1;
var key=NOKEY;

var memory;
var pc=0;
var trace=false;
var silent=false;

const keypressHandler = (str, k) => {
    if (k.ctrl && k.name === 'c') {
        process.exit(0);
    } else {
        key = k.sequence.charCodeAt(0);
    }
};

if (process.stdin.setRawMode) {
    readline.emitKeypressEvents(process.stdin);
    process.stdin.setRawMode(true);
    process.stdin.on('keypress', keypressHandler);
}

// Get the file name from the command line arguments
const fileName = process.argv[2];
if  (process.argv[3]=="-t") trace=true;
if  (process.argv[3]=="-q") silent=true;


try {
    const data = fs.readFileSync(fileName, 'utf8');
    memory = data.split('\n')
    .filter(line => line.trim() !== '')
    .slice(1)
    .map(line => {
        let num = parseInt(line, 16);
        num = num << 8 >> 8; // Convert to 24-bit signed integer
        return num;
    });
    if (!silent) console.log(`Loaded ${memory.length} instructions from ${fileName}`);
} catch (err) {
    console.error(`Error reading file from disk: ${err}`);
}

run();

async function run() {
    let sleepCnt = 0;
    while (pc>=0 && pc<memory.length) {

        // Delay 1 ms for every 1000 instructions to allow for handlng of key input
        await (async () => {
            sleepCnt++;
            if (sleepCnt === 1000) {
                sleepCnt = 0;
                return new Promise(resolve => setTimeout(resolve, 1));
            }
        })();

        let a = memory[pc];
        let b = memory[pc+1];
        let c = memory[pc+2];
        
        if (a==-1){
            if (trace) console.log(`pc=${pc.toString(16)} IN to location ${b.toString(16)} key=${key.toString(16)}`);
            pc += 3;
            memory[b] = key;
            key = NOKEY;
            continue;
        }
        
        if (b==-1){
            if (trace) console.log(`pc=${pc.toString(16)} OUT from location ${a.toString(16)} char=${memory[a].toString(16)}`);
            pc += 3;
            process.stdout.write(String.fromCharCode(memory[a]));
            continue;
        }
        
    let newB = (memory[b] - memory[a]) & 0xFFFFFF;
    if (newB & 0x800000) newB = -(0x1000000 - newB);
                
        if (trace) console.log(`pc=${pc.toString(16)} a=${a.toString(16)} [${memory[a].toString(16)}]  b=${b.toString(16)} [${memory[b].toString(16)}] c=${c.toString(16)} newB=${newB.toString(16)}`);

        memory[b]=newB;
        if (memory[b] <= 0) pc = c; else pc += 3;
    }

    process.stdin.pause();
    if (!silent) console.log("\nDone");
}
