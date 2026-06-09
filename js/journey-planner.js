// ============================================================================
// JOURNEY PLANNER — UI Logic for route planning and results display
// ============================================================================
import { STATIONS, METRO_LINES } from './metro-data.js';

class JourneyPlanner {
  constructor(graph, metroMap) {
    this.graph = graph;
    this.metroMap = metroMap;
    this.currentResult = null;
    this._bindElements();
    this._setupAutocomplete();
    this._setupEventListeners();
  }

  _bindElements() {
    this.srcInput = document.getElementById('source-input');
    this.dstInput = document.getElementById('dest-input');
    this.srcDropdown = document.getElementById('source-dropdown');
    this.dstDropdown = document.getElementById('dest-dropdown');
    this.findBtn = document.getElementById('find-route-btn');
    this.swapBtn = document.getElementById('swap-btn');
    this.modeToggle = document.getElementById('mode-toggle');
    this.resultsPanel = document.getElementById('results-panel');
    this.routeSteps = document.getElementById('route-steps');
    this.historyList = document.getElementById('history-list');
    this.clearHistoryBtn = document.getElementById('clear-history-btn');
    this.stationInfoPanel = document.getElementById('station-info-panel');
  }

  _setupAutocomplete() {
    const setup = (input, dropdown, hiddenId) => {
      input.addEventListener('input', () => {
        const q = input.value.trim();
        if (q.length < 1) { dropdown.classList.remove('active'); return; }
        const results = this.graph.searchStations(q);
        dropdown.innerHTML = '';
        if (results.length === 0) { dropdown.classList.remove('active'); return; }
        for (const id of results.slice(0, 8)) {
          const data = STATIONS[id];
          const item = document.createElement('div');
          item.className = 'dropdown-item';
          const lineColors = (data?.lines || []).map(l => `<span class="line-badge" style="background:${METRO_LINES[l]?.color||'#888'}">${l}</span>`).join('');
          item.innerHTML = `<span class="station-name">${data?.displayName || id}</span>${lineColors}`;
          item.addEventListener('click', () => {
            input.value = data?.displayName || id;
            input.dataset.stationId = id;
            dropdown.classList.remove('active');
          });
          dropdown.appendChild(item);
        }
        dropdown.classList.add('active');
      });
      input.addEventListener('focus', () => { if (dropdown.children.length) dropdown.classList.add('active'); });
      document.addEventListener('click', (e) => { if (!input.contains(e.target) && !dropdown.contains(e.target)) dropdown.classList.remove('active'); });
    };
    setup(this.srcInput, this.srcDropdown);
    setup(this.dstInput, this.dstDropdown);
  }

  _setupEventListeners() {
    this.findBtn.addEventListener('click', () => this.findRoute());
    this.swapBtn.addEventListener('click', () => this.swapStations());
    if (this.clearHistoryBtn) this.clearHistoryBtn.addEventListener('click', () => this.clearHistory());

    // Station click on map
    this.metroMap.onStationClick = (id, data) => {
      this._showStationInfo(id, data);
      // Auto-fill source or destination
      if (!this.srcInput.dataset.stationId) {
        this.srcInput.value = data.displayName;
        this.srcInput.dataset.stationId = id;
      } else if (!this.dstInput.dataset.stationId) {
        this.dstInput.value = data.displayName;
        this.dstInput.dataset.stationId = id;
      } else {
        this.srcInput.value = this.dstInput.value;
        this.srcInput.dataset.stationId = this.dstInput.dataset.stationId;
        this.dstInput.value = data.displayName;
        this.dstInput.dataset.stationId = id;
      }
    };

    this._renderHistory();
  }

  findRoute() {
    const srcId = this.srcInput.dataset.stationId;
    const dstId = this.dstInput.dataset.stationId;
    if (!srcId || !dstId) {
      this._showError('Please select both source and destination stations');
      return;
    }
    if (srcId === dstId) {
      this._showError('Source and destination cannot be the same');
      return;
    }
    if (!this.graph.containsVertex(srcId) || !this.graph.containsVertex(dstId)) {
      this._showError('Invalid station selected');
      return;
    }

    const mode = this.modeToggle?.checked ? 'time' : 'distance';
    const fareInfo = this.graph.getFareInfo(srcId, dstId, mode);
    if (!fareInfo) {
      this._showError('No route found between these stations');
      return;
    }

    // Get both distance and time for full info
    const distResult = this.graph.getShortestDistance(srcId, dstId);
    const timeResult = this.graph.getShortestTime(srcId, dstId);

    this.currentResult = {
      ...fareInfo,
      distanceKm: distResult.cost,
      timeMinutes: timeResult.timeMinutes,
    };

    this._renderResults(this.currentResult);
    this.metroMap.highlightRoute(fareInfo.path);
    this.graph.logJourney(srcId, dstId, fareInfo, mode);
    this._renderHistory();
  }

  swapStations() {
    const tmpVal = this.srcInput.value;
    const tmpId = this.srcInput.dataset.stationId;
    this.srcInput.value = this.dstInput.value;
    this.srcInput.dataset.stationId = this.dstInput.dataset.stationId || '';
    this.dstInput.value = tmpVal;
    this.dstInput.dataset.stationId = tmpId || '';
  }

  _renderResults(result) {
    this.resultsPanel.classList.add('active');
    this.resultsPanel.innerHTML = `
      <div class="results-header">
        <h3>🚇 Route Found</h3>
        <button class="close-results" id="close-results">✕</button>
      </div>
      <div class="result-cards">
        <div class="result-card distance-card">
          <div class="card-icon">📏</div>
          <div class="card-value">${result.distanceKm} km</div>
          <div class="card-label">Distance</div>
        </div>
        <div class="result-card time-card">
          <div class="card-icon">⏱️</div>
          <div class="card-value">${result.timeMinutes} min</div>
          <div class="card-label">Travel Time</div>
        </div>
        <div class="result-card fare-card">
          <div class="card-icon">💰</div>
          <div class="card-value">₹${result.fare}</div>
          <div class="card-label">Fare</div>
        </div>
        <div class="result-card stations-card">
          <div class="card-icon">🚉</div>
          <div class="card-value">${result.numStations}</div>
          <div class="card-label">Stations</div>
        </div>
      </div>
      ${result.interchanges > 0 ? `<div class="interchange-badge">🔄 ${result.interchanges} interchange${result.interchanges > 1 ? 's' : ''}</div>` : ''}
      <div class="route-path">
        <h4>Route Path</h4>
        <div class="route-steps" id="route-steps-inner"></div>
      </div>
      <div class="fare-slabs">
        <h4>Fare Slabs</h4>
        <div class="slab-list">
          ${result.fareBreakdown.map(s => `<div class="slab ${s.active ? 'active' : ''}"><span class="slab-range">${s.range}</span><span class="slab-fare">₹${s.fare}</span></div>`).join('')}
        </div>
      </div>
    `;

    // Render route steps
    const stepsContainer = document.getElementById('route-steps-inner');
    for (let i = 0; i < result.path.length; i++) {
      const stId = result.path[i];
      const data = STATIONS[stId];
      const lines = data?.lines || [];
      const primaryColor = METRO_LINES[lines[0]]?.color || '#888';
      const isInterchange = lines.length > 1;
      const step = document.createElement('div');
      step.className = `route-step ${i === 0 ? 'first' : ''} ${i === result.path.length - 1 ? 'last' : ''} ${isInterchange ? 'interchange' : ''}`;
      step.innerHTML = `
        <div class="step-dot" style="background:${primaryColor};box-shadow:0 0 8px ${primaryColor}"></div>
        <div class="step-line" style="background:${i < result.path.length - 1 ? primaryColor : 'transparent'}"></div>
        <div class="step-info">
          <span class="step-name">${data?.displayName || stId}</span>
          ${lines.map(l => `<span class="line-badge-sm" style="background:${METRO_LINES[l]?.color||'#888'}">${METRO_LINES[l]?.name||l}</span>`).join('')}
          ${isInterchange ? '<span class="interchange-tag">⇆ Interchange</span>' : ''}
        </div>
      `;
      step.style.animationDelay = `${i * 0.08}s`;
      stepsContainer.appendChild(step);
    }

    document.getElementById('close-results').addEventListener('click', () => {
      this.resultsPanel.classList.remove('active');
      this.metroMap.clearHighlight();
    });
  }

  _showStationInfo(id, data) {
    if (!this.stationInfoPanel) return;
    const lines = data.lines || [];
    const neighbors = this.graph.vertices.get(id);
    let neighborHTML = '';
    if (neighbors) {
      for (const [nId, dist] of neighbors) {
        const nData = STATIONS[nId];
        neighborHTML += `<div class="neighbor-item"><span>${nData?.displayName || nId}</span><span class="nbr-dist">${dist} km</span></div>`;
      }
    }
    this.stationInfoPanel.classList.add('active');
    this.stationInfoPanel.innerHTML = `
      <div class="station-info-header">
        <h3>${data.displayName}</h3>
        <button class="close-info" id="close-station-info">✕</button>
      </div>
      <div class="station-lines">${lines.map(l => `<span class="line-badge" style="background:${METRO_LINES[l]?.color||'#888'}">${METRO_LINES[l]?.name||l}</span>`).join('')}</div>
      <div class="station-zone">📍 ${data.zone || 'Unknown'}</div>
      <div class="station-facilities">${(data.facilities||[]).map(f => `<span class="facility-tag">${{parking:'🅿️ Parking',elevator:'♿ Elevator',washroom:'🚻 Washroom',interchange:'🔄 Interchange'}[f]||f}</span>`).join('')}</div>
      <div class="station-connections"><h4>Connected Stations</h4>${neighborHTML}</div>
    `;
    document.getElementById('close-station-info').addEventListener('click', () => {
      this.stationInfoPanel.classList.remove('active');
    });
  }

  _renderHistory() {
    if (!this.historyList) return;
    const history = this.graph.journeyHistory;
    if (history.length === 0) {
      this.historyList.innerHTML = '<div class="history-empty">No journeys yet. Plan your first route!</div>';
      return;
    }
    this.historyList.innerHTML = history.slice(0, 10).map(j => {
      const srcName = STATIONS[j.source]?.displayName || j.source;
      const dstName = STATIONS[j.destination]?.displayName || j.destination;
      return `<div class="history-item"><div class="history-route"><span class="history-src">${srcName}</span><span class="history-arrow">→</span><span class="history-dst">${dstName}</span></div><div class="history-meta"><span>₹${j.fare}</span><span>${j.stations} stn</span><span class="history-time">${j.timestamp}</span></div></div>`;
    }).join('');
  }

  clearHistory() {
    this.graph.clearJourneyHistory();
    this._renderHistory();
  }

  _showError(msg) {
    const toast = document.createElement('div');
    toast.className = 'error-toast';
    toast.textContent = msg;
    document.body.appendChild(toast);
    setTimeout(() => toast.classList.add('show'), 10);
    setTimeout(() => { toast.classList.remove('show'); setTimeout(() => toast.remove(), 300); }, 3000);
  }
}

export { JourneyPlanner };
