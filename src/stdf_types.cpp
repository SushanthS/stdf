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
 * Description: Record serialization and string formatting
 *              Implementation of toString() and getSize() methods for STDF records
 */

#include "stdf_types.h"
#include <sstream>
#include <iomanip>

namespace STDF {

// FARRecord implementation
std::string FARRecord::toString() const {
    std::ostringstream oss;
    oss << "FAR Record:\n";
    oss << "  CPU_TYP: " << static_cast<int>(CPU_TYP) << "\n";
    oss << "  STDF_VER: " << static_cast<int>(STDF_VER) << "\n";
    return oss.str();
}

// MIRRecord implementation
std::string MIRRecord::toString() const {
    std::ostringstream oss;
    oss << "MIR Record:\n";
    oss << "  SETUP_T: " << SETUP_T << "\n";
    oss << "  START_T: " << START_T << "\n";
    oss << "  STAT_NUM: " << static_cast<int>(STAT_NUM) << "\n";
    oss << "  MODE_COD: " << MODE_COD << "\n";
    oss << "  RTST_COD: " << RTST_COD << "\n";
    oss << "  PROT_COD: " << PROT_COD << "\n";
    oss << "  BURN_TIM: " << BURN_TIM << "\n";
    oss << "  CMOD_COD: " << CMOD_COD << "\n";
    oss << "  LOT_ID: \"" << LOT_ID << "\"\n";
    oss << "  PART_TYP: \"" << PART_TYP << "\"\n";
    oss << "  NODE_NAM: \"" << NODE_NAM << "\"\n";
    oss << "  TSTR_TYP: \"" << TSTR_TYP << "\"\n";
    oss << "  JOB_NAM: \"" << JOB_NAM << "\"\n";
    oss << "  JOB_REV: \"" << JOB_REV << "\"\n";
    oss << "  SBLOT_ID: \"" << SBLOT_ID << "\"\n";
    oss << "  OPER_NAM: \"" << OPER_NAM << "\"\n";
    oss << "  EXEC_TYP: \"" << EXEC_TYP << "\"\n";
    oss << "  EXEC_VER: \"" << EXEC_VER << "\"\n";
    oss << "  TEST_COD: \"" << TEST_COD << "\"\n";
    oss << "  TST_TEMP: \"" << TST_TEMP << "\"\n";
    oss << "  USER_TXT: \"" << USER_TXT << "\"\n";
    oss << "  AUX_FILE: \"" << AUX_FILE << "\"\n";
    oss << "  PKG_TYP: \"" << PKG_TYP << "\"\n";
    oss << "  FAMLY_ID: \"" << FAMLY_ID << "\"\n";
    oss << "  DATE_COD: \"" << DATE_COD << "\"\n";
    oss << "  FACIL_ID: \"" << FACIL_ID << "\"\n";
    oss << "  FLOOR_ID: \"" << FLOOR_ID << "\"\n";
    oss << "  PROC_ID: \"" << PROC_ID << "\"\n";
    oss << "  OPER_FRQ: \"" << OPER_FRQ << "\"\n";
    oss << "  SPEC_NAM: \"" << SPEC_NAM << "\"\n";
    oss << "  SPEC_VER: \"" << SPEC_VER << "\"\n";
    oss << "  FLOW_ID: \"" << FLOW_ID << "\"\n";
    oss << "  SETUP_ID: \"" << SETUP_ID << "\"\n";
    oss << "  DSGN_REV: \"" << DSGN_REV << "\"\n";
    oss << "  ENG_ID: \"" << ENG_ID << "\"\n";
    oss << "  ROM_COD: \"" << ROM_COD << "\"\n";
    oss << "  SERL_NUM: \"" << SERL_NUM << "\"\n";
    oss << "  SUPR_NAM: \"" << SUPR_NAM << "\"\n";
    return oss.str();
}

size_t MIRRecord::getSize() const {
    // Calculate variable size based on string lengths
    size_t fixedSize = 4 + 4 + 1 + 1 + 1 + 1 + 2 + 1; // Fixed fields
    size_t stringSize = LOT_ID.length() + PART_TYP.length() + NODE_NAM.length() +
                       TSTR_TYP.length() + JOB_NAM.length() + JOB_REV.length() +
                       SBLOT_ID.length() + OPER_NAM.length() + EXEC_TYP.length() +
                       EXEC_VER.length() + TEST_COD.length() + TST_TEMP.length() +
                       USER_TXT.length() + AUX_FILE.length() + PKG_TYP.length() +
                       FAMLY_ID.length() + DATE_COD.length() + FACIL_ID.length() +
                       FLOOR_ID.length() + PROC_ID.length() + OPER_FRQ.length() +
                       SPEC_NAM.length() + SPEC_VER.length() + FLOW_ID.length() +
                       SETUP_ID.length() + DSGN_REV.length() + ENG_ID.length() +
                       ROM_COD.length() + SERL_NUM.length() + SUPR_NAM.length();
    return fixedSize + stringSize + 30; // 30 length bytes for 30 strings
}

// PIRRecord implementation
std::string PIRRecord::toString() const {
    std::ostringstream oss;
    oss << "PIR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    return oss.str();
}

// PRRRecord implementation
std::string PRRRecord::toString() const {
    std::ostringstream oss;
    oss << "PRR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    oss << "  PART_FLG: " << static_cast<int>(PART_FLG) << "\n";
    oss << "  NUM_TEST: " << NUM_TEST << "\n";
    oss << "  HARD_BIN: " << HARD_BIN << "\n";
    oss << "  SOFT_BIN: " << SOFT_BIN << "\n";
    oss << "  X_COORD: " << X_COORD << "\n";
    oss << "  Y_COORD: " << Y_COORD << "\n";
    oss << "  TEST_T: " << TEST_T << "\n";
    oss << "  PART_ID: \"" << PART_ID << "\"\n";
    oss << "  PART_TXT: \"" << PART_TXT << "\"\n";
    return oss.str();
}

size_t PRRRecord::getSize() const {
    return 1 + 1 + 1 + 2 + 2 + 2 + 2 + 2 + 4 + 
           PART_ID.length() + 1 + PART_TXT.length() + 1 + PART_FIX.size() + 2;
}

// PTRRecord implementation
std::string PTRRecord::toString() const {
    std::ostringstream oss;
    oss << "PTR Record:\n";
    oss << "  TEST_NUM: " << TEST_NUM << "\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    oss << "  TEST_FLG: 0x" << std::hex << static_cast<int>(TEST_FLG) << std::dec << "\n";
    oss << "  PARM_FLG: 0x" << std::hex << static_cast<int>(PARM_FLG) << std::dec << "\n";
    oss << "  RESULT: " << std::fixed << std::setprecision(6) << RESULT << "\n";
    oss << "  TEST_TXT: \"" << TEST_TXT << "\"\n";
    oss << "  ALARM_ID: \"" << ALARM_ID << "\"\n";
    if (OPT_FLAG & 0x01) {
        oss << "  RES_SCAL: " << static_cast<int>(RES_SCAL) << "\n";
    }
    if (OPT_FLAG & 0x06) {
        oss << "  LLM_SCAL: " << static_cast<int>(LLM_SCAL) << "\n";
        oss << "  LO_LIMIT: " << std::fixed << std::setprecision(6) << LO_LIMIT << "\n";
    }
    if (OPT_FLAG & 0x18) {
        oss << "  HLM_SCAL: " << static_cast<int>(HLM_SCAL) << "\n";
        oss << "  HI_LIMIT: " << std::fixed << std::setprecision(6) << HI_LIMIT << "\n";
    }
    oss << "  UNITS: \"" << UNITS << "\"\n";
    return oss.str();
}

size_t PTRRecord::getSize() const {
    size_t baseSize = 4 + 1 + 1 + 1 + 1 + 4; // Fixed mandatory fields
    baseSize += TEST_TXT.length() + 1;
    baseSize += ALARM_ID.length() + 1;
    baseSize += 1; // OPT_FLAG
    
    // Optional fields based on OPT_FLAG
    if (OPT_FLAG & 0x01) baseSize += 1; // RES_SCAL
    if (OPT_FLAG & 0x06) baseSize += 1 + 4; // LLM_SCAL + LO_LIMIT
    if (OPT_FLAG & 0x18) baseSize += 1 + 4; // HLM_SCAL + HI_LIMIT
    if (OPT_FLAG & 0x20) baseSize += UNITS.length() + 1;
    if (OPT_FLAG & 0x40) baseSize += C_RESFMT.length() + 1;
    if (OPT_FLAG & 0x80) baseSize += C_LLMFMT.length() + 1 + C_HLMFMT.length() + 1;
    
    return baseSize;
}

// FTRRecord implementation
std::string FTRRecord::toString() const {
    std::ostringstream oss;
    oss << "FTR Record:\n";
    oss << "  TEST_NUM: " << TEST_NUM << "\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    oss << "  TEST_FLG: 0x" << std::hex << static_cast<int>(TEST_FLG) << std::dec << "\n";
    oss << "  OPT_FLAG: 0x" << std::hex << static_cast<int>(OPT_FLAG) << std::dec << "\n";
    oss << "  CYCL_CNT: " << CYCL_CNT << "\n";
    oss << "  REL_VADR: " << REL_VADR << "\n";
    oss << "  REPT_CNT: " << REPT_CNT << "\n";
    oss << "  NUM_FAIL: " << NUM_FAIL << "\n";
    oss << "  XFAIL_AD: " << XFAIL_AD << "\n";
    oss << "  YFAIL_AD: " << YFAIL_AD << "\n";
    oss << "  VECT_OFF: " << VECT_OFF << "\n";
    oss << "  RTN_ICNT: " << RTN_ICNT << "\n";
    oss << "  PGM_ICNT: " << PGM_ICNT << "\n";
    oss << "  VECT_NAM: \"" << VECT_NAM << "\"\n";
    oss << "  TIME_SET: \"" << TIME_SET << "\"\n";
    oss << "  OP_CODE: \"" << OP_CODE << "\"\n";
    oss << "  TEST_TXT: \"" << TEST_TXT << "\"\n";
    oss << "  ALARM_ID: \"" << ALARM_ID << "\"\n";
    oss << "  PROG_TXT: \"" << PROG_TXT << "\"\n";
    oss << "  RSLT_TXT: \"" << RSLT_TXT << "\"\n";
    oss << "  PATG_NUM: " << static_cast<int>(PATG_NUM) << "\n";
    return oss.str();
}

size_t FTRRecord::getSize() const {
    size_t baseSize = 4 + 1 + 1 + 1 + 1 + 4 + 4 + 4 + 4 + 4 + 4 + 2 + 2 + 2; // Fixed fields
    
    // Variable arrays
    baseSize += RTN_INDX.size() * 2;
    baseSize += RTN_STAT.size() * 2;
    baseSize += PGM_INDX.size() * 2;
    baseSize += PGM_STAT.size() * 2;
    baseSize += FAIL_PIN.size() + 2;
    
    // Variable strings
    baseSize += VECT_NAM.length() + 1;
    baseSize += TIME_SET.length() + 1;
    baseSize += OP_CODE.length() + 1;
    baseSize += TEST_TXT.length() + 1;
    baseSize += ALARM_ID.length() + 1;
    baseSize += PROG_TXT.length() + 1;
    baseSize += RSLT_TXT.length() + 1;
    baseSize += 1; // PATG_NUM
    baseSize += SPIN_MAP.size() + 2;
    
    return baseSize;
}

// HBRRecord implementation
std::string HBRRecord::toString() const {
    std::ostringstream oss;
    oss << "HBR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    oss << "  HBIN_NUM: " << HBIN_NUM << "\n";
    oss << "  HBIN_CNT: " << HBIN_CNT << "\n";
    oss << "  HBIN_PF: '" << HBIN_PF << "'\n";
    oss << "  HBIN_NAM: \"" << HBIN_NAM << "\"\n";
    return oss.str();
}

size_t HBRRecord::getSize() const {
    return 1 + 1 + 2 + 4 + 1 + 1 + HBIN_NAM.length(); // Fixed fields + string length byte + string
}

// SBRRecord implementation
std::string SBRRecord::toString() const {
    std::ostringstream oss;
    oss << "SBR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_NUM: " << static_cast<int>(SITE_NUM) << "\n";
    oss << "  SBIN_NUM: " << SBIN_NUM << "\n";
    oss << "  SBIN_CNT: " << SBIN_CNT << "\n";
    oss << "  SBIN_PF: '" << SBIN_PF << "'\n";
    oss << "  SBIN_NAM: \"" << SBIN_NAM << "\"\n";
    return oss.str();
}

size_t SBRRecord::getSize() const {
    return 1 + 1 + 2 + 4 + 1 + 1 + SBIN_NAM.length(); // Fixed fields + string length byte + string
}

// WIRRecord implementation
std::string WIRRecord::toString() const {
    std::ostringstream oss;
    oss << "WIR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_GRP: " << static_cast<int>(SITE_GRP) << "\n";
    oss << "  START_T: " << START_T << "\n";
    oss << "  WAFER_ID: \"" << WAFER_ID << "\"\n";
    return oss.str();
}

size_t WIRRecord::getSize() const {
    return 1 + 1 + 4 + 1 + WAFER_ID.length(); // Fixed fields + string length byte + string
}

// WRRRecord implementation
std::string WRRRecord::toString() const {
    std::ostringstream oss;
    oss << "WRR Record:\n";
    oss << "  HEAD_NUM: " << static_cast<int>(HEAD_NUM) << "\n";
    oss << "  SITE_GRP: " << static_cast<int>(SITE_GRP) << "\n";
    oss << "  FINISH_T: " << FINISH_T << "\n";
    oss << "  PART_CNT: " << PART_CNT << "\n";
    oss << "  RTST_CNT: " << RTST_CNT << "\n";
    oss << "  ABRT_CNT: " << ABRT_CNT << "\n";
    oss << "  GOOD_CNT: " << GOOD_CNT << "\n";
    oss << "  FUNC_CNT: " << FUNC_CNT << "\n";
    oss << "  WAFER_ID: \"" << WAFER_ID << "\"\n";
    oss << "  FABWF_ID: \"" << FABWF_ID << "\"\n";
    oss << "  FRAME_ID: \"" << FRAME_ID << "\"\n";
    oss << "  MASK_ID: \"" << MASK_ID << "\"\n";
    oss << "  USR_DESC: \"" << USR_DESC << "\"\n";
    oss << "  EXC_DESC: \"" << EXC_DESC << "\"\n";
    return oss.str();
}

size_t WRRRecord::getSize() const {
    size_t fixedSize = 1 + 1 + 4 + 4 + 4 + 4 + 4 + 4; // Fixed fields
    size_t stringSize = WAFER_ID.length() + FABWF_ID.length() + FRAME_ID.length() + 
                       MASK_ID.length() + USR_DESC.length() + EXC_DESC.length();
    return fixedSize + stringSize + 6; // 6 length bytes for 6 strings
}

} // namespace STDF