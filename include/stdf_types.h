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
 * Description: STDF record type definitions and base classes
 *              Defines all supported STDF record structures and data types
 */

#ifndef STDF_TYPES_H
#define STDF_TYPES_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace STDF {

// Basic STDF data types
using U1 = uint8_t;     // 1-byte unsigned integer
using U2 = uint16_t;    // 2-byte unsigned integer
using U4 = uint32_t;    // 4-byte unsigned integer
using I1 = int8_t;      // 1-byte signed integer
using I2 = int16_t;     // 2-byte signed integer
using I4 = int32_t;     // 4-byte signed integer
using R4 = float;       // 4-byte IEEE floating point
using R8 = double;      // 8-byte IEEE floating point
using C1 = char;        // 1-byte character
using Cn = std::string; // Variable length character string
using Bn = std::vector<uint8_t>; // Variable length binary data

// STDF Record Types (REC_TYP, REC_SUB)
enum class RecordType : uint8_t {
    FAR = 0,  // File Atribute Record
    ATR = 1,  // Audit Trail Record
    MIR = 2,  // Master Information Record
    MRR = 3,  // Master Results Record
    PCR = 4,  // Part Count Record
    HBR = 5,  // Hardware Bin Record
    SBR = 6,  // Software Bin Record
    PMR = 7,  // Pin Map Record
    PGR = 8,  // Pin Group Record
    PLR = 9,  // Pin List Record
    RDR = 10, // Retest Data Record
    SDR = 11, // Site Description Record
    WIR = 12, // Wafer Information Record
    WRR = 13, // Wafer Results Record
    WCR = 14, // Wafer Configuration Record
    PIR = 15, // Part Information Record
    PRR = 16, // Part Results Record
    TSR = 17, // Test Synopsis Record
    PTR = 18, // Parametric Test Record
    MPR = 19, // Multiple-Result Parametric Test Record
    FTR = 20, // Functional Test Record
    BPS = 21, // Begin Program Section Record
    EPS = 22, // End Program Section Record
    GDR = 23, // Generic Data Record
    DTR = 24  // Datalog Text Record
};

// Base class for all STDF records
class STDFRecord {
public:
    virtual ~STDFRecord() = default;
    virtual RecordType getRecordType() const = 0;
    virtual std::string toString() const = 0;
    virtual size_t getSize() const = 0;
};

// File Attribute Record (FAR)
struct FARRecord : public STDFRecord {
    U1 CPU_TYP;  // CPU type that wrote this file
    U1 STDF_VER; // STDF version number
    
    RecordType getRecordType() const override { return RecordType::FAR; }
    std::string toString() const override;
    size_t getSize() const override { return 2; }
};

// Master Information Record (MIR)
struct MIRRecord : public STDFRecord {
    U4 SETUP_T;    // Date and time of job setup
    U4 START_T;    // Date and time first part tested
    U1 STAT_NUM;   // Tester station number
    C1 MODE_COD;   // Test mode code
    C1 RTST_COD;   // Lot retest code
    C1 PROT_COD;   // Data protection code
    U2 BURN_TIM;   // Burn-in time (in minutes)
    C1 CMOD_COD;   // Command mode code
    Cn LOT_ID;     // Lot ID
    Cn PART_TYP;   // Part Type
    Cn NODE_NAM;   // Name of node that generated data
    Cn TSTR_TYP;   // Tester type
    Cn JOB_NAM;    // Job name
    Cn JOB_REV;    // Job revision number
    Cn SBLOT_ID;   // Sublot ID
    Cn OPER_NAM;   // Operator name or ID
    Cn EXEC_TYP;   // Tester executive software type
    Cn EXEC_VER;   // Tester exec software version number
    Cn TEST_COD;   // Test phase or step code
    Cn TST_TEMP;   // Test temperature
    Cn USER_TXT;   // Generic user text
    Cn AUX_FILE;   // Name of auxiliary data file
    Cn PKG_TYP;    // Package type
    Cn FAMLY_ID;   // Product family ID
    Cn DATE_COD;   // Date code
    Cn FACIL_ID;   // Test facility ID
    Cn FLOOR_ID;   // Test floor ID
    Cn PROC_ID;    // Fabrication process ID
    Cn OPER_FRQ;   // Operation frequency or step
    Cn SPEC_NAM;   // Test specification name
    Cn SPEC_VER;   // Test specification version number
    Cn FLOW_ID;    // Test flow ID
    Cn SETUP_ID;   // Test setup ID
    Cn DSGN_REV;   // Device design revision
    Cn ENG_ID;     // Engineering lot ID
    Cn ROM_COD;    // ROM code ID
    Cn SERL_NUM;   // Tester serial number
    Cn SUPR_NAM;   // Supervisor name or ID
    
    RecordType getRecordType() const override { return RecordType::MIR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Part Information Record (PIR)
struct PIRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    
    RecordType getRecordType() const override { return RecordType::PIR; }
    std::string toString() const override;
    size_t getSize() const override { return 2; }
};

// Part Results Record (PRR)
struct PRRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    U1 PART_FLG;   // Part information flag
    U2 NUM_TEST;   // Number of tests executed
    U2 HARD_BIN;   // Hardware bin number
    U2 SOFT_BIN;   // Software bin number
    I2 X_COORD;    // Wafer X coordinate
    I2 Y_COORD;    // Wafer Y coordinate
    U4 TEST_T;     // Elapsed test time in milliseconds
    Cn PART_ID;    // Part identification
    Cn PART_TXT;   // Part description text
    Bn PART_FIX;   // Part repair information
    
    RecordType getRecordType() const override { return RecordType::PRR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Parametric Test Record (PTR)
struct PTRRecord : public STDFRecord {
    U4 TEST_NUM;   // Test number
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    U1 TEST_FLG;   // Test flags
    U1 PARM_FLG;   // Parametric flags
    R4 RESULT;     // Test result
    Cn TEST_TXT;   // Test description
    Cn ALARM_ID;   // Name of alarm
    U1 OPT_FLAG;   // Optional data flag
    I1 RES_SCAL;   // Test results scaling exponent
    I1 LLM_SCAL;   // Low limit scaling exponent
    I1 HLM_SCAL;   // High limit scaling exponent
    R4 LO_LIMIT;   // Low test limit value
    R4 HI_LIMIT;   // High test limit value
    Cn UNITS;      // Test units
    Cn C_RESFMT;   // ANSI C result format string
    Cn C_LLMFMT;   // ANSI C low limit format string
    Cn C_HLMFMT;   // ANSI C high limit format string
    R4 LO_SPEC;    // Low specification limit value
    R4 HI_SPEC;    // High specification limit value
    
    RecordType getRecordType() const override { return RecordType::PTR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Functional Test Record (FTR)
struct FTRRecord : public STDFRecord {
    U4 TEST_NUM;   // Test number
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    U1 TEST_FLG;   // Test flags
    U1 OPT_FLAG;   // Optional data flag
    U4 CYCL_CNT;   // Cycle count of vector
    U4 REL_VADR;   // Relative vector address
    U4 REPT_CNT;   // Repeat count of vector
    U4 NUM_FAIL;   // Number of pins with 1 or more failures
    I4 XFAIL_AD;   // X logical address of failure
    I4 YFAIL_AD;   // Y logical address of failure
    I2 VECT_OFF;   // Offset from vector of interest
    U2 RTN_ICNT;   // Count of return states or pin indexes
    U2 PGM_ICNT;   // Count of programmed states or pin indexes
    std::vector<U2> RTN_INDX; // Array of return states or pin indexes
    std::vector<U2> RTN_STAT; // Array of returned states or pin data
    std::vector<U2> PGM_INDX; // Array of programmed states or pin indexes
    std::vector<U2> PGM_STAT; // Array of programmed states or pin data
    Bn FAIL_PIN;   // Failing pin bitfield
    Cn VECT_NAM;   // Vector module pattern name
    Cn TIME_SET;   // Time set name
    Cn OP_CODE;    // Vector Op Code
    Cn TEST_TXT;   // Test description
    Cn ALARM_ID;   // Name of alarm
    Cn PROG_TXT;   // Additional programmed information
    Cn RSLT_TXT;   // Additional result information
    U1 PATG_NUM;   // Pattern generator number
    Bn SPIN_MAP;   // Bit map of enabled comparators
    
    RecordType getRecordType() const override { return RecordType::FTR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Hardware Bin Record (HBR)
struct HBRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    U2 HBIN_NUM;   // Hardware bin number
    U4 HBIN_CNT;   // Number of parts in bin
    C1 HBIN_PF;    // Pass/fail indication (P/F)
    Cn HBIN_NAM;   // Name of hardware bin
    
    RecordType getRecordType() const override { return RecordType::HBR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Software Bin Record (SBR)
struct SBRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_NUM;   // Test site number
    U2 SBIN_NUM;   // Software bin number
    U4 SBIN_CNT;   // Number of parts in bin
    C1 SBIN_PF;    // Pass/fail indication (P/F)
    Cn SBIN_NAM;   // Name of software bin
    
    RecordType getRecordType() const override { return RecordType::SBR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Wafer Information Record (WIR)
struct WIRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_GRP;   // Site group number
    U4 START_T;    // Date and time first part tested
    Cn WAFER_ID;   // Wafer ID
    
    RecordType getRecordType() const override { return RecordType::WIR; }
    std::string toString() const override;
    size_t getSize() const override;
};

// Wafer Results Record (WRR)
struct WRRRecord : public STDFRecord {
    U1 HEAD_NUM;   // Test head number
    U1 SITE_GRP;   // Site group number
    U4 FINISH_T;   // Date and time last part tested
    U4 PART_CNT;   // Number of parts tested
    U4 RTST_CNT;   // Number of parts retested
    U4 ABRT_CNT;   // Number of aborts during testing
    U4 GOOD_CNT;   // Number of good (passed) parts tested
    U4 FUNC_CNT;   // Number of functional parts tested
    Cn WAFER_ID;   // Wafer ID
    Cn FABWF_ID;   // Fab wafer ID
    Cn FRAME_ID;   // Wafer frame ID
    Cn MASK_ID;    // Wafer mask ID
    Cn USR_DESC;   // Wafer description supplied by user
    Cn EXC_DESC;   // Wafer description supplied by exec
    
    RecordType getRecordType() const override { return RecordType::WRR; }
    std::string toString() const override;
    size_t getSize() const override;
};

} // namespace STDF

#endif // STDF_TYPES_H