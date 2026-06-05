# ⚡ QUICK START GUIDE - RUN & DEPLOY IN 5 MINUTES

## 🏃 FASTEST WAY TO RUN (Linux/Mac)

### One-Line Command
```bash
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer" && g++ -std=c++17 -O2 metro-app.cpp -o metro-app && ./metro-app
```

**That's it!** The app will start immediately.

---

## 📋 Step-by-Step Instructions

### **For Linux/Mac Users:**

#### Step 1: Open Terminal
```bash
# Navigate to project directory
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer"
```

#### Step 2: Compile (Choose ONE method)

**Method A - Using Bash Script (Easiest)**
```bash
chmod +x run.sh
./run.sh
```

**Method B - Direct Compilation**
```bash
g++ -std=c++17 -O2 metro-app.cpp -o metro-app
```

**Method C - With All Warnings**
```bash
g++ -std=c++17 -Wall -Wextra -O2 metro-app.cpp -o metro-app
```

#### Step 3: Run
```bash
./metro-app
```

**Expected Output:**
```
╔════════════════════════════════════════════════════════════════╗
║     WELCOME TO DELHI METRO ROUTE OPTIMIZER (v2.0)            ║
║     Enhanced with Dijkstra's Algorithm & Smart Features      ║
╚════════════════════════════════════════════════════════════════╝

╔════════════════════════════════════════════════╗
║          MAIN MENU                             ║
╠════════════════════════════════════════════════╣
║  1. List All Stations                         ║
...
```

---

### **For Windows Users:**

#### Step 1: Install Compiler
- **Option A**: Download [MinGW](https://www.mingw-w64.org/)
- **Option B**: Install [Visual Studio Community](https://visualstudio.microsoft.com/community/) with C++ tools
- **Option C**: Use [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install)

#### Step 2: Run Batch Script
```bash
# Double-click run.bat
# Or from Command Prompt:
run.bat
```

**Or Compile Manually:**
```bash
g++ -std=c++17 -O2 metro-app.cpp -o metro-app.exe
metro-app.exe
```

---

## 🐳 Run with Docker (Recommended for Production)

### **All Platforms (Requires Docker)**

#### Step 1: Install Docker
- Download from [docker.com](https://www.docker.com/products/docker-desktop)
- Mac: `brew install docker`
- Ubuntu: `sudo apt-get install docker.io`

#### Step 2: Build Docker Image
```bash
docker build -t metro-app:latest .
```

#### Step 3: Run Container
```bash
# Interactive mode
docker run -it metro-app:latest

# Or with docker-compose
docker-compose up -d
```

---

## 🚀 USAGE EXAMPLES

### Example 1: Find Shortest Route
```
Menu: Enter 3
Source: Noida_Sector_62~B
Destination: IGI_Airport~O

Output: ✓ SHORTEST DISTANCE: 62 KM
```

### Example 2: Search Stations
```
Menu: Enter 10
Query: airport

Output:
Found 1 matching stations:
1. IGI_Airport~O
Enter choice: 1
```

### Example 3: View Journey History
```
Menu: Enter 9

Output:
1. Noida_Sector_62~B → IGI_Airport~O | 62 km
2. Rajiv_Chowk~BY → New_Delhi~YO | 2 km
```

---

## 📊 COMPARISON: Running Methods

| Method | Time | Complexity | Best For |
|--------|------|-----------|----------|
| **Bash Script** | 30 sec | Very Easy | Beginners |
| **Direct Compile** | 10 sec | Easy | Quick test |
| **Docker** | 1 min | Moderate | Production |
| **Docker Compose** | 45 sec | Easy | Teams |

---

## 🌐 DEPLOY TO LIVE SERVER

### **Option 1: Easiest - Cloud Run (Google)**
```bash
# Deploy in 2 commands
gcloud run deploy metro-app --source .
# Get your live URL instantly
```

### **Option 2: AWS EC2**
```bash
# Create EC2 instance
# SSH into it
# Copy project files
# Run:
g++ -std=c++17 -O2 metro-app.cpp -o metro-app && ./metro-app
```

### **Option 3: Heroku**
```bash
heroku login
heroku create metro-app
git push heroku main
# Live URL: https://metro-app.herokuapp.com
```

### **Option 4: Docker Container (Any Cloud)**
```bash
# Build and push to Docker Hub
docker build -t yourusername/metro-app .
docker push yourusername/metro-app

# Deploy anywhere that supports Docker:
# AWS ECS, Google Cloud Run, Azure Container, Heroku, DigitalOcean, etc.
```

---

## 🔧 TROUBLESHOOTING

### **Error: g++ not found**
```bash
# Linux (Ubuntu/Debian)
sudo apt-get install g++

# Mac
brew install gcc

# Windows
# Download MinGW or Visual Studio
```

### **Error: metro-app.cpp not found**
```bash
# Make sure you're in the right directory
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer"
ls -la metro-app.cpp  # Should show the file
```

### **Error: Permission denied**
```bash
# Make file executable
chmod +x metro-app
chmod +x run.sh

# Then run
./metro-app
```

### **Compilation errors**
```bash
# Try with this command:
g++ -std=c++17 -Wall -Wextra -o metro-app metro-app.cpp

# Check for stdc++ library:
g++ --version
gcc-libs --version
```

---

## ⏱️ PERFORMANCE EXPECTATIONS

| Operation | Expected Time |
|-----------|---|
| Compile | 5-15 seconds |
| Start up | < 100ms |
| Menu response | < 1ms |
| Dijkstra query | < 1ms |
| Search query | < 0.1ms |
| Application exit | Instant |

---

## 📁 PROJECT FILES

```
Metro Route Optimizer/
├── metro-app.cpp                  ← Main C++ source
├── run.sh                         ← Linux/Mac launcher
├── run.bat                        ← Windows launcher
├── Dockerfile                     ← Docker build file
├── docker-compose.yml             ← Docker composition
├── README.md                      ← Full documentation
├── DEPLOYMENT_GUIDE.md            ← Detailed deployment
├── FEATURES_ADDED.md              ← v2.0 features
├── BUGS_FIXED_REPORT.md           ← Bug fixes
└── PROJECT_ANALYSIS_REPORT.md     ← Analysis report
```

---

## 🎯 RECOMMENDED FLOW

### **For Learning**
1. Read README.md
2. Run locally: `./run.sh` or `run.bat`
3. Try all menu options
4. Check FEATURES_ADDED.md

### **For Demo/Interview**
1. Run locally on your machine
2. Show working features
3. Reference code comments in metro-app.cpp
4. Explain Dijkstra algorithm

### **For Deployment**
1. Use Docker: `docker build -t metro-app .`
2. Push to Docker Hub
3. Deploy to cloud platform
4. Access from anywhere

---

## 📞 NEED HELP?

### **Quick Questions**
- See README.md section "FAQ"
- Check DEPLOYMENT_GUIDE.md for specific scenarios

### **Compilation Issues**
- See "Troubleshooting" section above
- Search for your error message in DEPLOYMENT_GUIDE.md

### **Deployment Questions**
- DEPLOYMENT_GUIDE.md has 8 deployment options
- Docker approach recommended for beginners

### **Code Questions**
- Read comments in metro-app.cpp
- Check FEATURES_ADDED.md for algorithms

---

## ✅ VERIFICATION CHECKLIST

After running, verify:
- [ ] App starts without errors
- [ ] Main menu displays correctly
- [ ] Can select menu options
- [ ] Searching stations works
- [ ] Dijkstra computation works
- [ ] Journey history tracks queries
- [ ] Exit option closes cleanly

---

## 🎉 YOU'RE READY!

### **Right now, run:**
```bash
cd "/media/yashghotekar/New Volume4/Metro Route Optimizer"
./run.sh    # Mac/Linux
# OR
run.bat     # Windows
```

**That's it! Your project is running! 🚀**

---

## 📚 NEXT STEPS

1. **Explore Features**: Try all 11 menu options
2. **Read Docs**: Check FEATURES_ADDED.md
3. **Understand Code**: Read algorithm comments in metro-app.cpp
4. **Deploy**: Follow DEPLOYMENT_GUIDE.md for live deployment
5. **Share**: Deploy to cloud and share live URL

---

**Version**: 2.0 Enhanced  
**Status**: ✅ Ready to Run & Deploy  
**Last Updated**: June 5, 2026

**Happy exploring! 🚇✨**
