\


TS0 rising   - 
TS0 high     - Read A-address at PC from RAM
TS0 halftime -
TS0 high     - Read A-address at PC from RAM
TS0 falling  - Latch A-address into register

TS1 rising   -
TS1 high     - Read A-address at PC from RAM
TS1 halftime - Increment PC to B
TS1 high     - Read B-address at PC+1 from RAM
TS1 falling  - Latch B-address into register

TS2 rising   -
TS2 high     - Read A-value at A-address from RAM
TS2 halftime -
TS2 high     -
TS2 falling  - Latch A-value into register

TS3 rising   - 
TS3 high     - Read B-value at B-address from RAM
TS3 halftime - Increment PC to C
TS3 high     -
TS3 falling  - Latch B-value into register

TS4 rising   - RAM latches address for write cycle
TS4 high     -
TS4 halftime -
TS4 high     -
TS4 falling  -

TS5 rising   -
TS5 high     -
TS5 halftime -
TS5 high     -
TS5 falling  -
