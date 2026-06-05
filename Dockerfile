FROM gcc:11-bullseye

WORKDIR /app

# Copy project files
COPY metro-app.cpp .
COPY README.md .
COPY FEATURES_ADDED.md .
COPY DEPLOYMENT_GUIDE.md .

# Compile with optimizations
RUN g++ -std=c++17 -O2 -static -o metro-app metro-app.cpp

# Create minimal final image (multi-stage build)
FROM debian:bullseye-slim

WORKDIR /app

# Copy only the compiled binary from builder
COPY --from=0 /app/metro-app .
COPY --from=0 /app/README.md .
COPY --from=0 /app/FEATURES_ADDED.md .
COPY --from=0 /app/DEPLOYMENT_GUIDE.md .

# Set permissions
RUN chmod +x metro-app

# Metadata
LABEL version="2.0"
LABEL description="Metro Route Optimizer - Dijkstra's Algorithm Implementation"
LABEL author="Your Name"

# Run the application
ENTRYPOINT ["./metro-app"]
