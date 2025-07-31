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
 * Description: Binary file parsing with error handling
 *              Core STDF parsing engine with endianness detection and stream processing
 */

#include "stdf_parser.h"
#include "logger.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

namespace STDF {

STDFParser::STDFParser(const std::string& filename) 
    : filename_(filename), fileSize_(0), endianSwap_(false) {
    
    file_.open(filename, std::ios::binary);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    // Get file size
    file_.seekg(0, std::ios::end);
    fileSize_ = file_.tellg();
    file_.seekg(0, std::ios::beg);
    
    // Detect endianness from first record (should be FAR)
    detectEndianness();
}

STDFParser::~STDFParser() {
    if (file_.is_open()) {
        file_.close();
    }
}

void STDFParser::detectEndianness() {
    // Save current position
    auto pos = file_.tellg();
    
    // Read potential FAR record header
    U2 length;
    U1 recordType, recordSub;
    
    file_.read(reinterpret_cast<char*>(&length), sizeof(length));
    file_.read(reinterpret_cast<char*>(&recordType), sizeof(recordType));
    file_.read(reinterpret_cast<char*>(&recordSub), sizeof(recordSub));
    
    // Check if this looks like a valid FAR record
    // FAR record: length=2, type=0, sub=10 (or 20 for V4)
    if (length == 2 && recordType == 0 && (recordSub == 10 || recordSub == 20)) {
        endianSwap_ = false;
    } else {
        // Try with byte swapping
        length = swapBytes(length);
        if (length == 2 && recordType == 0 && (recordSub == 10 || recordSub == 20)) {
            endianSwap_ = true;
        } else {
            // Reset position and assume no swapping
            endianSwap_ = false;
        }
    }
    
    // Restore position
    file_.seekg(pos);
}

std::vector<std::unique_ptr<STDFRecord>> STDFParser::parseFile() {
    std::vector<std::unique_ptr<STDFRecord>> records;
    
    file_.seekg(0, std::ios::beg);
    
    while (!isEndOfFile()) {
        try {
            auto record = parseNextRecord();
            if (record) {
                records.push_back(std::move(record));
            }
        } catch (const std::exception& e) {
            STDF_LOG_ERROR << "Error parsing record: " << e.what();
            break;
        }
    }
    
    return records;
}

std::unique_ptr<STDFRecord> STDFParser::parseNextRecord() {
    if (isEndOfFile()) {
        return nullptr;
    }
    
    return parseRecord();
}

bool STDFParser::isEndOfFile() {
    return file_.eof() || file_.tellg() >= static_cast<std::streampos>(fileSize_);
}

size_t STDFParser::getCurrentPosition() {
    return static_cast<size_t>(file_.tellg());
}

// Binary reading helpers
U1 STDFParser::readU1() {
    U1 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

U2 STDFParser::readU2() {
    U2 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return endianSwap_ ? swapBytes(value) : value;
}

U4 STDFParser::readU4() {
    U4 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return endianSwap_ ? swapBytes(value) : value;
}

I1 STDFParser::readI1() {
    I1 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

I2 STDFParser::readI2() {
    I2 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return endianSwap_ ? static_cast<I2>(swapBytes(static_cast<U2>(value))) : value;
}

I4 STDFParser::readI4() {
    I4 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    return endianSwap_ ? static_cast<I4>(swapBytes(static_cast<U4>(value))) : value;
}

R4 STDFParser::readR4() {
    R4 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (endianSwap_) {
        U4 temp = swapBytes(*reinterpret_cast<U4*>(&value));
        value = *reinterpret_cast<R4*>(&temp);
    }
    return value;
}

R8 STDFParser::readR8() {
    R8 value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(value));
    // Note: R8 byte swapping would need special handling for cross-platform
    return value;
}

C1 STDFParser::readC1() {
    C1 value;
    file_.read(&value, sizeof(value));
    return value;
}

Cn STDFParser::readCn() {
    U1 length = readU1();
    if (length == 0) {
        return "";
    }
    
    std::string result(length, '\0');
    file_.read(&result[0], length);
    return result;
}

Bn STDFParser::readBn() {
    U2 length = readU2();
    if (length == 0) {
        return {};
    }
    
    std::vector<uint8_t> result(length);
    file_.read(reinterpret_cast<char*>(result.data()), length);
    return result;
}

STDFParser::RecordHeader STDFParser::readRecordHeader() {
    RecordHeader header;
    header.length = readU2();
    header.recordType = readU1();
    header.recordSub = readU1();
    return header;
}

std::unique_ptr<STDFRecord> STDFParser::parseRecord() {
    auto header = readRecordHeader();
    
    // Determine record type based on recordType and recordSub
    if (header.recordType == 0 && header.recordSub == 10) {
        return parseFAR();
    } else if (header.recordType == 1 && header.recordSub == 10) {
        return parseMIR();
    } else if (header.recordType == 1 && header.recordSub == 40) {
        return parseHBR();
    } else if (header.recordType == 1 && header.recordSub == 50) {
        return parseSBR();
    } else if (header.recordType == 2 && header.recordSub == 10) {
        return parseWIR();
    } else if (header.recordType == 2 && header.recordSub == 20) {
        return parseWRR();
    } else if (header.recordType == 5 && header.recordSub == 10) {
        return parsePIR();
    } else if (header.recordType == 5 && header.recordSub == 20) {
        return parsePRR();
    } else if (header.recordType == 15 && header.recordSub == 10) {
        return parsePTR();
    } else if (header.recordType == 15 && header.recordSub == 20) {
        return parseFTR();
    } else {
        // Skip unknown record
        skipBytes(header.length);
        return nullptr;
    }
}

std::unique_ptr<FARRecord> STDFParser::parseFAR() {
    auto record = std::make_unique<FARRecord>();
    record->CPU_TYP = readU1();
    record->STDF_VER = readU1();
    return record;
}

std::unique_ptr<MIRRecord> STDFParser::parseMIR() {
    auto record = std::make_unique<MIRRecord>();
    
    record->SETUP_T = readU4();
    record->START_T = readU4();
    record->STAT_NUM = readU1();
    record->MODE_COD = readC1();
    record->RTST_COD = readC1();
    record->PROT_COD = readC1();
    record->BURN_TIM = readU2();
    record->CMOD_COD = readC1();
    record->LOT_ID = readCn();
    record->PART_TYP = readCn();
    record->NODE_NAM = readCn();
    record->TSTR_TYP = readCn();
    record->JOB_NAM = readCn();
    record->JOB_REV = readCn();
    record->SBLOT_ID = readCn();
    record->OPER_NAM = readCn();
    record->EXEC_TYP = readCn();
    record->EXEC_VER = readCn();
    record->TEST_COD = readCn();
    record->TST_TEMP = readCn();
    record->USER_TXT = readCn();
    record->AUX_FILE = readCn();
    record->PKG_TYP = readCn();
    record->FAMLY_ID = readCn();
    record->DATE_COD = readCn();
    record->FACIL_ID = readCn();
    record->FLOOR_ID = readCn();
    record->PROC_ID = readCn();
    record->OPER_FRQ = readCn();
    record->SPEC_NAM = readCn();
    record->SPEC_VER = readCn();
    record->FLOW_ID = readCn();
    record->SETUP_ID = readCn();
    record->DSGN_REV = readCn();
    record->ENG_ID = readCn();
    record->ROM_COD = readCn();
    record->SERL_NUM = readCn();
    record->SUPR_NAM = readCn();
    
    return record;
}

std::unique_ptr<PIRRecord> STDFParser::parsePIR() {
    auto record = std::make_unique<PIRRecord>();
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    return record;
}

std::unique_ptr<PRRRecord> STDFParser::parsePRR() {
    auto record = std::make_unique<PRRRecord>();
    
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    record->PART_FLG = readU1();
    record->NUM_TEST = readU2();
    record->HARD_BIN = readU2();
    record->SOFT_BIN = readU2();
    record->X_COORD = readI2();
    record->Y_COORD = readI2();
    record->TEST_T = readU4();
    record->PART_ID = readCn();
    record->PART_TXT = readCn();
    record->PART_FIX = readBn();
    
    return record;
}

std::unique_ptr<PTRRecord> STDFParser::parsePTR() {
    auto record = std::make_unique<PTRRecord>();
    
    record->TEST_NUM = readU4();
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    record->TEST_FLG = readU1();
    record->PARM_FLG = readU1();
    record->RESULT = readR4();
    record->TEST_TXT = readCn();
    record->ALARM_ID = readCn();
    record->OPT_FLAG = readU1();
    
    // Optional fields based on OPT_FLAG
    if (record->OPT_FLAG & 0x01) {
        record->RES_SCAL = readI1();
    }
    if (record->OPT_FLAG & 0x06) {
        record->LLM_SCAL = readI1();
        record->LO_LIMIT = readR4();
    }
    if (record->OPT_FLAG & 0x18) {
        record->HLM_SCAL = readI1();
        record->HI_LIMIT = readR4();
    }
    if (record->OPT_FLAG & 0x20) {
        record->UNITS = readCn();
    }
    if (record->OPT_FLAG & 0x40) {
        record->C_RESFMT = readCn();
    }
    if (record->OPT_FLAG & 0x80) {
        record->C_LLMFMT = readCn();
        record->C_HLMFMT = readCn();
    }
    
    return record;
}

std::unique_ptr<FTRRecord> STDFParser::parseFTR() {
    auto record = std::make_unique<FTRRecord>();
    
    record->TEST_NUM = readU4();
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    record->TEST_FLG = readU1();
    record->OPT_FLAG = readU1();
    record->CYCL_CNT = readU4();
    record->REL_VADR = readU4();
    record->REPT_CNT = readU4();
    record->NUM_FAIL = readU4();
    record->XFAIL_AD = readI4();
    record->YFAIL_AD = readI4();
    record->VECT_OFF = readI2();
    record->RTN_ICNT = readU2();
    record->PGM_ICNT = readU2();
    
    // Read return arrays
    record->RTN_INDX.resize(record->RTN_ICNT);
    record->RTN_STAT.resize(record->RTN_ICNT);
    for (U2 i = 0; i < record->RTN_ICNT; ++i) {
        record->RTN_INDX[i] = readU2();
    }
    for (U2 i = 0; i < record->RTN_ICNT; ++i) {
        record->RTN_STAT[i] = readU2();
    }
    
    // Read program arrays
    record->PGM_INDX.resize(record->PGM_ICNT);
    record->PGM_STAT.resize(record->PGM_ICNT);
    for (U2 i = 0; i < record->PGM_ICNT; ++i) {
        record->PGM_INDX[i] = readU2();
    }
    for (U2 i = 0; i < record->PGM_ICNT; ++i) {
        record->PGM_STAT[i] = readU2();
    }
    
    record->FAIL_PIN = readBn();
    record->VECT_NAM = readCn();
    record->TIME_SET = readCn();
    record->OP_CODE = readCn();
    record->TEST_TXT = readCn();
    record->ALARM_ID = readCn();
    record->PROG_TXT = readCn();
    record->RSLT_TXT = readCn();
    record->PATG_NUM = readU1();
    record->SPIN_MAP = readBn();
    
    return record;
}

// Utility methods
U2 STDFParser::swapBytes(U2 value) {
    return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
}

U4 STDFParser::swapBytes(U4 value) {
    return ((value & 0xFF) << 24) | 
           (((value >> 8) & 0xFF) << 16) | 
           (((value >> 16) & 0xFF) << 8) | 
           ((value >> 24) & 0xFF);
}

void STDFParser::skipBytes(size_t count) {
    file_.seekg(count, std::ios::cur);
}

std::unique_ptr<HBRRecord> STDFParser::parseHBR() {
    auto record = std::make_unique<HBRRecord>();
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    record->HBIN_NUM = readU2();
    record->HBIN_CNT = readU4();
    record->HBIN_PF = readC1();
    record->HBIN_NAM = readCn();
    return record;
}

std::unique_ptr<SBRRecord> STDFParser::parseSBR() {
    auto record = std::make_unique<SBRRecord>();
    record->HEAD_NUM = readU1();
    record->SITE_NUM = readU1();
    record->SBIN_NUM = readU2();
    record->SBIN_CNT = readU4();
    record->SBIN_PF = readC1();
    record->SBIN_NAM = readCn();
    return record;
}

std::unique_ptr<WIRRecord> STDFParser::parseWIR() {
    auto record = std::make_unique<WIRRecord>();
    record->HEAD_NUM = readU1();
    record->SITE_GRP = readU1();
    record->START_T = readU4();
    record->WAFER_ID = readCn();
    return record;
}

std::unique_ptr<WRRRecord> STDFParser::parseWRR() {
    auto record = std::make_unique<WRRRecord>();
    record->HEAD_NUM = readU1();
    record->SITE_GRP = readU1();
    record->FINISH_T = readU4();
    record->PART_CNT = readU4();
    record->RTST_CNT = readU4();
    record->ABRT_CNT = readU4();
    record->GOOD_CNT = readU4();
    record->FUNC_CNT = readU4();
    record->WAFER_ID = readCn();
    record->FABWF_ID = readCn();
    record->FRAME_ID = readCn();
    record->MASK_ID = readCn();
    record->USR_DESC = readCn();
    record->EXC_DESC = readCn();
    return record;
}

} // namespace STDF