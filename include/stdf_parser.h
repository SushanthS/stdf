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
 * Description: Binary parser with endianness detection
 *              Core STDF file parsing engine with stream processing capabilities
 */

#ifndef STDF_PARSER_H
#define STDF_PARSER_H

#include "stdf_types.h"
#include <fstream>
#include <memory>
#include <vector>

namespace STDF {

class STDFParser {
public:
    explicit STDFParser(const std::string& filename);
    ~STDFParser();

    // Parse the entire STDF file and return all records
    std::vector<std::unique_ptr<STDFRecord>> parseFile();
    
    // Parse records one by one (for streaming)
    std::unique_ptr<STDFRecord> parseNextRecord();
    
    // Check if end of file reached
    bool isEndOfFile();
    
    // Get file information
    std::string getFilename() const { return filename_; }
    size_t getFileSize() const { return fileSize_; }
    size_t getCurrentPosition();

private:
    // File handling
    std::string filename_;
    std::ifstream file_;
    size_t fileSize_;
    bool endianSwap_;

    // Binary data reading helpers
    U1 readU1();
    U2 readU2();
    U4 readU4();
    I1 readI1();
    I2 readI2();
    I4 readI4();
    R4 readR4();
    R8 readR8();
    C1 readC1();
    Cn readCn();
    Bn readBn();
    
    // Record parsing methods
    std::unique_ptr<STDFRecord> parseRecord();
    std::unique_ptr<FARRecord> parseFAR();
    std::unique_ptr<MIRRecord> parseMIR();
    std::unique_ptr<PIRRecord> parsePIR();
    std::unique_ptr<PRRRecord> parsePRR();
    std::unique_ptr<PTRRecord> parsePTR();
    std::unique_ptr<FTRRecord> parseFTR();
    std::unique_ptr<HBRRecord> parseHBR();
    std::unique_ptr<SBRRecord> parseSBR();
    std::unique_ptr<WIRRecord> parseWIR();
    std::unique_ptr<WRRRecord> parseWRR();
    
    // Utility methods
    void detectEndianness();
    U2 swapBytes(U2 value);
    U4 swapBytes(U4 value);
    void skipBytes(size_t count);
    
    // Record header structure
    struct RecordHeader {
        U2 length;     // Record length (excluding length field)
        U1 recordType; // Record type
        U1 recordSub;  // Record subtype
    };
    
    RecordHeader readRecordHeader();
};

} // namespace STDF

#endif // STDF_PARSER_H