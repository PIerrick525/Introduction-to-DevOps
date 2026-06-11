Epitech DevOps & CI/CD Projects

Welcome to this repository containing two major DevOps and Cloud projects realized during my curriculum at Epitech: POPEYE (Containerization & Orchestration) and CHOCOLATINE (CI/CD Automation).
Project 1: POPEYE (Docker & Orchestration)
Overview

Popeye is a full-stack containerization project. The goal was to take an existing multi-language web application (a voting system) and containerize every single component using optimized Dockerfiles and link them using Docker Compose.

Architecture

The application is microservices-based and split into 5 distinct layers:

    Poll: A Flask (Python) web application where users can vote. (Exposed on port 5000).

    Redis: A message queue that temporarily stores incoming votes.

    Worker: A Java (Worker) multi-stage build application that consumes votes from Redis and pushes them to the database.

    PostgreSQL: A persistent database that stores the final results.

    Result: A Node.js web application displaying the voting results in real-time. (Exposed on port 5001).

Key Features Implemented

    Multi-Stage Builds: Optimized Java Worker image size using a compilation stage (maven:3.9.6-alpine) and a lightweight runtime stage (eclipse-temurin:21-jre-alpine).

    Network Isolation: Created 3 distinct networks (poll-tier, result-tier, back-tier) to respect the least-privilege communication principle.

    Data Persistence: Bound a named volume (db-data) to PostgreSQL to prevent data loss.

    Security: Secret management handled properly without hardcoding credentials (using environment variables for POSTGRES_PASSWORD).

Project 2: CHOCOLATINE (GitHub Actions Workflow)
Overview

Chocolatine is an automation project focused on setting up a robust, reusable, and optimized CI/CD pipeline via GitHub Actions (chocolatine.yml).

Workflow Pipeline Steps

The pipeline triggers automatically on every push and pull_request (ignoring specific branch patterns like no-ga/*) and executes the following sequential stages:

    Coding Style & Compilation Check (check_compilation_and_coding_style)

        Runs inside the official epitechcontent/epitest-docker.

        Executes banana-check-repo to verify standard compliance.

        Compiles the project using make (with CC=epiclang and a 2-minute timeout).

        Generates interactive GitHub Error Annotations for lines failing the coding style.

        Verifies the presence and execution rights of required binaries.

    Unit Testing (run_tests)

        Launches automated test suites using make tests_run.

    Delivery Mirroring (push_to_mirror)

        Securely mirrors the repository to a target destination (e.g., Epitech repository) using SSH keys stored in GitHub Secrets (GIT_SSH_PRIVATE_KEY).

How to use this repository

    [!NOTE]
    If you have grouped these projects into sub-folders, navigate to the respective directory first.

Running Popeye (Docker)
Bash

# Clone the repository
git clone https://github.com/your-username/your-repo.git
cd popeye

# Launch the full infrastructure safely
export POSTGRES_PASSWORD="your_secure_password_here"
docker compose up --build -d

    Vote here: http://localhost:5000

    See results here: http://localhost:5001

Using Chocolatine (CI/CD)

Simply copy the .github/workflows/chocolatine.yml file into your own project repository, set up your MIRROR_SSH_URL and EXECUTABLES environment variables at the workflow level, and add your SSH private key to your GitHub repository secrets.

Technologies Used

    DevOps: Docker, Docker Compose, GitHub Actions, CI/CD pipelines.

    Languages & Frameworks: Python (Flask), Node.js, Java (Maven/Temurin), SQL (PostgreSQL).
