/*
 * Copyright (C) 2025 ComputingStudios
 * 
 * Project Director: Sushanth Sivaram
 * Author: AI Assistant (Claude-4-Sonnet) with Cursor IDE
 * Generated: July 2025
 * 
 * This file is part of the STDF Parser project, a high-performance C++ library
 * for parsing Standard Test Data Format files used in semiconductor testing.
 *
 * This software was generated using Cursor IDE with Claude-4-Sonnet AI assistance,
 * under the direction of Sushanth Sivaram, providing advanced code generation 
 * and optimization capabilities.
 *
 * Licensed under the MIT License. See LICENSE file for details.
 * 
 * Description: Parser application with CLI
 *              Main STDF parser application with command-line interface and syslog integration
 */

#include "stdf_parser.h"
#include "database.h"
#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

void printUsage(const std::string& programName) {
    // Usage information should still go to stdout for help command
    std::cout << "Usage: " << programName << " [options] <stdf_file>\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -h, --help      Show this help message\n";
    std::cout << "  -d, --database  Specify database file (default: stdf_data.db)\n";
    std::cout << "  -v, --verbose   Enable verbose output\n";
    std::cout << "  -s, --stats     Show statistics after parsing\n";
    std::cout << "\nExample:\n";
    std::cout << "  " << programName << " -d test.db -v -s data/sample.stdf\n";
}

void printStatistics(const STDF::Database& db) {
    STDF_LOG_INFO << "=== Database Statistics ===";
    
    auto stats = db.getTestStatistics();
    STDF_LOG_INFO << "Total Parts: " << stats.totalParts;
    STDF_LOG_INFO << "Passed Parts: " << stats.passedParts;
    STDF_LOG_INFO << "Failed Parts: " << stats.failedParts;
    STDF_LOG_INFO << "Yield: " << std::fixed << std::setprecision(2) << stats.yieldPercent << "%";
    STDF_LOG_INFO << "Total Tests: " << stats.totalTests;
    
    STDF_LOG_INFO << "=== Record Counts ===";
    STDF_LOG_INFO << "FAR Records: " << db.getRecordCount("far_records");
    STDF_LOG_INFO << "MIR Records: " << db.getRecordCount("mir_records");
    STDF_LOG_INFO << "PIR Records: " << db.getRecordCount("pir_records");
    STDF_LOG_INFO << "PRR Records: " << db.getRecordCount("prr_records");
    STDF_LOG_INFO << "PTR Records: " << db.getRecordCount("ptr_records");
    STDF_LOG_INFO << "FTR Records: " << db.getRecordCount("ftr_records");
    STDF_LOG_INFO << "HBR Records: " << db.getRecordCount("hbr_records");
    STDF_LOG_INFO << "SBR Records: " << db.getRecordCount("sbr_records");
    STDF_LOG_INFO << "WIR Records: " << db.getRecordCount("wir_records");
    STDF_LOG_INFO << "WRR Records: " << db.getRecordCount("wrr_records");
    
    auto lots = db.getAvailableLots();
    if (!lots.empty()) {
        STDF_LOG_INFO << "=== Available Lots ===";
        for (const auto& lot : lots) {
            STDF_LOG_INFO << "  " << lot;
        }
    }
    
    auto partTypes = db.getAvailablePartTypes();
    if (!partTypes.empty()) {
        STDF_LOG_INFO << "=== Available Part Types ===";
        for (const auto& partType : partTypes) {
            STDF_LOG_INFO << "  " << partType;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string stdfFile;
    std::string dbFile = "stdf_data.db";
    bool verbose = false;
    bool showStats = false;
    
    // Initialize logging
    STDF::Logger::init("stdf_parser");
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            STDF::Logger::cleanup();
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-d" || arg == "--database") {
            if (i + 1 < argc) {
                dbFile = argv[++i];
            } else {
                STDF_LOG_ERROR << "Error: --database requires a filename";
                STDF::Logger::cleanup();
                return 1;
            }
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "-s" || arg == "--stats") {
            showStats = true;
        } else if (arg[0] == '-') {
            STDF_LOG_ERROR << "Unknown option: " << arg;
            STDF::Logger::cleanup();
            printUsage(argv[0]);
            return 1;
        } else {
            if (stdfFile.empty()) {
                stdfFile = arg;
            } else {
                STDF_LOG_ERROR << "Error: Multiple STDF files specified";
                STDF::Logger::cleanup();
                return 1;
            }
        }
    }
    
    if (stdfFile.empty()) {
        STDF_LOG_ERROR << "Error: No STDF file specified";
        STDF::Logger::cleanup();
        printUsage(argv[0]);
        return 1;
    }
    
    STDF_LOG_INFO << "STDF Parser v1.0 starting";
    STDF_LOG_INFO << "Input file: " << stdfFile;
    STDF_LOG_INFO << "Database: " << dbFile;
    STDF_LOG_INFO << "Verbose: " << (verbose ? "Yes" : "No");
    
    try {
        // Initialize database
        STDF::Database database(dbFile);
        if (!database.open()) {
            STDF_LOG_ERROR << "Error: Failed to open database: " << database.getLastError();
            STDF::Logger::cleanup();
            return 1;
        }
        
        if (!database.createTables()) {
            STDF_LOG_ERROR << "Error: Failed to create database tables: " << database.getLastError();
            STDF::Logger::cleanup();
            return 1;
        }
        
        if (verbose) {
            STDF_LOG_DEBUG << "Database initialized successfully";
        }
        
        // Initialize parser
        STDF::STDFParser parser(stdfFile);
        
        if (verbose) {
            STDF_LOG_DEBUG << "File size: " << parser.getFileSize() << " bytes";
            STDF_LOG_DEBUG << "Starting to parse...";
        }
        
        // Start timing
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Begin database transaction for better performance
        if (!database.beginTransaction()) {
            STDF_LOG_WARNING << "Warning: Failed to begin transaction: " << database.getLastError();
        }
        
        // Parse records and insert into database
        size_t recordCount = 0;
        size_t insertedCount = 0;
        
        while (!parser.isEndOfFile()) {
            auto record = parser.parseNextRecord();
            if (record) {
                recordCount++;
                
                if (verbose && recordCount % 1000 == 0) {
                    STDF_LOG_DEBUG << "Processed " << recordCount << " records...";
                }
                
                if (database.insertRecord(*record)) {
                    insertedCount++;
                } else {
                    if (verbose) {
                        STDF_LOG_WARNING << "Warning: Failed to insert record " << recordCount 
                                  << ": " << database.getLastError();
                    }
                }
                
                // Optional: log record details in verbose mode
                if (verbose && recordCount <= 10) {
                    STDF_LOG_DEBUG << "Record " << recordCount << ": " << record->toString();
                }
            }
        }
        
        // Commit transaction
        if (!database.commitTransaction()) {
            STDF_LOG_WARNING << "Warning: Failed to commit transaction: " << database.getLastError();
        }
        
        // End timing
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        STDF_LOG_INFO << "=== Parsing Complete ===";
        STDF_LOG_INFO << "Total records parsed: " << recordCount;
        STDF_LOG_INFO << "Records inserted: " << insertedCount;
        STDF_LOG_INFO << "Processing time: " << duration.count() << " ms";
        
        if (recordCount > 0) {
            double recordsPerSecond = (recordCount * 1000.0) / duration.count();
            STDF_LOG_INFO << "Processing rate: " << std::fixed << std::setprecision(1) 
                      << recordsPerSecond << " records/second";
        }
        
        if (showStats) {
            printStatistics(database);
        }
        
    } catch (const std::exception& e) {
        STDF_LOG_ERROR << "Error: " << e.what();
        STDF::Logger::cleanup();
        return 1;
    }
    
    STDF::Logger::cleanup();
    return 0;
}