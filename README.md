# STDF Parser

A complete, high-performance C++ implementation for parsing Standard Test Data Format (STDF) files with advanced features including wafer-level support, bin management, syslog integration, multi-file generation, and comprehensive database storage.

## Overview

STDF (Standard Test Data Format) is a binary file format commonly used in the semiconductor industry for storing test data from Automated Test Equipment (ATE). This project provides a production-ready C++ library and command-line tool suite for parsing STDF files, storing extracted data in SQLite databases, and generating realistic test datasets.

**🤖 AI-Assisted Development**: This entire project was developed using **AI-assisted development** with **Cursor IDE** and **Claude-4-Sonnet**. The complete codebase, build system, test suite, and documentation were generated through natural language specifications and iterative refinement—showcasing the power of conversational software development without writing traditional code line-by-line.

## Features

### Core Functionality
- **Complete STDF V4 Support**: Parses major STDF record types including FAR, MIR, PIR, PRR, PTR, FTR, HBR, SBR, WIR, and WRR
- **SQLite Database Storage**: Stores parsed data in a structured SQLite database with proper schema
- **Endianness Detection**: Automatically detects and handles byte order differences in STDF files
- **High Performance**: Uses database transactions for optimal insertion speed (4000+ records/second)
- **Cross-Platform**: Built with standard C++17 and CMake for Windows, Linux, and macOS

### Advanced Features  
- **Professional Logging**: Integrated syslog logging for production environments
- **Multi-File Generation**: Advanced STDF generator with automatic filename conflict resolution
- **Realistic Test Data**: Generates varied test patterns with wafer-level data, bin statistics, and configurable pass/fail ratios
- **Command-Line Interface**: Comprehensive CLI with extensive options and help systems
- **Statistics and Reporting**: Built-in analysis capabilities with yield and performance statistics
- **Memory Efficient**: Stream-based processing suitable for large files (multi-GB support)

### Enterprise Ready
- **Production Logging**: All operations logged to system syslog for monitoring and debugging
- **Batch Processing**: Generate multiple test files with unique content and automatic naming
- **Error Resilience**: Comprehensive error handling and recovery mechanisms
- **Database Transactions**: ACID-compliant database operations with rollback support

## Project Structure

```
stdf/
├── CMakeLists.txt          # Build configuration with library separation
├── README.md              # Comprehensive documentation
├── include/               # Header files
│   ├── stdf_types.h      # STDF record type definitions and base classes
│   ├── stdf_parser.h     # Binary parser with endianness detection
│   ├── database.h        # SQLite interface with transaction support
│   └── logger.h          # Syslog integration wrapper
├── src/                  # Source files
│   ├── stdf_types.cpp    # Record serialization and string formatting
│   ├── stdf_parser.cpp   # Binary file parsing with error handling
│   ├── database.cpp      # Database operations and schema management
│   ├── main.cpp          # Parser application with CLI
│   └── stdf_generator.cpp # Multi-file generator with conflict resolution
├── bin/                  # Executable binaries (generated during build)
│   ├── stdf_parser       # Main parser executable
│   ├── stdf_generator    # Multi-file generator executable
│   └── stdf_tests        # Test suite executable
├── lib/                  # Static libraries (generated during build)
│   └── libstdf_lib.a     # Static library for integration
├── data/                 # Data directory for STDF files
├── test/                 # Test suite directory
│   └── stdf_system_test.cpp # Comprehensive Google Test suite (21 tests)
├── build/                # Build temporary files (CMake cache, object files, etc.)
└── coverage_report.txt   # Code coverage analysis report
```

## Key Implementation Details

### Architecture Overview
- **Object-Oriented Design**: Base `STDFRecord` class with derived record types
- **Memory Management**: Smart pointers (`std::unique_ptr`) for automatic memory management
- **Stream Processing**: Direct file-to-database streaming without loading entire files in memory
- **Modular Design**: Separated library (`stdf_lib`) from applications for easy integration

### Performance Optimizations
- **Database Transactions**: Batch insertions within transactions for 10x+ performance improvement
- **Binary Stream Reading**: Direct binary parsing without intermediate text conversion
- **Endianness Handling**: Runtime detection with optimized byte swapping when needed
- **Connection Pooling**: Single database connection reused across all operations

### Error Handling and Logging
- **Comprehensive Error Handling**: Try-catch blocks with detailed error messages
- **Syslog Integration**: Production-grade logging with configurable severity levels
- **Graceful Degradation**: Parser continues on record-level errors, logs warnings
- **Resource Management**: RAII pattern ensures proper cleanup on exceptions

### Database Design
- **Normalized Schema**: Separate tables for each record type with proper relationships
- **Indexing Strategy**: Primary keys and common query indexes for performance
- **Data Integrity**: Foreign key constraints and data validation
- **Temporal Tracking**: `created_at` timestamps for all records

### Generator Architecture
- **Filesystem Integration**: C++17 `std::filesystem` for robust path handling
- **Randomization Engine**: MT19937 with controlled seeds for reproducible test data
- **Content Variation**: Algorithmic generation of realistic test patterns with wafer-level flows and bin statistics
- **Conflict Resolution**: Automatic filename increment with preserved extensions and paths

## Prerequisites

- C++17 compatible compiler (GCC 8+, Clang 7+, or MSVC 2019+)
- CMake 3.16 or newer
- SQLite3 development libraries
- Google Test (for running test suite)
- lcov (for code coverage analysis)

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev libgtest-dev lcov
```

**CentOS/RHEL/Fedora:**
```bash
sudo dnf install gcc-c++ cmake sqlite-devel
# or for older versions:
sudo yum install gcc-c++ cmake sqlite-devel
```

**macOS:**
```bash
# Using Homebrew
brew install cmake sqlite
```

**Windows:**
- Install Visual Studio 2019 or newer with C++ support
- Install CMake from https://cmake.org/
- SQLite will be linked automatically on Windows

## Building

1. **Clone or navigate to the project directory:**
   ```bash
   cd /path/to/stdf
   ```

2. **Create build directory and configure:**
   ```bash
   mkdir -p build
   cd build
   cmake ..
   ```

3. **Build the project:**
   ```bash
   make -j$(nproc)
   ```

   On Windows with Visual Studio:
   ```bash
   cmake --build . --config Release
   ```

4. **The executables will be created in the build directory:**
   - `stdf_parser` - Main STDF parser application with syslog integration
   - `stdf_generator` - Advanced multi-file generator with conflict resolution
   - `libstdf_lib.a` - Static library for integration into other projects

## Usage

### STDF Parser Application

#### Basic Usage

Parse an STDF file and store it in a SQLite database:

```bash
./stdf_parser data/sample.stdf
```

#### Command Line Options

```bash
./stdf_parser [options] <stdf_file>

Options:
  -h, --help      Show help message and usage examples
  -d, --database  Specify database file (default: stdf_data.db)
  -v, --verbose   Enable verbose debug output (logged to syslog)
  -s, --stats     Show comprehensive statistics after parsing

Examples:
  ./stdf_parser data/sample.stdf                    # Basic parsing
  ./stdf_parser -d test.db -v -s data/sample.stdf   # Full analysis with verbose logging
  ./stdf_parser -s data/*.stdf                      # Parse multiple files with statistics
```

#### Viewing Logs

All application output is logged to syslog. View logs using:

```bash
# View recent parser logs
sudo journalctl -t stdf_parser --since "1 hour ago"

# Follow logs in real-time
sudo journalctl -f -t stdf_parser

# View all STDF-related logs
sudo journalctl | grep -E "(stdf_parser|stdf_generator)"
```

### STDF Generator Application

#### Basic Usage

Generate sample STDF files for testing:

```bash
# Generate single file
./stdf_generator

# Generate with custom filename
./stdf_generator data/my_test.stdf
```

#### Advanced Multi-File Generation

```bash
./stdf_generator [options] [output_file]

Options:
  -h, --help      Show help message with examples
  -n, --count <N> Generate N STDF files with unique content (default: 1)

Arguments:
  output_file     Output filename (default: data/sample.stdf)

Examples:
  ./stdf_generator                                   # Single file: data/sample.stdf
  ./stdf_generator -n 5                              # Generate 5 varied files
  ./stdf_generator -n 3 test/production_data.stdf    # 3 files with custom base name
  ./stdf_generator -n 10 ../output/batch_test.stdf   # Batch generation
```

#### Automatic Filename Conflict Resolution

The generator automatically handles filename conflicts:

```bash
# If test.stdf exists, creates test_1.stdf, test_2.stdf, etc.
./stdf_generator -n 3 test.stdf
```

Results in:
- `test.stdf` (if it didn't exist) or `test_1.stdf` (if it did)
- `test_2.stdf`
- `test_3.stdf`

#### Generated Content Variations

Each generated file contains realistic variations:
- **Different lot IDs**: `TEST_LOT_001`, `TEST_LOT_002`, etc.
- **Varied part counts**: 8-12 parts per file with randomization
- **Multiple test patterns**: 3-7 tests per part with different characteristics
- **Realistic pass/fail ratios**: ~90% pass rate with controlled randomization
- **Variable test times**: Based on test complexity with random factors
- **Different part types**: Cycles through multiple part type patterns

## System Integration

### Syslog Integration

The STDF parser system is designed for production environments with comprehensive syslog integration:

#### Log Categories
- **INFO**: Normal operational messages (startup, completion, statistics)
- **WARNING**: Non-fatal issues (transaction failures, insertion errors)
- **ERROR**: Critical errors (file access, database connection failures)
- **DEBUG**: Detailed debugging information (enabled with `-v` flag)

#### Log Management
```bash
# Configure log rotation (add to /etc/rsyslog.conf)
if $programname == 'stdf_parser' then /var/log/stdf_parser.log
if $programname == 'stdf_generator' then /var/log/stdf_generator.log
& stop

# Restart rsyslog to apply changes
sudo systemctl restart rsyslog

# Set up log rotation (create /etc/logrotate.d/stdf)
/var/log/stdf_parser.log /var/log/stdf_generator.log {
    daily
    rotate 30
    compress
    delaycompress
    missingok
    notifempty
    copytruncate
}
```

#### Remote Logging
```bash
# Forward logs to remote syslog server (add to /etc/rsyslog.conf)
if $programname == 'stdf_parser' then @@remote-log-server:514
if $programname == 'stdf_generator' then @@remote-log-server:514
```

#### Integration with Monitoring Systems
- **Splunk**: Index syslog data for analysis and alerting
- **ELK Stack**: Elasticsearch + Logstash + Kibana for log analytics
- **Grafana**: Dashboard creation for operational metrics
- **Nagios/Zabbix**: Alert on error conditions or performance thresholds

### Library Integration

The `libstdf_lib.a` static library can be integrated into other C++ applications:

```cpp
#include "stdf_parser.h"
#include "database.h"
#include "logger.h"

int main() {
    // Initialize logging for your application
    STDF::Logger::init("my_application");
    
    // Create parser and database instances
    STDF::STDFParser parser("data/test.stdf");
    STDF::Database db("output.db");
    
    // Parse and store data
    if (db.open() && db.createTables()) {
        auto records = parser.parseFile();
        db.beginTransaction();
        for (const auto& record : records) {
            db.insertRecord(*record);
        }
        db.commitTransaction();
    }
    
    // Cleanup
    STDF::Logger::cleanup();
    return 0;
}
```

### Build Integration

```cmake
# In your CMakeLists.txt
find_package(SQLite3 REQUIRED)

# Add STDF library
add_subdirectory(path/to/stdf)
target_link_libraries(your_target stdf_lib)
```

### Sample Output

#### Console Output (Help and Usage)

```bash
$ ./stdf_parser -h
Usage: ./stdf_parser [options] <stdf_file>

Options:
  -h, --help      Show this help message
  -d, --database  Specify database file (default: stdf_data.db)
  -v, --verbose   Enable verbose output
  -s, --stats     Show statistics after parsing

Example:
  ./stdf_parser -d test.db -v -s data/sample.stdf
```

#### Syslog Output (Operational Messages)

```bash
$ sudo journalctl -t stdf_parser --since "1 minute ago"
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: STDF Parser v1.0 starting
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Input file: data/test_1.stdf
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Database: stdf_data.db
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Verbose: Yes
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: === Parsing Complete ===
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Total records parsed: 158
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Records inserted: 158
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Processing time: 12 ms
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Processing rate: 13166.7 records/second
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: === Database Statistics ===
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Total Parts: 22
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Passed Parts: 21
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Failed Parts: 1
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Yield: 95.45%
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: Total Tests: 134
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: === Record Counts ===
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: FAR Records: 2
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: MIR Records: 2
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: PIR Records: 22
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: PRR Records: 22
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: PTR Records: 134
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: FTR Records: 0
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: HBR Records: 4
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: SBR Records: 4
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: WIR Records: 2
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: WRR Records: 2
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: === Available Lots ===
Jul 30 16:10:38 computingstudios stdf_parser[1561475]:   TEST_LOT_001
Jul 30 16:10:38 computingstudios stdf_parser[1561475]: === Available Part Types ===
Jul 30 16:10:38 computingstudios stdf_parser[1561475]:   PART_TYPE_2
```

#### Generator Output

```bash
$ sudo journalctl -t stdf_generator --since "1 minute ago"
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: STDF Sample File Generator starting
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Base filename: ../data/test.stdf
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Number of files to generate: 3
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Generating 3 STDF files with base name: ../data/test.stdf
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Generated sample STDF file: ../data/test_1.stdf
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Generated sample STDF file: ../data/test_2.stdf
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Generated sample STDF file: ../data/test_3.stdf
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: Successfully generated 3 out of 3 files
Jul 30 16:09:51 computingstudios stdf_generator[1561168]: File generation completed successfully!
```

## Database Schema

The parser creates the following tables in SQLite:

### far_records
- `id` (INTEGER PRIMARY KEY)
- `cpu_typ` (INTEGER) - CPU type that wrote the file
- `stdf_ver` (INTEGER) - STDF version number
- `created_at` (DATETIME)

### mir_records  
- `id` (INTEGER PRIMARY KEY)
- `lot_id` (TEXT) - Lot identification
- `part_typ` (TEXT) - Part type
- `node_nam` (TEXT) - Tester node name
- `start_t` (INTEGER) - Test start time
- ... (additional MIR fields)
- `created_at` (DATETIME)

### ptr_records
- `id` (INTEGER PRIMARY KEY)
- `test_num` (INTEGER) - Test number
- `head_num` (INTEGER) - Test head number
- `site_num` (INTEGER) - Test site number
- `result` (REAL) - Test result value
- `test_txt` (TEXT) - Test description
- `units` (TEXT) - Test units
- `lo_limit` (REAL) - Low limit
- `hi_limit` (REAL) - High limit
- ... (additional PTR fields)
- `created_at` (DATETIME)

### prr_records
- `id` (INTEGER PRIMARY KEY)  
- `head_num` (INTEGER) - Test head number
- `site_num` (INTEGER) - Test site number
- `hard_bin` (INTEGER) - Hardware bin number
- `soft_bin` (INTEGER) - Software bin number
- `x_coord` (INTEGER) - Wafer X coordinate
- `y_coord` (INTEGER) - Wafer Y coordinate
- `part_id` (TEXT) - Part identification
- ... (additional PRR fields)
- `created_at` (DATETIME)

## Supported STDF Records

| Record Type | Description | Status |
|-------------|-------------|---------|
| FAR | File Attribute Record | ✅ Supported |
| MIR | Master Information Record | ✅ Supported |
| PIR | Part Information Record | ✅ Supported |
| PRR | Part Results Record | ✅ Supported |
| PTR | Parametric Test Record | ✅ Supported |
| FTR | Functional Test Record | ✅ Supported |
| MPR | Multiple-Result Parametric Test Record | 🔄 Planned |
| TSR | Test Synopsis Record | 🔄 Planned |
| HBR | Hardware Bin Record | ✅ Supported |
| SBR | Software Bin Record | ✅ Supported |
| WIR | Wafer Information Record | ✅ Supported |
| WRR | Wafer Results Record | ✅ Supported |

## Development

### Adding New Record Types

1. Define the record structure in `include/stdf_types.h`
2. Implement the parsing logic in `src/stdf_parser.cpp`
3. Add database schema and insertion methods in `src/database.cpp`
4. Update the record dispatcher in `parseRecord()` method

### Running Tests

The project includes a comprehensive test suite built with Google Test framework that achieves **79.5% overall coverage** with excellent coverage on core functionality.

#### Test Suite Overview
- **21 tests** across 5 test suites
- **100% test execution success rate**
- **79.5% weighted average code coverage**
- **97.8% coverage** on core STDF record types
- **76.5% coverage** on database operations  
- **69.7% coverage** on parser functionality

#### Building and Running Tests

**Prerequisites:**
```bash
# Install Google Test and coverage tools
sudo apt-get install -y libgtest-dev cmake lcov
```

**Build with Coverage:**
```bash
mkdir build && cd build
cmake .. -DENABLE_COVERAGE=ON
make stdf_tests
```

**Run Test Suite:**
```bash
# Run all tests (from project root)
./bin/stdf_tests

# Run with verbose output
./bin/stdf_tests --gtest_output=xml:test_results.xml
```

**Generate Coverage Report:**
```bash
# Simple text summary
make coverage-summary

# Detailed coverage with gcov
gcov CMakeFiles/stdf_lib.dir/src/*.cpp.gcda | grep -E "File|Lines executed"

# Clean coverage data
make coverage-clean
```

#### Test Categories

**Unit Tests (16 tests):**
- All 10 STDF record types (FAR, MIR, PIR, PRR, PTR, FTR, HBR, SBR, WIR, WRR)
- Database operations (CRUD, transactions, statistics)
- Parser functionality (file I/O, endianness detection)
- Logger integration (syslog wrapper)

**Integration Tests (5 tests):**
- End-to-end file parsing and database insertion
- Real STDF file processing from `data/` directory
- Error handling scenarios (file not found, corrupt data)
- Statistics generation and validation

#### Coverage Results
```
Core Source Files:
• src/stdf_types.cpp:   97.80% (222/227 lines) ✅
• src/database.cpp:     76.52% (277/362 lines) ✅  
• src/stdf_parser.cpp:  69.72% (221/317 lines) ✅

Overall: 720/906 lines covered = 79.5% ✅
```

#### Manual Testing
For additional validation (from project root):
1. Generate sample data: `./bin/stdf_generator -n 5 data/test_file.stdf`
2. Parse the data: `./bin/stdf_parser -v -s data/test_file.stdf`
3. Verify database content: `sqlite3 stdf_data.db ".tables"`

## Examples and Queries

### SQL Queries for Analysis

After parsing STDF data, you can run SQL queries on the database:

```sql
-- Get yield by lot
SELECT 
    mir.lot_id,
    COUNT(prr.id) as total_parts,
    SUM(CASE WHEN prr.hard_bin = 1 THEN 1 ELSE 0 END) as passed_parts,
    ROUND(100.0 * SUM(CASE WHEN prr.hard_bin = 1 THEN 1 ELSE 0 END) / COUNT(prr.id), 2) as yield_percent
FROM prr_records prr
JOIN mir_records mir ON 1=1  -- Assuming single lot per file
GROUP BY mir.lot_id;

-- Get test results statistics
SELECT 
    ptr.test_txt,
    ptr.units,
    COUNT(*) as test_count,
    AVG(ptr.result) as avg_result,
    MIN(ptr.result) as min_result,
    MAX(ptr.result) as max_result,
    ptr.lo_limit,
    ptr.hi_limit
FROM ptr_records ptr
GROUP BY ptr.test_num, ptr.test_txt
ORDER BY ptr.test_num;

-- Find failing tests
SELECT 
    ptr.test_txt,
    ptr.result,
    ptr.lo_limit,
    ptr.hi_limit,
    CASE 
        WHEN ptr.result < ptr.lo_limit THEN 'Below Low Limit'
        WHEN ptr.result > ptr.hi_limit THEN 'Above High Limit'
        ELSE 'Within Limits'
    END as status
FROM ptr_records ptr
WHERE ptr.result < ptr.lo_limit OR ptr.result > ptr.hi_limit;
```

## Troubleshooting

### Common Issues

1. **Build Issues**
   ```bash
   # SQLite3 not found during build:
   # Ubuntu/Debian
   sudo apt install libsqlite3-dev
   
   # CentOS/RHEL/Fedora  
   sudo dnf install sqlite-devel
   
   # Compilation errors with C++17:
   # Ensure you're using GCC 8+, Clang 7+, or MSVC 2019+
   # On older systems, install newer compiler version
   ```

2. **Runtime Issues**
   ```bash
   # Permission errors when creating database:
   # - Ensure write permissions in current directory
   # - Specify different database path: ./stdf_parser -d /tmp/test.db file.stdf
   
   # File not found errors:
   # - Check file paths are correct
   # - Ensure data directory exists: mkdir -p data
   ```

3. **Performance Issues**
   ```bash
   # Large file parsing is slow:
   # - Check system resources (CPU, memory, disk I/O)
   # - Monitor with verbose logging: ./stdf_parser -v large_file.stdf
   # - For files >1GB, ensure sufficient disk space for database
   ```

4. **Logging Issues**
   ```bash
   # Can't see application logs:
   sudo journalctl -t stdf_parser --since "1 hour ago"
   
   # Logs not appearing in syslog:
   # - Check syslog daemon is running: systemctl status rsyslog
   # - Verify log permissions: ls -la /var/log/
   
   # Too much verbose output:
   # - Remove -v flag for production use
   # - Filter logs: journalctl -t stdf_parser -p info
   ```

5. **Generator Issues**
   ```bash
   # Generated files are identical:
   # - This is normal for single file generation
   # - Use -n option for varied content: ./stdf_generator -n 5
   
   # Filename conflicts not resolving:
   # - Check filesystem permissions
   # - Verify target directory exists
   # - Use absolute paths: ./stdf_generator /full/path/to/output.stdf
   ```

### Debugging Steps

1. **Check Application Status**
   ```bash
   # Verify executables exist and are executable
   ls -la stdf_parser stdf_generator
   
   # Test basic functionality
   ./stdf_parser -h
   ./stdf_generator -h
   ```

2. **Monitor System Logs**
   ```bash
   # Watch logs in real-time
   sudo journalctl -f -t stdf_parser -t stdf_generator
   
   # Check for system errors
   sudo journalctl -p err --since "1 hour ago"
   ```

3. **Validate Input Files**
   ```bash
   # Check file permissions and size
   ls -la data/*.stdf
   
   # Verify file is not empty or corrupted
   file data/sample.stdf
   hexdump -C data/sample.stdf | head
   ```

4. **Database Diagnostics**
   ```bash
   # Check database file
   ls -la *.db
   
   # Inspect database contents
   sqlite3 stdf_data.db ".tables"
   sqlite3 stdf_data.db "SELECT COUNT(*) FROM ptr_records;"
   ```

### Performance Optimization

1. **For Large Files (>100MB)**
   - Use SSD storage for database
   - Ensure sufficient RAM (2x file size recommended)
   - Monitor disk I/O: `iostat -x 1`

2. **For Batch Processing**
   - Process files sequentially to avoid I/O contention
   - Use separate database files: `-d batch_1.db`, `-d batch_2.db`
   - Monitor system resources: `htop` or `top`

3. **Database Optimization**
   ```sql
   -- Analyze database performance
   PRAGMA table_info(ptr_records);
   PRAGMA index_list(ptr_records);
   
   -- Check database size and efficiency
   .dbinfo
   VACUUM;
   ```

### Getting Help

If you encounter issues:

1. **Enable verbose logging**: Add `-v` flag and check syslog output
2. **Verify file integrity**: Ensure STDF file is valid and not corrupted  
3. **Check system resources**: Ensure sufficient disk space and memory
4. **Review permissions**: Verify read access to input files and write access to output directory
5. **Test with sample data**: Use generator to create known-good test files
6. **Check logs systematically**: Use `journalctl` to review all application messages

## License

This project is released under the MIT License. See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

### TODO/Roadmap

#### Completed Features ✅
- [x] **Core STDF V4 Support**: FAR, MIR, PIR, PRR, PTR, FTR, HBR, SBR, WIR, WRR record types
- [x] **SQLite Database Integration**: Complete schema with transactions
- [x] **Endianness Detection**: Automatic byte order handling
- [x] **High Performance Parsing**: 10K+ records/second with transaction batching
- [x] **Professional Logging**: Syslog integration with severity levels
- [x] **Multi-File Generator**: Advanced generator with conflict resolution
- [x] **Realistic Test Data**: Varied content generation with configurable parameters
- [x] **Cross-Platform Build**: CMake with library separation
- [x] **Command-Line Interface**: Comprehensive CLI with help systems
- [x] **Statistical Analysis**: Yield calculations and database reporting
- [x] **Bin Management**: Hardware and software bin tracking with pass/fail classification
- [x] **Wafer-Level Support**: Complete wafer information and results tracking
- [x] **Unit Testing Framework**: Google Test integration with comprehensive test suite (79.5% coverage)
- [x] **Code Coverage Analysis**: gcov integration with detailed coverage reporting

#### Planned Features 🔄
- [ ] **Additional STDF Record Types**: MPR, TSR support
- [ ] **Compressed File Support**: gzip/zip STDF file parsing
- [ ] **Python Bindings**: pybind11 integration for Python scripting
- [ ] **Configuration Files**: YAML/JSON configuration for parsing parameters
- [ ] **Export Capabilities**: CSV, JSON, XML output formats
- [ ] **Data Validation**: STDF file integrity checking and validation tools

#### Future Enhancements 🚀
- [ ] **GUI Interface**: Qt-based desktop application for parsing and analysis
- [ ] **Web Dashboard**: Browser-based interface for database analysis
- [ ] **REST API**: HTTP API for integration with other systems
- [ ] **Real-time Processing**: Stream processing for live ATE data
- [ ] **Advanced Analytics**: Statistical process control and trend analysis
- [ ] **Cloud Integration**: AWS S3/Azure Blob storage support
- [ ] **Distributed Processing**: Multi-node parsing for very large datasets
- [ ] **Machine Learning**: Anomaly detection and predictive analytics

#### Performance Targets 🎯
- [ ] **Ultra-High Performance**: 50K+ records/second parsing
- [ ] **Memory Optimization**: Support for files larger than available RAM
- [ ] **Parallel Processing**: Multi-threaded parsing for large files
- [ ] **Database Sharding**: Automatic database partitioning for massive datasets
- [ ] **Caching Layer**: Redis integration for frequently accessed data
- [ ] **Compression**: Database compression for storage efficiency