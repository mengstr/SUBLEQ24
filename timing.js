{ 
  config: { hscale: 1 },
  signal: [
  ['PC+0 (A)',
    {name: "CNT",     wave: "22222222222222222", data: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,"(16)"] },
    {name: "EnaPC",   wave: "1......0........1" },
    {name: "CkPC",    wave: "0......10........"},
    {name: "OeRAM",   wave: "1................"},
    {name: "OeSUB",   wave: "0................"},
    {name: "ABUS",    wave: "2......xx2.....x.",data:["PC+0","A"]},
    {name: "DBUS",    wave: "xxxx2..x.....2..x",data:["A","[A]"]},
    {name: "LatAA",   wave: "0....10.........."},
    {name: "EnaAA",   wave: "0.......1.......0"},
    {name: "LatAV",   wave: "0.............10."},
    {name: "CNT",     wave: "22222222222222222", data: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,"(16)"] },
  ],
  {},
  ['PC+1 (B)',
  {name: "CNT",     wave: "22222222222222222", data: [16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,"(32)"] },
  {name: "EnaPC",   wave: "1......0........." },
  {name: "CkPC",    wave: "0......10........"},
  {name: "OeRAM",   wave: "1...............0"},
  {name: "OeSUB",   wave: "0...............1"},
  {name: "ABUS",    wave: "2......xx2.......",data:["PC+1","B"]},
  {name: "DBUS",    wave: "xxxx2..x.....2..x",data:["B","[B]"]},
  {name: "LatBA",   wave: "0....10.........."},
  {name: "EnaBA",   wave: "0.......1........"},
  {name: "LatBV",   wave: "0.............10."}, 
  {name: "CNT",     wave: "22222222222222222", data: [16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,"(32)"] },
],
{},
['PC+2 (Writeback B)',
{name: "CNT",     wave: "22222222222222222", data: [32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,"..."] },
{name: "EnaPC",   wave: "0.......1........" },
{name: "CkPC",    wave: "0................"},
{name: "OeRAM",   wave: "0................"},
{name: "WeRAM",   wave: "0...1..0........."},
{name: "OeSUB",   wave: "1......0........."},
{name: "ABUS",    wave: "2......xx2.....x.",data:["B","C"]},
{name: "DBUS",    wave: "x2.....x.....2..x",data:["DIFF","[C]"]},
{name: "LatBA",   wave: "0................"},
{name: "EnaBA",   wave: "1......0........0"},
{name: "LatBV",   wave: "0.............10."}, 
{name: "CNT",     wave: "22222222222222222", data: [32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,"..."] },
],
{},

]}
