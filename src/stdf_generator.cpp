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
 * Description: Multi-file generator with conflict resolution
 *              Advanced STDF file generator with automatic filename management and varied content
 */

#include "stdf_types.h"
#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <filesystem>
#include <sstream>

namespace STDF {

class STDFGenerator {
public:
    STDFGenerator() : gen_(std::random_device{}()) {}
    
    std::string generateUniqueFilename(const std::string& baseFilename) {
        std::filesystem::path basePath(baseFilename);
        std::string stem = basePath.stem().string();
        std::string extension = basePath.extension().string();
        std::string directory = basePath.parent_path().string();
        
        if (directory.empty()) {
            directory = ".";
        }
        
        std::string uniqueFilename = baseFilename;
        int counter = 1;
        
        while (std::filesystem::exists(uniqueFilename)) {
            std::ostringstream oss;
            oss << directory << "/" << stem << "_" << counter << extension;
            uniqueFilename = oss.str();
            counter++;
        }
        
        return uniqueFilename;
    }
    
    bool generateSampleFile(const std::string& filename, int lotNumber = 1, int waferNumber = 1) {
        std::string uniqueFilename = generateUniqueFilename(filename);
        
        std::ofstream file(uniqueFilename, std::ios::binary);
        if (!file.is_open()) {
            STDF_LOG_ERROR << "Failed to create file: " << uniqueFilename;
            return false;
        }
        
        // Generate sample STDF file with variations
        writeFAR(file);
        writeMIR(file, lotNumber, waferNumber);
        writeWIR(file, waferNumber);
        
        // Generate test data with some randomization
        int numParts = 8 + (gen_() % 5); // 8-12 parts
        int numTests = 3 + (gen_() % 5); // 3-7 tests per part
        
        uint32_t passedParts = 0;
        uint32_t failedParts = 0;
        
        for (int part = 1; part <= numParts; ++part) {
            writePIR(file, part);
            
            // Generate test results
            for (int test = 1; test <= numTests; ++test) {
                writePTR(file, test, part, lotNumber);
            }
            
            writePRR(file, part, numTests);
            
            // Track pass/fail for bin records (simple simulation based on random)
            if ((gen_() % 10) != 0) { // 90% pass rate
                passedParts++;
            } else {
                failedParts++;
            }
        }
        
        // Write bin summary records
        if (passedParts > 0) {
            writeHBR(file, 1, passedParts, true);  // Hardware bin 1 - pass
            writeSBR(file, 1, passedParts, true);  // Software bin 1 - pass
        }
        if (failedParts > 0) {
            writeHBR(file, 2, failedParts, false); // Hardware bin 2 - fail
            writeSBR(file, 2, failedParts, false); // Software bin 2 - fail
        }
        
        // Close wafer-level information
        writeWRR(file, waferNumber, numParts, passedParts);
        
        file.close();
        STDF_LOG_INFO << "Generated sample STDF file: " << uniqueFilename;
        return true;
    }
    
    bool generateMultipleFiles(const std::string& baseFilename, int count) {
        STDF_LOG_INFO << "Generating " << count << " STDF files with base name: " << baseFilename;
        
        int successCount = 0;
        for (int i = 1; i <= count; ++i) {
            if (generateSampleFile(baseFilename, i, i)) {
                successCount++;
            } else {
                STDF_LOG_WARNING << "Failed to generate file " << i << " of " << count;
            }
        }
        
        STDF_LOG_INFO << "Successfully generated " << successCount << " out of " << count << " files";
        return successCount == count;
    }

private:
    std::mt19937 gen_;
    
    void writeU1(std::ofstream& file, uint8_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeU2(std::ofstream& file, uint16_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeU4(std::ofstream& file, uint32_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeI2(std::ofstream& file, int16_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeI4(std::ofstream& file, int32_t value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeR4(std::ofstream& file, float value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    void writeC1(std::ofstream& file, char value) {
        file.write(&value, sizeof(value));
    }
    
    void writeCn(std::ofstream& file, const std::string& value) {
        writeU1(file, static_cast<uint8_t>(value.length()));
        if (!value.empty()) {
            file.write(value.c_str(), value.length());
        }
    }
    
    void writeRecordHeader(std::ofstream& file, uint16_t length, uint8_t type, uint8_t subtype) {
        writeU2(file, length);
        writeU1(file, type);
        writeU1(file, subtype);
    }
    
    void writeFAR(std::ofstream& file) {
        writeRecordHeader(file, 2, 0, 10); // FAR record
        writeU1(file, 2);  // CPU_TYP (Intel format)
        writeU1(file, 4);  // STDF_VER (Version 4)
    }
    
    void writeMIR(std::ofstream& file, int lotNumber = 1, int waferNumber = 1) {
        // Calculate approximate record length
        uint16_t length = 4 + 4 + 1 + 1 + 1 + 1 + 2 + 1; // Fixed fields
        
        // Generate varied lot and part names
        std::string lotId = "TEST_LOT_" + std::to_string(lotNumber).insert(0, 3 - std::to_string(lotNumber).length(), '0');
        std::string partType = "PART_TYPE_" + std::to_string((lotNumber % 5) + 1);
        std::string jobName = "JOB_" + std::to_string(lotNumber).insert(0, 3 - std::to_string(lotNumber).length(), '0');
        std::string waferInfo = "WFR_" + std::to_string(waferNumber);
        
        std::vector<std::string> strings = {
            lotId, partType, "TESTER_NODE", "ATE_TESTER",
            jobName, "REV_1.0", "", "OPERATOR", "EXEC_SW", "VER_2.1",
            "PROD", "25C", "Sample test data " + waferInfo, "", "QFN48", "FAMILY_A",
            "2024" + std::to_string(lotNumber).insert(0, 3 - std::to_string(lotNumber).length(), '0'), 
            "FAB_1", "FLOOR_2", "PROC_90NM", "1GHZ",
            "SPEC_V1", "VER_1.0", "FLOW_PROD", "SETUP_A", "REV_B",
            "ENG_LOT", "ROM_001", "SN_12345", "SUPERVISOR"
        };
        
        for (const auto& str : strings) {
            length += 1 + str.length(); // Length byte + string content
        }
        
        writeRecordHeader(file, length, 1, 10); // MIR record
        
        writeU4(file, static_cast<uint32_t>(std::time(nullptr))); // SETUP_T
        writeU4(file, static_cast<uint32_t>(std::time(nullptr))); // START_T
        writeU1(file, 1);  // STAT_NUM
        writeC1(file, 'P'); // MODE_COD
        writeC1(file, ' '); // RTST_COD
        writeC1(file, ' '); // PROT_COD
        writeU2(file, 0);  // BURN_TIM
        writeC1(file, ' '); // CMOD_COD
        
        // Write all the strings
        for (const auto& str : strings) {
            writeCn(file, str);
        }
    }
    
    void writePIR(std::ofstream& file, int partNum) {
        writeRecordHeader(file, 2, 5, 10); // PIR record
        writeU1(file, 1);  // HEAD_NUM
        writeU1(file, 1);  // SITE_NUM
    }
    
    void writePTR(std::ofstream& file, int testNum, int partNum, int lotNumber = 1) {
        std::string testName = "TEST_" + std::to_string(testNum) + "_LOT" + std::to_string(lotNumber);
        uint16_t length = 4 + 1 + 1 + 1 + 1 + 4; // Fixed fields
        length += 1 + testName.length(); // TEST_TXT
        length += 1; // ALARM_ID (empty)
        length += 1; // OPT_FLAG
        
        writeRecordHeader(file, length, 15, 10); // PTR record
        
        writeU4(file, testNum);  // TEST_NUM
        writeU1(file, 1);  // HEAD_NUM
        writeU1(file, 1);  // SITE_NUM
        writeU1(file, 0);  // TEST_FLG
        writeU1(file, 0);  // PARM_FLG
        
        // Generate test result with variation based on lot and part
        std::normal_distribution<float> dist(1.0f + (lotNumber * 0.01f), 0.1f + (partNum * 0.005f));
        float result = dist(gen_);
        
        // Occasionally introduce some failing tests
        if ((gen_() % 20) == 0) { // 5% chance of failure
            result = 0.5f; // Below typical pass threshold
        }
        
        writeR4(file, result); // RESULT
        
        writeCn(file, testName); // TEST_TXT
        writeCn(file, ""); // ALARM_ID
        writeU1(file, 0);  // OPT_FLAG
    }
    
    void writePRR(std::ofstream& file, int partNum, int numTests) {
        std::string partId = "PART_" + std::to_string(partNum);
        uint16_t length = 1 + 1 + 1 + 2 + 2 + 2 + 2 + 2 + 4; // Fixed fields
        length += 1 + partId.length(); // PART_ID
        length += 1; // PART_TXT (empty)
        length += 2; // PART_FIX (empty binary)
        
        writeRecordHeader(file, length, 5, 20); // PRR record
        
        writeU1(file, 1);  // HEAD_NUM
        writeU1(file, 1);  // SITE_NUM
        writeU1(file, 0);  // PART_FLG
        writeU2(file, numTests);  // NUM_TEST
        
        // Randomly assign some parts as fail (10% chance)
        bool isPassed = (gen_() % 10) != 0; // 90% pass rate
        uint16_t hardBin = isPassed ? 1 : 2; // 1=pass, 2=fail
        uint16_t softBin = isPassed ? 1 : 2;
        
        writeU2(file, hardBin);  // HARD_BIN
        writeU2(file, softBin);  // SOFT_BIN
        writeI2(file, partNum % 10); // X_COORD
        writeI2(file, partNum / 10); // Y_COORD
        
        // Vary test time based on number of tests and random factor
        uint32_t testTime = (numTests * 200) + (gen_() % 500); // 200ms per test + random
        writeU4(file, testTime); // TEST_T
        
        writeCn(file, partId); // PART_ID
        writeCn(file, ""); // PART_TXT
        writeU2(file, 0); // PART_FIX (empty binary data)
    }
    
    void writeWIR(std::ofstream& file, int waferNumber = 1) {
        std::string waferId = "WFR_" + std::to_string(waferNumber).insert(0, 3 - std::to_string(waferNumber).length(), '0');
        uint16_t length = 1 + 1 + 4 + 1 + waferId.length(); // Fixed fields + string
        
        writeRecordHeader(file, length, 2, 10); // WIR record
        writeU1(file, 1); // HEAD_NUM
        writeU1(file, 1); // SITE_GRP
        writeU4(file, static_cast<uint32_t>(std::time(nullptr))); // START_T
        writeCn(file, waferId); // WAFER_ID
    }
    
    void writeWRR(std::ofstream& file, int waferNumber = 1, uint32_t totalParts = 10, uint32_t goodParts = 9) {
        std::string waferId = "WFR_" + std::to_string(waferNumber).insert(0, 3 - std::to_string(waferNumber).length(), '0');
        std::string fabWfId = "FAB_" + std::to_string(waferNumber);
        uint16_t length = 1 + 1 + 4 + 4 + 4 + 4 + 4 + 4; // Fixed fields
        length += 1 + waferId.length(); // WAFER_ID
        length += 1 + fabWfId.length(); // FABWF_ID
        length += 1; // FRAME_ID (empty)
        length += 1; // MASK_ID (empty)
        length += 1; // USR_DESC (empty)
        length += 1; // EXC_DESC (empty)
        
        writeRecordHeader(file, length, 2, 20); // WRR record
        writeU1(file, 1); // HEAD_NUM
        writeU1(file, 1); // SITE_GRP
        writeU4(file, static_cast<uint32_t>(std::time(nullptr))); // FINISH_T
        writeU4(file, totalParts); // PART_CNT
        writeU4(file, 0); // RTST_CNT (no retests)
        writeU4(file, 0); // ABRT_CNT (no aborts)
        writeU4(file, goodParts); // GOOD_CNT
        writeU4(file, goodParts); // FUNC_CNT (assume good = functional)
        writeCn(file, waferId); // WAFER_ID
        writeCn(file, fabWfId); // FABWF_ID
        writeCn(file, ""); // FRAME_ID
        writeCn(file, ""); // MASK_ID
        writeCn(file, ""); // USR_DESC
        writeCn(file, ""); // EXC_DESC
    }
    
    void writeHBR(std::ofstream& file, uint16_t binNum, uint32_t binCount, bool isPass = true) {
        std::string binName = isPass ? ("PASS_BIN_" + std::to_string(binNum)) : ("FAIL_BIN_" + std::to_string(binNum));
        uint16_t length = 1 + 1 + 2 + 4 + 1 + 1 + binName.length(); // Fixed fields + string
        
        writeRecordHeader(file, length, 1, 40); // HBR record
        writeU1(file, 1); // HEAD_NUM
        writeU1(file, 1); // SITE_NUM
        writeU2(file, binNum); // HBIN_NUM
        writeU4(file, binCount); // HBIN_CNT
        writeC1(file, isPass ? 'P' : 'F'); // HBIN_PF
        writeCn(file, binName); // HBIN_NAM
    }
    
    void writeSBR(std::ofstream& file, uint16_t binNum, uint32_t binCount, bool isPass = true) {
        std::string binName = isPass ? ("SOFT_PASS_" + std::to_string(binNum)) : ("SOFT_FAIL_" + std::to_string(binNum));
        uint16_t length = 1 + 1 + 2 + 4 + 1 + 1 + binName.length(); // Fixed fields + string
        
        writeRecordHeader(file, length, 1, 50); // SBR record
        writeU1(file, 1); // HEAD_NUM
        writeU1(file, 1); // SITE_NUM
        writeU2(file, binNum); // SBIN_NUM
        writeU4(file, binCount); // SBIN_CNT
        writeC1(file, isPass ? 'P' : 'F'); // SBIN_PF
        writeCn(file, binName); // SBIN_NAM
    }
};

} // namespace STDF

void printUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " [options] [output_file]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -h, --help      Show this help message\n";
    std::cout << "  -n, --count <N> Generate N STDF files (default: 1)\n";
    std::cout << "\nArguments:\n";
    std::cout << "  output_file     Output filename (default: data/sample.stdf)\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << "                           # Generate single file: data/sample.stdf\n";
    std::cout << "  " << programName << " -n 5                      # Generate 5 files with unique names\n";
    std::cout << "  " << programName << " -n 3 test/data.stdf       # Generate 3 files starting with test/data.stdf\n";
    std::cout << "\nNote: If files already exist, new names will be automatically generated (e.g., file_1.stdf, file_2.stdf)\n";
}

int main(int argc, char* argv[]) {
    std::string baseFilename = "data/sample.stdf";
    int fileCount = 1;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-n" || arg == "--count") {
            if (i + 1 < argc) {
                try {
                    fileCount = std::stoi(argv[++i]);
                    if (fileCount <= 0) {
                        std::cerr << "Error: File count must be positive\n";
                        return 1;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid file count: " << argv[i] << "\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: --count requires a number\n";
                return 1;
            }
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        } else {
            baseFilename = arg;
        }
    }
    
    // Initialize logging
    STDF::Logger::init("stdf_generator");
    
    STDF_LOG_INFO << "STDF Sample File Generator starting";
    STDF_LOG_INFO << "Base filename: " << baseFilename;
    STDF_LOG_INFO << "Number of files to generate: " << fileCount;
    
    STDF::STDFGenerator generator;
    
    bool success;
    if (fileCount == 1) {
        success = generator.generateSampleFile(baseFilename);
    } else {
        success = generator.generateMultipleFiles(baseFilename, fileCount);
    }
    
    if (success) {
        STDF_LOG_INFO << "File generation completed successfully!";
        STDF::Logger::cleanup();
        return 0;
    } else {
        STDF_LOG_ERROR << "File generation failed";
        STDF::Logger::cleanup();
        return 1;
    }
}