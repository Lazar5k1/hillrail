#!/bin/bash

# Hill Cipher + Rail Fence Cipher Testing Script
# Author: Jie Lin, Ph.D.
# 
# This script compiles and tests student implementations of the Hill Cipher + Rail Fence Cipher
# Supports C (.c), C++ (.cpp), and Rust (.rs) source files
# Tests all permutations against expected results in the expected_results folder

# Check if source file argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_file>"
    echo "Supported files: hillrail.c, hillrail.cpp, hillrail.rs"
    echo ""
    echo "This script will:"
    echo "  1. Compile your source code"
    echo "  2. Run all test cases from the test_cases directory"
    echo "  3. Compare your output with expected results"
    echo "  4. Provide detailed feedback on any differences"
    exit 1
fi

# Color codes for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# Determine source file type and compile accordingly
case $1 in
    hillrail.c)
        print_status $BLUE "[COMPILE] Compiling C source file: $1"
        rm -f -- hillrail
        gcc -Wall -Wextra -std=c99 -O2 hillrail.c -o hillrail -lm
        if [ $? -ne 0 ]; then 
            print_status $RED "[ERROR] Compilation of hillrail.c failed"
            echo "Please fix compilation errors and try again."
            exit 1 
        fi
        EXE="./hillrail"
        LANG="C"
        ;;
    hillrail.cpp)
        print_status $BLUE "[COMPILE] Compiling C++ source file: $1"
        rm -f -- hillrail
        g++ -Wall -Wextra -std=c++17 -O2 hillrail.cpp -o hillrail
        if [ $? -ne 0 ]; then 
            print_status $RED "[ERROR] Compilation of hillrail.cpp failed"
            echo "Please fix compilation errors and try again."
            exit 1 
        fi
        EXE="./hillrail"
        LANG="C++"
        ;;
    hillrail.rs)
        print_status $BLUE "[COMPILE] Compiling Rust source file: $1"
        rm -f -- hillrail
        rustc -O hillrail.rs -o hillrail
        if [ $? -ne 0 ]; then
            print_status $RED "[ERROR] Compilation of hillrail.rs failed"
            echo "Please fix compilation errors and try again."
            exit 1
        fi
        EXE="./hillrail"
        LANG="Rust"
        ;;
    *)
        print_status $RED "[ERROR] Invalid source file name: $1"
        echo ">>  Supported files: hillrail.c, hillrail.cpp, hillrail.rs"
        echo ">>  Only C, C++, and Rust are supported"
        exit 1
        ;;
esac

print_status $GREEN "[SUCCESS] Compilation of $1 succeeded using $LANG"
echo ""

# Create student output directory if it doesn't exist
mkdir -p student_output

# Initialize test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

print_status $CYAN "[TESTING] Starting comprehensive test evaluation..."
echo "============================================================"

# Check if expected_results directory exists
if [ ! -d "expected_results" ]; then
    print_status $RED "[ERROR] Expected results directory not found!"
    echo "Please ensure the 'expected_results' directory exists with all test case results."
    exit 1
fi

# Function to run a single test case
run_test_case() {
    local mode=$1
    local key_file=$2
    local text_file=$3
    local depth=$4
    local expected_file=$5
    local test_name=$6
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    print_status $PURPLE "[TEST] Test Case: $test_name"
    echo "   Mode: $mode, Key: $key_file, Text: $text_file, Depth: $depth"
    
    # Check if input files exist
    if [ ! -f "$key_file" ]; then
        print_status $RED "   [ERROR] Key file $key_file not found"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    if [ ! -f "$text_file" ]; then
        print_status $RED "   [ERROR] Text file $text_file not found"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    if [ ! -f "$expected_file" ]; then
        print_status $RED "   [ERROR] Expected result file $expected_file not found"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    # Run student program and capture output
    local student_output="student_output/${test_name}_output.txt"
    eval $EXE "$mode" "$key_file" "$text_file" "$depth" > "$student_output" 2>&1
    
    if [ $? -ne 0 ]; then
        print_status $RED "   [ERROR] Program execution failed"
        echo "   [FILE] Error output saved to: $student_output"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    # Compare with expected output
    if diff -w "$student_output" "$expected_file" > /dev/null 2>&1; then
        print_status $GREEN "   [PASS] PASSED - Output matches expected result"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        print_status $RED "   [FAIL] FAILED - Output differs from expected result"
        echo "   [FILE] Student output: $student_output"
        echo "   [FILE] Expected output: $expected_file"
        echo "   [DIFF] First few differences:"
        diff -w "$student_output" "$expected_file" | head -10
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Run all test cases by scanning expected_results directory
echo "[SCAN] Scanning expected_results directory for test cases..."

# Find all expected result files and extract test parameters
for expected_file in expected_results/*.txt; do
    if [ -f "$expected_file" ]; then
        # Extract filename without path and extension
        filename=$(basename "$expected_file" .txt)
        
        # Parse filename to extract test parameters
        # Expected format: key<N>_plaintext<M>_depth<D>_<mode>
        # or similar patterns that match your expected results
        
        if [[ $filename =~ key([0-9]+)_plaintext([0-9]+)_depth([0-9]+) ]]; then
            key_num="${BASH_REMATCH[1]}"
            plaintext_num="${BASH_REMATCH[2]}"
            depth="${BASH_REMATCH[3]}"
            
            key_file="test_cases/key${key_num}.txt"
            plaintext_file="test_cases/plaintext${plaintext_num}.txt"
            
            # Determine mode (encrypt/decrypt) from filename or default to encrypt
            if [[ $filename == *"decrypt"* ]]; then
                mode="decrypt"
            else
                mode="encrypt"
            fi
            
            run_test_case "$mode" "$key_file" "$plaintext_file" "$depth" "$expected_file" "$filename"
        else
            print_status $YELLOW "   [WARN] Skipping file with unrecognized format: $filename"
        fi
    fi
done

echo ""

# Print final results
echo "============================================================"
print_status $CYAN "[COMPLETE] EVALUATION COMPLETE"
echo "============================================================"
echo "[RESULTS] Results Summary:"
echo "   Total Tests: $TOTAL_TESTS"
print_status $GREEN "   Passed: $PASSED_TESTS"
print_status $RED "   Failed: $FAILED_TESTS"

if [ $TOTAL_TESTS -gt 0 ]; then
    SUCCESS_RATE=$(( PASSED_TESTS * 100 / TOTAL_TESTS ))
    echo "   Success Rate: ${SUCCESS_RATE}%"
else
    echo "   Success Rate: N/A (No tests found)"
fi

if [ $FAILED_TESTS -eq 0 ] && [ $TOTAL_TESTS -gt 0 ]; then
    echo ""
    print_status $GREEN "[SUCCESS] Congratulations! All tests passed!"
    print_status $GREEN "[SUCCESS] Your implementation is working correctly."
elif [ $TOTAL_TESTS -eq 0 ]; then
    echo ""
    print_status $YELLOW "[WARN] No test cases found in expected_results directory."
    echo "Please ensure your expected_results directory contains properly named test files."
else
    echo ""
    print_status $YELLOW "[WARN] Some tests failed. Please review the differences above."
    echo "[DIR] Student outputs saved in: student_output/"
    echo "[DIR] Expected results in: expected_results/"
    echo ""
    echo "[TIPS] Tips for debugging:"
    echo "   - Check the differences shown above"
    echo "   - Verify your algorithm implementation"
    echo "   - Ensure output format matches exactly"
    echo "   - Test individual cases manually"
fi

echo ""
print_status $BLUE "[CLEANUP] Cleanup: Removing executable..."
rm -f hillrail

exit $FAILED_TESTS
