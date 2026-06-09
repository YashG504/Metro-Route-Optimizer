// ============================================================================
// METRO GRAPH — Dijkstra's Algorithm in JavaScript
// ============================================================================

class MinHeap {
  constructor() { this.heap = []; }
  push(item) { this.heap.push(item); this._up(this.heap.length - 1); }
  pop() {
    if (!this.heap.length) return null;
    const min = this.heap[0]; const last = this.heap.pop();
    if (this.heap.length) { this.heap[0] = last; this._down(0); }
    return min;
  }
  get size() { return this.heap.length; }
  _up(i) { while (i > 0) { const p = (i - 1) >> 1; if (this.heap[p].cost <= this.heap[i].cost) break; [this.heap[p], this.heap[i]] = [this.heap[i], this.heap[p]]; i = p; } }
  _down(i) { const n = this.heap.length; while (true) { let s = i, l = 2*i+1, r = 2*i+2; if (l < n && this.heap[l].cost < this.heap[s].cost) s = l; if (r < n && this.heap[r].cost < this.heap[s].cost) s = r; if (s === i) break; [this.heap[s], this.heap[i]] = [this.heap[i], this.heap[s]]; i = s; } }
}

class MetroGraph {
  constructor() { this.vertices = new Map(); this.journeyHistory = []; }

  addVertex(name) { if (!this.vertices.has(name)) this.vertices.set(name, new Map()); }
  addEdge(v1, v2, w) { if (!this.vertices.has(v1)||!this.vertices.has(v2)) return; this.vertices.get(v1).set(v2, w); this.vertices.get(v2).set(v1, w); }
  containsVertex(n) { return this.vertices.has(n); }
  get numVertices() { return this.vertices.size; }
  getStationList() { return Array.from(this.vertices.keys()); }

  hasPath(src, dst, visited = new Set()) {
    if (src === dst) return true;
    visited.add(src);
    const nb = this.vertices.get(src);
    if (!nb) return false;
    for (const [n] of nb) { if (!visited.has(n) && this.hasPath(n, dst, visited)) return true; }
    return false;
  }

  dijkstraWithPath(src, dst, timeMode = false) {
    const result = { cost: Infinity, path: [], found: false, edges: [] };
    const dist = new Map(), parent = new Map(), pq = new MinHeap();
    for (const [v] of this.vertices) { dist.set(v, Infinity); parent.set(v, null); }
    dist.set(src, 0); pq.push({ vname: src, cost: 0 });

    while (pq.size > 0) {
      const curr = pq.pop();
      if (curr.cost > dist.get(curr.vname)) continue;
      if (curr.vname === dst) { result.found = true; result.cost = curr.cost; break; }
      const nb = this.vertices.get(curr.vname);
      if (!nb) continue;
      for (const [nbr, w] of nb) {
        const nc = timeMode ? curr.cost + 120 + 40 * w : curr.cost + w;
        if (nc < dist.get(nbr)) { dist.set(nbr, nc); parent.set(nbr, curr.vname); pq.push({ vname: nbr, cost: nc }); }
      }
    }

    if (result.found) {
      let node = dst;
      while (node !== null) { result.path.unshift(node); node = parent.get(node); }
      for (let i = 0; i < result.path.length - 1; i++) {
        const f = result.path[i], t = result.path[i+1];
        result.edges.push({ from: f, to: t, weight: this.vertices.get(f).get(t) });
      }
    }
    return result;
  }

  getShortestDistance(src, dst) { return this.dijkstraWithPath(src, dst, false); }
  getShortestTime(src, dst) { const r = this.dijkstraWithPath(src, dst, true); if (r.found) r.timeMinutes = Math.ceil(r.cost / 60); return r; }

  calculateFare(n) { if (n <= 0) return 0; if (n <= 3) return 10; if (n <= 7) return 20; if (n <= 11) return 30; if (n <= 20) return 40; return 40 + (n-20)*10; }

  getFareInfo(src, dst, mode = 'distance') {
    const r = mode === 'distance' ? this.getShortestDistance(src, dst) : this.getShortestTime(src, dst);
    if (!r.found) return null;
    const ns = r.path.length - 1, fare = this.calculateFare(ns);
    const interchanges = this._countInterchanges(r.path);
    return { ...r, numStations: ns, fare, interchanges, fareBreakdown: this._slabs(ns) };
  }

  _slabs(s) { return [{range:'1-3',fare:10,active:s>=1&&s<=3},{range:'4-7',fare:20,active:s>=4&&s<=7},{range:'8-11',fare:30,active:s>=8&&s<=11},{range:'12-20',fare:40,active:s>=12&&s<=20},{range:'21+',fare:'40+',active:s>=21}]; }

  _countInterchanges(path) {
    let c = 0;
    for (let i = 1; i < path.length; i++) {
      const p = this._getLines(path[i-1]), curr = this._getLines(path[i]);
      if (!p.some(l => curr.includes(l))) c++;
    }
    return c;
  }

  _getLines(s) { const t = s.indexOf('~'); return t===-1 ? [] : s.substring(t+1).split(''); }

  searchStations(query) {
    if (!query) return [];
    const q = query.toLowerCase();
    return this.getStationList().filter(id => id.replace(/_/g,' ').split('~')[0].toLowerCase().includes(q));
  }

  logJourney(src, dst, result, mode) {
    const j = { id: Date.now(), source: src, destination: dst, distance: mode==='distance'?result.cost:null, time: mode==='time'?result.timeMinutes:null, fare: this.calculateFare(result.path.length-1), stations: result.path.length-1, mode, timestamp: new Date().toLocaleString(), path: result.path };
    this.journeyHistory.unshift(j);
    try { localStorage.setItem('metroJourneyHistory', JSON.stringify(this.journeyHistory.slice(0,50))); } catch(e) {}
    return j;
  }

  loadJourneyHistory() { try { const d = localStorage.getItem('metroJourneyHistory'); if (d) this.journeyHistory = JSON.parse(d); } catch(e) {} }
  clearJourneyHistory() { this.journeyHistory = []; try { localStorage.removeItem('metroJourneyHistory'); } catch(e) {} }

  static buildFromData(stations, edges) {
    const g = new MetroGraph();
    for (const id of Object.keys(stations)) g.addVertex(id);
    for (const [v1, v2, w] of edges) g.addEdge(v1, v2, w);
    g.loadJourneyHistory();
    return g;
  }
}

export { MetroGraph, MinHeap };
