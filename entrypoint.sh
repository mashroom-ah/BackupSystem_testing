#!/bin/bash
set -e

case "$1" in
    test)
        echo "Running unit tests..."
        exec /app/bin/unit_tests
        ;;
    scenarios)
        echo "Running scenario tests..."
        exec /app/bin/test_scenarios
        ;;
    all)
        echo "Running all tests..."
        /app/bin/unit_tests
        /app/bin/test_scenarios
        ;;
    *)
        echo "Starting BackupSystem GUI..."
        exec /app/bin/BackupSystem
        ;;
esac