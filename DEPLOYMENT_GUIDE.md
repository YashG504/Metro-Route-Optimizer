# 🚀 METRO ROUTE OPTIMIZER - DEPLOYMENT & EXECUTION GUIDE

## 📋 Table of Contents
1. [How to Run Locally](#how-to-run-locally)
2. [Deployment Options](#deployment-options)
3. [Advanced Deployments](#advanced-deployments)
4. [Troubleshooting](#troubleshooting)

---

## 🖥️ HOW TO RUN LOCALLY

### Method 1: Linux/Mac (Recommended)

#### Prerequisites
```bash
# Check if g++ is installed
g++ --version

# If not installed, install it:
# Ubuntu/Debian
sudo apt-get install g++

# Mac
brew install gcc
```

#### Step-by-Step Compilation & Execution
```bash
# Navigate to project directory
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer"

# Step 1: Compile
g++ -std=c++17 -o metro-app metro-app.cpp

# Step 2: Run
./metro-app

# Alternative (with optimization)
g++ -std=c++17 -O2 -o metro-app metro-app.cpp
./metro-app
```

#### Expected Output
```
╔════════════════════════════════════════════════════════════════╗
║     WELCOME TO DELHI METRO ROUTE OPTIMIZER (v2.0)            ║
║     Enhanced with Dijkstra's Algorithm & Smart Features      ║
╚════════════════════════════════════════════════════════════════╝

╔════════════════════════════════════════════════╗
║          MAIN MENU                             ║
╠════════════════════════════════════════════════╣
║  1. List All Stations                         ║
║  2. Show Metro Map                            ║
║  3. Shortest Distance (Dijkstra)              ║
...
```

---

### Method 2: Windows

#### Prerequisites
- **MinGW** (Minimalist GNU for Windows)
- **Visual Studio Build Tools**
- **Or**: Use Windows Subsystem for Linux (WSL)

#### Option A: Using MinGW
```bash
# Download from: https://sourceforge.net/projects/mingw/

# After installation, compile:
g++ -std=c++17 -o metro-app.exe metro-app.cpp

# Run:
metro-app.exe
```

#### Option B: Using Visual Studio
```bash
# Compile with MSVC
cl /std:c++17 /EHsc metro-app.cpp

# Run:
metro-app.exe
```

#### Option C: Windows Subsystem for Linux (WSL)
```bash
# Install WSL if not already done
# Then follow Linux instructions above
```

---

### Method 3: One-Command Execution (Mac/Linux)

```bash
# Compile and run in one command
g++ -std=c++17 metro-app.cpp -o metro-app && ./metro-app
```

---

### Method 4: Using Makefile (Professional Approach)

Create `Makefile` in project directory:

```makefile
# Makefile for Metro Route Optimizer

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = metro-app
SOURCES = metro-app.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
```

Usage:
```bash
# Compile
make

# Run
make run

# Clean build files
make clean

# Full rebuild
make clean && make run
```

---

## 🌐 DEPLOYMENT OPTIONS

### Option 1: CLI Standalone (Current)

**✅ Pros:**
- Simple to deploy
- Minimal dependencies
- Works on any OS
- Easy to integrate

**❌ Cons:**
- Text-only interface
- No remote access
- Limited scalability

**Deployment Steps:**
```bash
# Copy executable to target machine
scp metro-app user@server:/usr/local/bin/

# SSH into server
ssh user@server

# Run it
metro-app
```

---

### Option 2: Web Application (Recommended for Live)

#### Convert to Web Server (C++ Backend)

**Technology Stack:**
- **Backend**: C++ with web framework
- **Frontend**: HTML/CSS/JavaScript
- **Server**: Apache/Nginx

**Option A: Using Crow (C++ Framework)**

Create `metro-web.cpp`:
```cpp
#include "crow_all.h"
#include "metro-app.cpp"  // Include core logic

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/route").methods("POST"_method)
    ([](const crow::request& req){
        auto data = crow::json::load(req.body);
        string src = data["source"].s();
        string dst = data["destination"].s();
        
        // Call dijkstra here
        Graph_M g;
        Graph_M::Create_Metro_Map(g);
        int distance = g.dijkstra(src, dst, false);
        
        crow::response res;
        res.code = 200;
        res.body = crow::json::wvalue{
            {"distance", distance},
            {"status", "success"}
        }.dump();
        return res;
    });

    app.port(8080).multithreaded().run();
}
```

Compile:
```bash
g++ -std=c++17 metro-web.cpp -o metro-web
./metro-web
# Server runs on localhost:8080
```

**Option B: Using REST API with Node.js Backend**

Convert C++ to library (.so):
```bash
# Compile as shared library
g++ -std=c++17 -fPIC -shared metro-app.cpp -o metro.so
```

Then create Node.js wrapper (simpler alternative):
```javascript
// metro-server.js
const express = require('express');
const cors = require('cors');
const app = express();

app.use(cors());
app.use(express.json());

// In-memory metro graph (replicate C++ logic)
const routes = {
  "Noida_Sector_62~B": {
    "Botanical_Garden~B": 8
  },
  // ... more routes
};

app.post('/api/route', (req, res) => {
  const { source, destination } = req.body;
  
  // Call dijkstra algorithm
  const distance = dijkstra(source, destination, routes);
  
  res.json({
    source,
    destination,
    distance,
    timestamp: new Date()
  });
});

app.listen(3000, () => {
  console.log('Metro API running on port 3000');
});
```

Run:
```bash
npm install express cors
node metro-server.js
```

---

### Option 3: Containerization (Docker)

Create `Dockerfile`:
```dockerfile
FROM gcc:11

WORKDIR /app

# Copy project files
COPY metro-app.cpp .

# Compile
RUN g++ -std=c++17 -O2 -o metro-app metro-app.cpp

# Run
CMD ["./metro-app"]
```

#### Build and Run with Docker

```bash
# Build image
docker build -t metro-app:latest .

# Run container
docker run -it metro-app:latest

# Or run as daemon with port mapping (for web version)
docker run -d -p 8080:8080 metro-app:web
```

Create `docker-compose.yml` for more complex setups:
```yaml
version: '3.8'

services:
  metro-app:
    build: .
    ports:
      - "8080:8080"
    environment:
      - ENV=production
    volumes:
      - ./journey_history.txt:/app/journey_history.txt
```

Run:
```bash
docker-compose up -d
```

---

### Option 4: Cloud Deployment

#### A. AWS Deployment

**Step 1: Create EC2 Instance**
```bash
# SSH into EC2
ssh -i your-key.pem ec2-user@your-instance.amazonaws.com

# Install dependencies
sudo yum install gcc-c++ git

# Clone/upload project
git clone <your-repo>
cd metro-optimizer

# Compile and run
g++ -std=c++17 -O2 -o metro-app metro-app.cpp
./metro-app
```

**Step 2: Create AMI (Amazon Machine Image)**
```bash
# From EC2 dashboard, create image from instance
# Share with others or launch new instances from it
```

#### B. Heroku Deployment

Create `Procfile`:
```
web: ./metro-app
```

Create `heroku.yml`:
```yaml
build:
  docker:
    web: Dockerfile

run:
  web: ./metro-app
```

Deploy:
```bash
heroku login
heroku create metro-app
git push heroku main
```

#### C. Google Cloud Platform

```bash
# Create compute instance
gcloud compute instances create metro-app \
  --zone=us-central1-a \
  --machine-type=f1-micro

# SSH into instance
gcloud compute ssh metro-app --zone=us-central1-a

# Then follow same steps as EC2
```

#### D. Azure Deployment

```bash
# Create container registry
az acr create --resource-group myResourceGroup \
  --name metroAppRegistry --sku Basic

# Build and push
az acr build --registry metroAppRegistry \
  --image metro-app:latest .

# Deploy to container instance
az container create --resource-group myResourceGroup \
  --name metro-app \
  --image metroAppRegistry.azurecr.io/metro-app:latest
```

---

## 🔄 ADVANCED DEPLOYMENTS

### Option 1: Kubernetes Deployment

Create `metro-deployment.yaml`:
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: metro-app
spec:
  replicas: 3
  selector:
    matchLabels:
      app: metro-app
  template:
    metadata:
      labels:
        app: metro-app
    spec:
      containers:
      - name: metro-app
        image: metro-app:latest
        ports:
        - containerPort: 8080
        resources:
          requests:
            memory: "64Mi"
            cpu: "250m"
          limits:
            memory: "128Mi"
            cpu: "500m"
---
apiVersion: v1
kind: Service
metadata:
  name: metro-app-service
spec:
  selector:
    app: metro-app
  ports:
  - protocol: TCP
    port: 80
    targetPort: 8080
  type: LoadBalancer
```

Deploy:
```bash
kubectl apply -f metro-deployment.yaml
kubectl get services
# Access via LoadBalancer IP
```

---

### Option 2: Serverless Deployment (AWS Lambda)

Package as Lambda function:
```python
# lambda_handler.py
import json
import subprocess

def lambda_handler(event, context):
    body = json.loads(event['body'])
    source = body['source']
    destination = body['destination']
    
    # Call C++ binary
    result = subprocess.run(
        ['./metro-app', source, destination],
        capture_output=True,
        text=True
    )
    
    return {
        'statusCode': 200,
        'body': json.dumps({
            'result': result.stdout
        })
    }
```

Deploy:
```bash
zip function.zip lambda_handler.py metro-app
aws lambda create-function --function-name metro-app \
  --runtime python3.9 --handler lambda_handler.lambda_handler \
  --zip-file fileb://function.zip
```

---

## 📊 DEPLOYMENT COMPARISON

| Option | Ease | Scalability | Cost | Speed | Best For |
|--------|------|-------------|------|-------|----------|
| **Standalone CLI** | ⭐⭐⭐⭐⭐ | ⭐ | Free | ⭐⭐⭐⭐⭐ | Development |
| **Docker** | ⭐⭐⭐⭐ | ⭐⭐⭐ | Low | ⭐⭐⭐⭐ | Production |
| **EC2/VM** | ⭐⭐⭐ | ⭐⭐ | Medium | ⭐⭐⭐ | Small scale |
| **Kubernetes** | ⭐⭐ | ⭐⭐⭐⭐⭐ | Medium-High | ⭐⭐⭐⭐ | Enterprise |
| **Serverless** | ⭐⭐⭐ | ⭐⭐⭐⭐ | Pay-per-use | ⭐⭐⭐⭐ | API endpoints |

---

## 🧪 TESTING BEFORE DEPLOYMENT

### Unit Testing
```bash
# Create test file
cat > test_metro.cpp << 'EOF'
#include <cassert>
#include "metro-app.cpp"

void test_dijkstra() {
    Graph_M g;
    Graph_M::Create_Metro_Map(g);
    int dist = g.dijkstra("Noida_Sector_62~B", "Rajiv_Chowk~BY", false);
    assert(dist == 24);  // Expected distance
}

int main() {
    test_dijkstra();
    cout << "All tests passed!" << endl;
    return 0;
}
EOF

# Compile and run
g++ -std=c++17 -o test test_metro.cpp
./test
```

### Load Testing
```bash
# Create load test script
for i in {1..1000}; do
    echo "Test $i" | ./metro-app > /dev/null
done
echo "Load test completed"
```

---

## ⚠️ TROUBLESHOOTING

### Problem 1: "g++ command not found"
```bash
# Solution: Install g++
# Ubuntu/Debian
sudo apt-get install g++

# Mac
brew install gcc

# Windows
# Download MinGW from https://www.mingw-w64.org/
```

### Problem 2: "Compilation errors: bits/stdc++.h not found"
```bash
# Solution 1: Use without bits/stdc++.h (better practice)
# Or Solution 2: Install development headers
sudo apt-get install build-essential

# Or Solution 3: Use -isystem flag
g++ -std=c++17 -isystem /usr/include metro-app.cpp -o metro-app
```

### Problem 3: "Runtime: No such file or directory"
```bash
# Solution: Use absolute path
/media/yashghotekar/New\ Volume4/Metro\ Route\ Optimizer/metro-app

# Or change directory first
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer"
./metro-app
```

### Problem 4: Permission Denied
```bash
# Solution: Make executable
chmod +x metro-app

# Then run
./metro-app
```

### Problem 5: Memory Issues in Docker
```dockerfile
# Add memory limits
FROM gcc:11
# ... rest of Dockerfile ...
# Set resource limits in docker-compose.yml
```

---

## 🎯 QUICK START COMMANDS

### Fastest Way to Run (5 seconds)
```bash
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer" && \
g++ -std=c++17 -O2 metro-app.cpp -o metro-app && \
./metro-app
```

### Production Deployment (Docker)
```bash
# Build
docker build -t metro-app:latest .

# Run
docker run -it metro-app:latest

# Share
docker push your-registry/metro-app:latest
```

### Cloud Deployment (AWS)
```bash
# Setup EC2
aws ec2 run-instances --image-id ami-0c55b159cbfafe1f0 \
  --count 1 --instance-type t2.micro --key-name my-key

# Deploy
scp -i my-key.pem metro-app ec2-user@instance:/tmp/
ssh -i my-key.pem ec2-user@instance "./metro-app"
```

---

## 📱 ACCESS FROM ANYWHERE

### Option 1: Cloud Server with Web Interface
```
1. Deploy to AWS/GCP/Azure
2. Create REST API wrapper
3. Access via: https://metro-app.example.com
4. Call API from anywhere globally
```

### Option 2: SSH Access
```bash
# From anywhere
ssh user@metro-app-server.com
./metro-app
```

### Option 3: WebSocket Real-time
Create WebSocket wrapper for live updates:
```javascript
const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', (ws) => {
  ws.on('message', (msg) => {
    // Process route query
    ws.send(JSON.stringify({ route: /* result */ }));
  });
});
```

---

## 📈 SCALING RECOMMENDATIONS

### Small Usage (< 100 queries/day)
- ✅ Docker on single server
- ✅ Cost: $5-10/month

### Medium Usage (100-10,000 queries/day)
- ✅ Kubernetes with 3 replicas
- ✅ Cost: $30-100/month

### Large Usage (> 10,000 queries/day)
- ✅ Full Kubernetes cluster
- ✅ Database caching layer
- ✅ Cost: $100-500/month

---

## 🔒 SECURITY CONSIDERATIONS

### Before Going Live
```bash
# 1. Input validation
# ✅ Already implemented

# 2. Rate limiting
# Add in web server config
# limit_req_zone $binary_remote_addr zone=api:10m rate=10r/s;

# 3. HTTPS/SSL
# Use Let's Encrypt for free SSL
# certbot certonly --standalone -d metro-app.example.com

# 4. Firewall rules
# Allow only necessary ports (80, 443)

# 5. Regular backups
# Backup journey_history.txt periodically
```

---

## ✅ CHECKLIST BEFORE DEPLOYMENT

- [ ] Code compiled without warnings
- [ ] Tested locally with various inputs
- [ ] All menu options working correctly
- [ ] No memory leaks detected
- [ ] Journey history file can be written
- [ ] Configuration file loads correctly
- [ ] Error messages are clear
- [ ] Performance is acceptable
- [ ] Documentation is complete
- [ ] Backup created of all files

---

## 🎉 SUMMARY

**Your project is ready to deploy!**

### Simplest Way (Today)
```bash
g++ -std=c++17 -O2 metro-app.cpp -o metro-app && ./metro-app
```

### Best Way (Production)
```bash
docker build -t metro-app . && docker run -p 8080:8080 metro-app
```

### Enterprise Way (Scalable)
```bash
kubectl apply -f metro-deployment.yaml
```

Choose based on your needs! 🚀

---

**Questions?**
- For CLI: Just run and follow menu
- For Web: Check Crow/Node.js examples
- For Cloud: See AWS/GCP/Azure sections
- For Kubernetes: Check K8s deployment file

Happy deploying! 🌟
