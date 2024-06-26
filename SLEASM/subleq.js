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
var keyBuffer = [];

const keypressHandler = (str, k) => {
    if (k.ctrl && k.name === 'd') {
        let cnt=0;

        process.stdout.write("\r\n");
        process.stdout.write("       00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F  10  11  12  13  14  15  16  17  18  19  1A  1B  1C  1D  1E  1F\r\n");
        process.stdout.write("-------------------------------------------------------------------------------------------------------------------------------------");
        for (let i=0x7000; i<0x7200; i++) {
            if (cnt==0) {
                process.stdout.write("\r\n");
                process.stdout.write(`${i.toString(16)}: `);
            }
            if (memory[i]==undefined) memory[i]=0;
            process.stdout.write(`${(memory[i].toString(16)).padStart(3, '0')} `);
            cnt++;
            if (cnt==32) cnt=0;
        }
        process.stdout.write('\r\n');
    } else if (k.ctrl && k.name === 'c') {
        process.exit(0);
    } else {
        for (let i = 0; i < k.sequence.length; i++) {
            keyBuffer.push(k.sequence.charCodeAt(i));
        }
    }
};

// Function to get the next key from the buffer, waits for 10ms if a key has been pressed
async function getNextKey() {
    if (keyBuffer.length > 0) {
        await new Promise(resolve => setTimeout(resolve, 10));
        return keyBuffer.shift();
    } else {
        return NOKEY;
    }
}

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
            if (sleepCnt === 10000) {
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
            memory[b] = await getNextKey();
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
