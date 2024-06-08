{ 
  signal: [
    { name: "clk",     wave: "p......"},
    { name: "PcE",     wave: "1010.10"},
    { name: "RD",      wave: "1...010"},
    { name: "WR",      wave: "0...10."},
    { name: "MarCk",   wave: "01010.."},
    { name: "AnegCk",     wave: "010...."},
    { name: "AvalCk",  wave: "0.10..."},
    { name: "BnegCk",     wave: "0..10.."},
    { name: "BvalCk",  wave: "0...10."},
    { name: "PCck",    wave: "01010.1"},
    { name: "ResultE", wave: "0...10."},
    { name: "MarE",    wave: "0101.0."},
    { name: "PcLd",    wave: "0....10"},
    ],
    head:{
      text:'SUBLEQ24 Timing Diagram',
      tick:0,
    },  
  }


  // TS0 Get A address
  //        Output PC (pointing to A) to address bus
  //        Enable RAM read
  // Transition TS0->TS1
  //        Latch value from data bus into MAR register
  //        Latch Databus bit 23 into AisNeg register
  //        Increment PC (Now points to B address)
  //
  // TS1 Get value from [A]
  //        Stop outputting PC to address bus
  //        Output MAR to address bus
  // Transition TS1->TS2
  //        Latch value from data bus into Subtractor A register
  //
  // TS2 Get B address
  //        Output PC (pointing to B) to address bus
  // Transition TS2->TS3
  //        Latch value from data bus into MAR register
  //        Latch Databus bit 23 into BisNeg register
  //        Increment PC (Now points to C address)
  //
  // TS3 Get value from [B]
  //        Stop outputting PC to address bus
  //        Output MAR to address bus
  // Transition TS3->TS4
  //        Latch value from data bus into Subtractor B register
  //        Stop reading from RAM and enable the WR signal
  //
  // TS4 - Subtract and store result
  //        Output the subtraction result to the data bus
  //        Continue to output MAR (B-address) to the address bus
  // Transition TS4->TS5
  //       Store the subtraction into RAM by de-asserting WR
  //
  // TS5 - Get C address
  //      Output PC (pointing to C) to address bus
  //      Read from RAM
  //      If isLEQ then enable PC load signal
  // Transition TS5->TS6
  //      Increment or Load PC based on result

  // TS6 - Set PC to C if result is negative or zero
  //       (happens at the TS5->TS6 transition)
   
