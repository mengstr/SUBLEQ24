# SUBLEQ24


https://hasith.vidanamadura.net/projects/subleq/
https://www.sccs.swarthmore.edu/users/07/mustpaks/oiscdoc/compileOISC.html
https://github.com/mustpax/OISC?tab=readme-ov-file


```
-------------------------
FLASH ADDRESS SCRAMBLING
-------------------------
          0x5555  0x2AAA
-------------------------
A0   A7     1     0     
A1   A6     0     1
A2   A5     1     0
A3   A4     0     1

A4   A3     1     0
A5   A2     0     1
A6   A1     1     0
A7   A0     0     1

A8   A11    1     0
A9   A10    0     1
A10  A8     1     0
A11  A9     0     1

A12  A17    1     0
A13  A12    0     1
A14  A13    1     0
A15  A16    0     0

A16  A15    0     0
A17  A14    0     0

0x5555 -> 10 0010 1001 1010 1010 = 0x229AA
0x2AAA -> 00 0001 0110 0101 0101 = 0x01655
```




```
000000-1FFFFF    RAM#0 (2M) 
200000-3FFFFF    RAM#1 (2M) 
400000-5FFFFF    RAM#2 (2M)
600000-7FFFFF    EPROM (2M)
    600000-63FFFF    EPROM#0 (256K)
    640000-67FFFF    EPROM#1 (256K)
    680000-6BFFFF    EPROM#2 (256K)
    6C0000-6FFFFF    EPROM#3 (256K)
    700000-73FFFF    EPROM#4 (256K)
    740000-77FFFF    EPROM#5 (256K)
    780000-7BFFFF    EPROM#6 (256K)
    7C0000-7FFFFF    EPROM#7 (256K)
800000-9FFFFF    Unused (2M)
A00000-BFFFFF    Unused (2M)
C00000-DFFFFF    Unused (2M)
E00000-FFFFFF    IO     (2M)
    E00000-EFFFFF    Unused (1M)
    F00000-F0FFFF    Unused (64K)
    F10000-F1FFFF    Unused (64K)
    F20000-F2FFFF    Unused (64K)
    F30000-F3FFFF    Unused (64K)
    F40000-F4FFFF    Unused (64K)
    F50000-F5FFFF    Unused (64K)
    F60000-F6FFFF    Unused (64K)
    F70000-F7FFFF    Unused (64K)
    F80000-F8FFFF    Unused (64K)
    F90000-F9FFFF    Unused (64K)
    FA0000-FAFFFF    Unused (64K)
    FB0000-FBFFFF    Unused (64K)
    FC0000-FCFFFF    Unused (64K)
    FD0000-FDFFFF    Unused (64K)
    FE0000-FEFFFF    Unused (64K)
    FF0000-FFFFFF    Unused (64K)
```



## PCB PLACEMENT
```
                    ADDR  MISC  DATA
        PC          | |  -----  | |      A-ADDR/VALU
+---------------+   | |   | |   | |   +---------------+
|           OUT ====* |   | *========== OUTVALUE      |
|               |   | |   | |   | |   |               |
|              .|   | *================ OUTADDR       |
|               |   | |   | |   | |   |               |
|            IN ================* *==== IN            |
+---------------+   | |   | |   | |   +---------------+
                    | |   | |   | |                           
     SERIAL         | |   | |   | |       SUBTRACT
+---------------+   | |   | |   | |   +---------------+
|          DATA ================* *==== OUT           |
|               |   | |   | |   | |   |               |
|              .|   | |   | *========== IN2           |
|               |   | |  -----  | |   |               |
|              .|   | |   | *========== IN1           |
+---------------+   | |   | |   | |   +---------------+
                    | |   | |   | |                            
      MEMORY        | |   | |   | |      B-ADDR/VALUE
+---------------+   | |   | |   | |   +---------------+
|              .|   | |   | *========== OUTVALUE      |
|               |   | |   | |   | |   |               |
|       ADDRESS ====* *================ OUTADDR       |
|               |   | |   | |   | |   |               |
|          DATA ================* *==== IN            |
+---------------+   | |   | |   | |   +---------------+
                    | |  -----  | |                            
  INTERRUPT-PC      | |   | |   | |    PROTO-MICROCODE 
+---------------+   | |   | |   | |   +---------------+
|           OUT ====* |   | |   | |   |.              |
|               |   | |   | |   | |   |               |
|              .|   | |   | *========== TIMESTEP IN   |
|               |   | |   | |   | |   |               |
|            IN ================* |   |.              |
+---------------+   | |   | |   | |   +---------------+
                    | |   | |   | |                            
                    | |   | |   | |     PROTO-SEQENCER
+---------------+   | |   | |   | |   +---------------+
|              .|   | |   | |   | |   |.              |
|               |   | |   | |   | |   |               |
|              .|   | |   | *========== TIMESTEP OUT  |
|               |   | |   | |   | |   |               |
|              .|   | |   | |   | |   |.              |
+---------------+   | |   | |   | |   +---------------+
                    | |  -----  | |
```

......

```

                157 163 541 540 283 27  30  00  245 574 21  RAM FLSH
MEMORY                                          6           3   3
REGISTER1               6                           6   4
REGISTER2               6                           6   4
SUBTRACT                    3   6   3   1   1   3
PROGRAMCOUNTER  6   6   3
=====================================================================
                6   6   15  3   6   3   1   1   9   12  8   3   3


                74  00  14  163 238 555 4075
PROTO-SEQ/CNTRL 4   1   2   1   2   2   1
```
