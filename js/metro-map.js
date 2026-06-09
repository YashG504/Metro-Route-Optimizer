// ============================================================================
// METRO MAP — SVG Interactive Map Renderer (Pune Metro)
// ============================================================================
import { STATIONS, METRO_LINES, LINE_ROUTES } from './metro-data.js';

class MetroMap {
  constructor(containerId) {
    this.container = document.getElementById(containerId);
    this.svg = null;
    this.width = 1000;
    this.height = 600;
    this.stationElements = {};
    this.highlightedPath = null;
    this.selectedStation = null;
    this.onStationClick = null;
    this._init();
  }

  _init() {
    this.svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    this.svg.setAttribute('viewBox', `0 0 ${this.width} ${this.height}`);
    this.svg.setAttribute('class', 'metro-map-svg');
    this.svg.innerHTML = `
      <defs>
        <filter id="glow"><feGaussianBlur stdDeviation="3" result="blur"/><feMerge><feMergeNode in="blur"/><feMergeNode in="SourceGraphic"/></feMerge></filter>
        <filter id="glow-strong"><feGaussianBlur stdDeviation="6" result="blur"/><feMerge><feMergeNode in="blur"/><feMergeNode in="SourceGraphic"/></feMerge></filter>
        <filter id="shadow"><feDropShadow dx="0" dy="2" stdDeviation="3" flood-opacity="0.3"/></filter>
      </defs>
      <g class="map-lines"></g>
      <g class="map-highlight"></g>
      <g class="map-trains"></g>
      <g class="map-stations"></g>
      <g class="map-labels"></g>
    `;
    this.container.appendChild(this.svg);
    this._drawLines();
    this._drawStations();
    this._startTrainAnimations();
  }

  _coords(id) {
    const s = STATIONS[id];
    return s ? { x: (s.x / 100) * this.width, y: (s.y / 100) * this.height } : { x: 0, y: 0 };
  }

  _buildPath(stations) {
    let d = '';
    for (let i = 0; i < stations.length; i++) {
      const c = this._coords(stations[i]);
      d += i === 0 ? `M${c.x},${c.y}` : ` L${c.x},${c.y}`;
    }
    return d;
  }

  _drawLines() {
    const g = this.svg.querySelector('.map-lines');
    for (const [lineKey, stations] of Object.entries(LINE_ROUTES)) {
      const color = METRO_LINES[lineKey]?.color || '#888';
      const d = this._buildPath(stations);
      // Background glow
      const bg = document.createElementNS('http://www.w3.org/2000/svg', 'path');
      bg.setAttribute('d', d); bg.setAttribute('fill', 'none');
      bg.setAttribute('stroke', color); bg.setAttribute('stroke-width', '10');
      bg.setAttribute('stroke-opacity', '0.12'); bg.setAttribute('stroke-linecap', 'round');
      g.appendChild(bg);
      // Main line
      const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
      path.setAttribute('d', d); path.setAttribute('fill', 'none');
      path.setAttribute('stroke', color); path.setAttribute('stroke-width', '4');
      path.setAttribute('stroke-linecap', 'round'); path.setAttribute('stroke-linejoin', 'round');
      path.setAttribute('filter', 'url(#glow)');
      g.appendChild(path);
    }
  }

  _drawStations() {
    const gS = this.svg.querySelector('.map-stations');
    const gL = this.svg.querySelector('.map-labels');

    for (const [id, data] of Object.entries(STATIONS)) {
      const c = this._coords(id);
      const isInterchange = data.lines.length > 1;
      const r = isInterchange ? 9 : 6;
      const color = METRO_LINES[data.lines[0]]?.color || '#fff';

      // Pulse ring
      const pulse = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      pulse.setAttribute('cx', c.x); pulse.setAttribute('cy', c.y);
      pulse.setAttribute('r', r + 5); pulse.setAttribute('fill', color);
      pulse.setAttribute('opacity', '0'); pulse.setAttribute('class', 'station-pulse');
      gS.appendChild(pulse);

      // Interchange outer ring
      if (isInterchange) {
        const outer = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
        outer.setAttribute('cx', c.x); outer.setAttribute('cy', c.y);
        outer.setAttribute('r', r + 4); outer.setAttribute('fill', 'none');
        outer.setAttribute('stroke', '#fff'); outer.setAttribute('stroke-width', '2.5');
        outer.setAttribute('opacity', '0.7');
        gS.appendChild(outer);
        // Second color ring
        const ring2 = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
        ring2.setAttribute('cx', c.x); ring2.setAttribute('cy', c.y);
        ring2.setAttribute('r', r + 7); ring2.setAttribute('fill', 'none');
        ring2.setAttribute('stroke', METRO_LINES[data.lines[1]]?.color || '#fff');
        ring2.setAttribute('stroke-width', '2'); ring2.setAttribute('opacity', '0.5');
        gS.appendChild(ring2);
      }

      // Station dot
      const circle = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      circle.setAttribute('cx', c.x); circle.setAttribute('cy', c.y);
      circle.setAttribute('r', r); circle.setAttribute('fill', color);
      circle.setAttribute('stroke', '#fff'); circle.setAttribute('stroke-width', '2');
      circle.setAttribute('class', 'station-dot'); circle.setAttribute('filter', 'url(#shadow)');
      circle.dataset.stationId = id; circle.style.cursor = 'pointer';

      circle.addEventListener('mouseenter', () => {
        circle.setAttribute('r', r + 3); pulse.setAttribute('opacity', '0.3');
      });
      circle.addEventListener('mouseleave', () => {
        circle.setAttribute('r', r); pulse.setAttribute('opacity', '0');
      });
      circle.addEventListener('click', () => {
        if (this.onStationClick) this.onStationClick(id, data);
      });

      gS.appendChild(circle);
      this.stationElements[id] = { circle, pulse, coords: c, radius: r };

      // Label
      const text = document.createElementNS('http://www.w3.org/2000/svg', 'text');
      // Position labels: Purple line labels to the right, Aqua line labels above/below
      const isAquaOnly = data.lines.length === 1 && data.lines[0] === 'A';
      if (isAquaOnly) {
        text.setAttribute('x', c.x); text.setAttribute('y', c.y - r - 6);
        text.setAttribute('text-anchor', 'middle');
      } else if (isInterchange) {
        text.setAttribute('x', c.x + r + 8); text.setAttribute('y', c.y + 4);
        text.setAttribute('text-anchor', 'start');
      } else {
        text.setAttribute('x', c.x + r + 6); text.setAttribute('y', c.y + 4);
        text.setAttribute('text-anchor', 'start');
      }
      text.setAttribute('class', 'station-label');
      text.setAttribute('fill', 'rgba(255,255,255,0.75)'); text.setAttribute('font-size', '9');
      text.setAttribute('font-family', 'Inter, sans-serif');
      text.textContent = data.displayName;
      gL.appendChild(text);
    }
  }

  highlightRoute(path) {
    this.clearHighlight();
    if (!path || path.length < 2) return;
    const g = this.svg.querySelector('.map-highlight');
    const d = this._buildPath(path);

    // Glow bg
    const bg = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    bg.setAttribute('d', d); bg.setAttribute('fill', 'none');
    bg.setAttribute('stroke', '#00E5FF'); bg.setAttribute('stroke-width', '12');
    bg.setAttribute('stroke-opacity', '0.25'); bg.setAttribute('stroke-linecap', 'round');
    bg.setAttribute('filter', 'url(#glow-strong)'); bg.setAttribute('class', 'route-highlight');
    g.appendChild(bg);

    // Main highlight
    const line = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    line.setAttribute('d', d); line.setAttribute('fill', 'none');
    line.setAttribute('stroke', '#00E5FF'); line.setAttribute('stroke-width', '5');
    line.setAttribute('stroke-linecap', 'round'); line.setAttribute('class', 'route-highlight');
    line.setAttribute('filter', 'url(#glow)');
    line.style.strokeDasharray = '2000'; line.style.strokeDashoffset = '2000';
    line.style.animation = 'route-draw 1.5s ease forwards';
    g.appendChild(line);

    // Highlight dots
    for (const stId of path) {
      const el = this.stationElements[stId];
      if (el) {
        el.circle.setAttribute('fill', '#00E5FF'); el.circle.setAttribute('r', '9');
        el.pulse.setAttribute('opacity', '0.4'); el.pulse.setAttribute('fill', '#00E5FF');
      }
    }
    this.highlightedPath = path;
  }

  clearHighlight() {
    this.svg.querySelector('.map-highlight').innerHTML = '';
    if (this.highlightedPath) {
      for (const stId of this.highlightedPath) {
        const el = this.stationElements[stId];
        if (el) {
          const data = STATIONS[stId];
          const color = METRO_LINES[data.lines[0]]?.color || '#fff';
          el.circle.setAttribute('fill', color);
          el.circle.setAttribute('r', el.radius);
          el.pulse.setAttribute('opacity', '0');
        }
      }
    }
    this.highlightedPath = null;
  }

  _startTrainAnimations() {
    const trainGroup = this.svg.querySelector('.map-trains');
    const routes = [
      { line: 'P', stations: LINE_ROUTES.P, speed: 20000 },
      { line: 'A', stations: LINE_ROUTES.A, speed: 22000 },
    ];

    for (const route of routes) {
      const color = METRO_LINES[route.line]?.color || '#fff';
      // Build round-trip path
      let pathD = this._buildPath(route.stations);
      for (let i = route.stations.length - 2; i >= 0; i--) {
        const c = this._coords(route.stations[i]);
        pathD += ` L${c.x},${c.y}`;
      }

      // Train indicator
      const train = document.createElementNS('http://www.w3.org/2000/svg', 'g');
      train.setAttribute('class', 'train-indicator');
      const rect = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
      rect.setAttribute('x', '-12'); rect.setAttribute('y', '-7');
      rect.setAttribute('width', '24'); rect.setAttribute('height', '14');
      rect.setAttribute('rx', '5'); rect.setAttribute('fill', color);
      rect.setAttribute('filter', 'url(#glow)');
      const dot = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      dot.setAttribute('cx', '0'); dot.setAttribute('cy', '0');
      dot.setAttribute('r', '3'); dot.setAttribute('fill', '#fff');
      train.appendChild(rect); train.appendChild(dot);

      const anim = document.createElementNS('http://www.w3.org/2000/svg', 'animateMotion');
      anim.setAttribute('dur', `${route.speed}ms`); anim.setAttribute('repeatCount', 'indefinite');
      anim.setAttribute('path', pathD);
      train.appendChild(anim);
      trainGroup.appendChild(train);

      // Second train (offset)
      const train2 = train.cloneNode(true);
      const anim2 = train2.querySelector('animateMotion');
      anim2.setAttribute('begin', `${route.speed / 2}ms`);
      trainGroup.appendChild(train2);
    }
  }
}

export { MetroMap };
