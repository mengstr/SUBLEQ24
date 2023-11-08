{ 
  signal: [
  { name: "clk",         wave: ".p..............." },
  { name: "Slot",        wave: "x2222222222222222x", data: ["t0", "t1", "t2", "t3","t4", "t5", "t6", "t7", "t8","t9", "t10", "t11", "t12","t13", "t14", "t15"] },
  { name: "Address",     wave: "x2.2.2.2....x....x", data: ["PC", "Aaddr", "PC", "Baddr" ]},
  { name: "Data",        wave: "x2.2.2.2....x....x", data: ["RAM0", "[RAM0]","RAM1", "[RAM1]" ]},
  {},
  { name: "PC_LD",      wave: "xl............hl.x" },
  { name: "PC_CK",      wave: "xlHl..Hl.......Hlx" },
  { name: "PC_E",       wave: "x1.0.1.0......10.x" },
  {},
  { name: "Aaddr_CK",   wave: "xlHl.............x" },
  { name: "Aaddr_E",    wave: "x0.1.0...........x" },
  { name: "Aval_CK",    wave: "xl..Hl...........x" },
  {},
  { name: "Baddr_CK",   wave: "xl....Hl.........x" },
  { name: "Baddr_E",    wave: "x0.....1....0....x" },
  { name: "Bval_CK",    wave: "x0......10.......x" },
  {},
  { name: "WR",         wave: "x0........hL.....x" },
  { name: "Result_E",   wave: "x0.......1..0....x" },
  

]}
