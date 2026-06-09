// ============================================================================
// METRO DATA — Pune Metro Station definitions (Purple & Aqua Lines)
// ============================================================================

const METRO_LINES = {
  P: { name: 'Purple Line', color: '#9C27B0', gradient: ['#6A1B9A', '#CE93D8'] },
  A: { name: 'Aqua Line', color: '#00BCD4', gradient: ['#00838F', '#4DD0E1'] },
};

// Pune Metro Stations with SVG coordinates
// Purple Line runs top→bottom (PCMC to Swargate)
// Aqua Line runs left→right (Vanaz to Ramwadi)
// They intersect at Civil Court
const STATIONS = {
  // ===== PURPLE LINE (Line 1): PCMC → Swargate =====
  'PCMC~P': {
    displayName: 'PCMC', lines: ['P'],
    x: 35, y: 5, zone: 'Pimpri-Chinchwad',
    facilities: ['parking', 'elevator'],
  },
  'Sant_Tukaram_Nagar~P': {
    displayName: 'Sant Tukaram Nagar', lines: ['P'],
    x: 37, y: 11, zone: 'Pimpri-Chinchwad',
    facilities: ['parking'],
  },
  'Bhosari~P': {
    displayName: 'Bhosari', lines: ['P'],
    x: 39, y: 17, zone: 'Pimpri-Chinchwad',
    facilities: ['parking'],
  },
  'Kasarwadi~P': {
    displayName: 'Kasarwadi', lines: ['P'],
    x: 40, y: 23, zone: 'Pimpri-Chinchwad',
    facilities: [],
  },
  'Phugewadi~P': {
    displayName: 'Phugewadi', lines: ['P'],
    x: 41, y: 29, zone: 'Pune',
    facilities: [],
  },
  'Dapodi~P': {
    displayName: 'Dapodi', lines: ['P'],
    x: 42, y: 35, zone: 'Pune',
    facilities: ['parking'],
  },
  'Bopodi~P': {
    displayName: 'Bopodi', lines: ['P'],
    x: 43, y: 41, zone: 'Pune',
    facilities: [],
  },
  'Khadki~P': {
    displayName: 'Khadki', lines: ['P'],
    x: 44, y: 47, zone: 'Pune',
    facilities: ['parking'],
  },
  'Range_Hills~P': {
    displayName: 'Range Hills', lines: ['P'],
    x: 45, y: 52, zone: 'Pune',
    facilities: ['elevator'],
  },
  'Shivajinagar~P': {
    displayName: 'Shivajinagar', lines: ['P'],
    x: 46, y: 57, zone: 'Pune',
    facilities: ['parking', 'elevator'],
  },
  'Civil_Court~PA': {
    displayName: 'Civil Court', lines: ['P', 'A'],
    x: 48, y: 63, zone: 'Pune Central',
    facilities: ['interchange', 'elevator', 'washroom'],
  },
  'Budhwar_Peth~P': {
    displayName: 'Budhwar Peth', lines: ['P'],
    x: 47, y: 70, zone: 'Pune Central',
    facilities: [],
  },
  'Mandai~P': {
    displayName: 'Mandai', lines: ['P'],
    x: 46, y: 77, zone: 'Pune Central',
    facilities: [],
  },
  'Swargate~P': {
    displayName: 'Swargate', lines: ['P'],
    x: 45, y: 85, zone: 'Pune',
    facilities: ['parking', 'elevator', 'washroom'],
  },

  // ===== AQUA LINE (Line 2): Vanaz → Ramwadi =====
  'Vanaz~A': {
    displayName: 'Vanaz', lines: ['A'],
    x: 5, y: 60, zone: 'Pune West',
    facilities: ['parking', 'elevator'],
  },
  'Anand_Nagar~A': {
    displayName: 'Anand Nagar', lines: ['A'],
    x: 11, y: 60, zone: 'Pune West',
    facilities: [],
  },
  'Ideal_Colony~A': {
    displayName: 'Ideal Colony', lines: ['A'],
    x: 17, y: 61, zone: 'Pune West',
    facilities: [],
  },
  'Nal_Stop~A': {
    displayName: 'Nal Stop', lines: ['A'],
    x: 22, y: 61, zone: 'Pune',
    facilities: ['parking'],
  },
  'Garware_College~A': {
    displayName: 'Garware College', lines: ['A'],
    x: 28, y: 62, zone: 'Pune',
    facilities: [],
  },
  'Deccan_Gymkhana~A': {
    displayName: 'Deccan Gymkhana', lines: ['A'],
    x: 34, y: 62, zone: 'Pune',
    facilities: ['elevator'],
  },
  'Chhatrapati_Sambhaji_Udyan~A': {
    displayName: 'Sambhaji Udyan', lines: ['A'],
    x: 40, y: 63, zone: 'Pune Central',
    facilities: [],
  },
  'PMC~A': {
    displayName: 'PMC', lines: ['A'],
    x: 44, y: 63, zone: 'Pune Central',
    facilities: ['washroom'],
  },
  // Civil_Court~PA is already defined above (interchange)
  'Mangalwar_Peth~A': {
    displayName: 'Mangalwar Peth', lines: ['A'],
    x: 54, y: 63, zone: 'Pune Central',
    facilities: [],
  },
  'Pune_Railway_Station~A': {
    displayName: 'Pune Railway Station', lines: ['A'],
    x: 60, y: 62, zone: 'Pune',
    facilities: ['parking', 'elevator', 'washroom'],
  },
  'Ruby_Hall_Clinic~A': {
    displayName: 'Ruby Hall Clinic', lines: ['A'],
    x: 66, y: 61, zone: 'Pune',
    facilities: ['elevator'],
  },
  'Bund_Garden~A': {
    displayName: 'Bund Garden', lines: ['A'],
    x: 72, y: 60, zone: 'Pune East',
    facilities: ['parking'],
  },
  'Yerawada~A': {
    displayName: 'Yerawada', lines: ['A'],
    x: 78, y: 59, zone: 'Pune East',
    facilities: [],
  },
  'Kalyani_Nagar~A': {
    displayName: 'Kalyani Nagar', lines: ['A'],
    x: 85, y: 58, zone: 'Pune East',
    facilities: ['parking'],
  },
  'Ramwadi~A': {
    displayName: 'Ramwadi', lines: ['A'],
    x: 93, y: 57, zone: 'Pune East',
    facilities: ['parking', 'elevator'],
  },
};

// Edges: [station1, station2, distanceKm]
const EDGES = [
  // Purple Line
  ['PCMC~P', 'Sant_Tukaram_Nagar~P', 2],
  ['Sant_Tukaram_Nagar~P', 'Bhosari~P', 1],
  ['Bhosari~P', 'Kasarwadi~P', 2],
  ['Kasarwadi~P', 'Phugewadi~P', 2],
  ['Phugewadi~P', 'Dapodi~P', 1],
  ['Dapodi~P', 'Bopodi~P', 2],
  ['Bopodi~P', 'Khadki~P', 1],
  ['Khadki~P', 'Range_Hills~P', 1],
  ['Range_Hills~P', 'Shivajinagar~P', 2],
  ['Shivajinagar~P', 'Civil_Court~PA', 1],
  ['Civil_Court~PA', 'Budhwar_Peth~P', 1],
  ['Budhwar_Peth~P', 'Mandai~P', 1],
  ['Mandai~P', 'Swargate~P', 1],
  // Aqua Line
  ['Vanaz~A', 'Anand_Nagar~A', 1],
  ['Anand_Nagar~A', 'Ideal_Colony~A', 1],
  ['Ideal_Colony~A', 'Nal_Stop~A', 1],
  ['Nal_Stop~A', 'Garware_College~A', 1],
  ['Garware_College~A', 'Deccan_Gymkhana~A', 1],
  ['Deccan_Gymkhana~A', 'Chhatrapati_Sambhaji_Udyan~A', 1],
  ['Chhatrapati_Sambhaji_Udyan~A', 'PMC~A', 1],
  ['PMC~A', 'Civil_Court~PA', 1],
  ['Civil_Court~PA', 'Mangalwar_Peth~A', 1],
  ['Mangalwar_Peth~A', 'Pune_Railway_Station~A', 1],
  ['Pune_Railway_Station~A', 'Ruby_Hall_Clinic~A', 1],
  ['Ruby_Hall_Clinic~A', 'Bund_Garden~A', 1],
  ['Bund_Garden~A', 'Yerawada~A', 2],
  ['Yerawada~A', 'Kalyani_Nagar~A', 1],
  ['Kalyani_Nagar~A', 'Ramwadi~A', 2],
];

// Line route sequences for SVG line drawing
const LINE_ROUTES = {
  P: [
    'PCMC~P','Sant_Tukaram_Nagar~P','Bhosari~P','Kasarwadi~P','Phugewadi~P',
    'Dapodi~P','Bopodi~P','Khadki~P','Range_Hills~P','Shivajinagar~P',
    'Civil_Court~PA','Budhwar_Peth~P','Mandai~P','Swargate~P',
  ],
  A: [
    'Vanaz~A','Anand_Nagar~A','Ideal_Colony~A','Nal_Stop~A','Garware_College~A',
    'Deccan_Gymkhana~A','Chhatrapati_Sambhaji_Udyan~A','PMC~A','Civil_Court~PA',
    'Mangalwar_Peth~A','Pune_Railway_Station~A','Ruby_Hall_Clinic~A',
    'Bund_Garden~A','Yerawada~A','Kalyani_Nagar~A','Ramwadi~A',
  ],
};

const TRAIN_TIMINGS = {
  firstTrain: '06:00',
  lastTrain: '23:00',
  frequency: { peak: 5, offPeak: 10 },
  peakHours: [
    { start: '08:00', end: '10:00' },
    { start: '17:00', end: '20:00' },
  ],
};

export { METRO_LINES, STATIONS, EDGES, LINE_ROUTES, TRAIN_TIMINGS };
