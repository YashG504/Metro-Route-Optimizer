// ============================================================================
// APP — Main Application Controller
// ============================================================================
import { STATIONS, EDGES, METRO_LINES, TRAIN_TIMINGS } from './metro-data.js';
import { MetroGraph } from './metro-graph.js';
import { MetroMap } from './metro-map.js';
import { JourneyPlanner } from './journey-planner.js';

class App {
  constructor() {
    this.graph = null;
    this.metroMap = null;
    this.journeyPlanner = null;
    this.init();
  }

  init() {
    // Build graph
    this.graph = MetroGraph.buildFromData(STATIONS, EDGES);
    console.log(`✓ Graph built: ${this.graph.numVertices} stations loaded`);

    // Initialize map
    this.metroMap = new MetroMap('metro-map-container');

    // Initialize journey planner
    this.journeyPlanner = new JourneyPlanner(this.graph, this.metroMap);

    // Theme toggle
    this._setupTheme();

    // Stats
    this._renderStats();

    // Clock
    this._startClock();

    // Tab navigation
    this._setupTabs();

    console.log('✓ Metro Route Optimizer UI initialized');
  }

  _setupTheme() {
    const toggle = document.getElementById('theme-toggle');
    if (!toggle) return;
    const saved = localStorage.getItem('metroTheme');
    if (saved === 'light') document.body.classList.add('light-theme');
    toggle.addEventListener('click', () => {
      document.body.classList.toggle('light-theme');
      const isLight = document.body.classList.contains('light-theme');
      localStorage.setItem('metroTheme', isLight ? 'light' : 'dark');
      toggle.textContent = isLight ? '🌙' : '☀️';
    });
  }

  _renderStats() {
    const el = document.getElementById('metro-stats');
    if (!el) return;
    const now = new Date();
    const hours = now.getHours();
    const isPeak = (hours >= 8 && hours <= 10) || (hours >= 17 && hours <= 20);
    const freq = isPeak ? TRAIN_TIMINGS.frequency.peak : TRAIN_TIMINGS.frequency.offPeak;

    el.innerHTML = `
      <div class="stat-item"><span class="stat-value">${this.graph.numVertices}</span><span class="stat-label">Stations</span></div>
      <div class="stat-item"><span class="stat-value">2</span><span class="stat-label">Lines</span></div>
      <div class="stat-item"><span class="stat-value">${freq} min</span><span class="stat-label">Frequency</span></div>
      <div class="stat-item"><span class="stat-value">${isPeak ? '🔴 Peak' : '🟢 Off-Peak'}</span><span class="stat-label">Status</span></div>
    `;
  }

  _startClock() {
    const el = document.getElementById('live-clock');
    if (!el) return;
    const update = () => {
      const now = new Date();
      el.textContent = now.toLocaleTimeString('en-IN', { hour: '2-digit', minute: '2-digit', second: '2-digit' });
    };
    update();
    setInterval(update, 1000);
  }

  _setupTabs() {
    const tabs = document.querySelectorAll('.tab-btn');
    const panels = document.querySelectorAll('.tab-panel');
    tabs.forEach(tab => {
      tab.addEventListener('click', () => {
        tabs.forEach(t => t.classList.remove('active'));
        panels.forEach(p => p.classList.remove('active'));
        tab.classList.add('active');
        const target = document.getElementById(tab.dataset.tab);
        if (target) target.classList.add('active');
      });
    });
  }
}

// Launch
document.addEventListener('DOMContentLoaded', () => new App());
