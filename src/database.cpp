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
 * Description: Database operations and schema management
 *              SQLite interface with transaction support and STDF record storage
 */

#include "database.h"
#include <iostream>
#include <sstream>

namespace STDF {

// Schema definitions
const char* Database::CREATE_FAR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS far_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        cpu_typ INTEGER NOT NULL,
        stdf_ver INTEGER NOT NULL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_MIR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS mir_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        setup_t INTEGER,
        start_t INTEGER,
        stat_num INTEGER,
        mode_cod TEXT,
        rtst_cod TEXT,
        prot_cod TEXT,
        burn_tim INTEGER,
        cmod_cod TEXT,
        lot_id TEXT,
        part_typ TEXT,
        node_nam TEXT,
        tstr_typ TEXT,
        job_nam TEXT,
        job_rev TEXT,
        sblot_id TEXT,
        oper_nam TEXT,
        exec_typ TEXT,
        exec_ver TEXT,
        test_cod TEXT,
        tst_temp TEXT,
        user_txt TEXT,
        aux_file TEXT,
        pkg_typ TEXT,
        famly_id TEXT,
        date_cod TEXT,
        facil_id TEXT,
        floor_id TEXT,
        proc_id TEXT,
        oper_frq TEXT,
        spec_nam TEXT,
        spec_ver TEXT,
        flow_id TEXT,
        setup_id TEXT,
        dsgn_rev TEXT,
        eng_id TEXT,
        rom_cod TEXT,
        serl_num TEXT,
        supr_nam TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_PIR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS pir_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_PRR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS prr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        part_flg INTEGER,
        num_test INTEGER,
        hard_bin INTEGER,
        soft_bin INTEGER,
        x_coord INTEGER,
        y_coord INTEGER,
        test_t INTEGER,
        part_id TEXT,
        part_txt TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_PTR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS ptr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        test_num INTEGER NOT NULL,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        test_flg INTEGER,
        parm_flg INTEGER,
        result REAL,
        test_txt TEXT,
        alarm_id TEXT,
        opt_flag INTEGER,
        res_scal INTEGER,
        llm_scal INTEGER,
        hlm_scal INTEGER,
        lo_limit REAL,
        hi_limit REAL,
        units TEXT,
        c_resfmt TEXT,
        c_llmfmt TEXT,
        c_hlmfmt TEXT,
        lo_spec REAL,
        hi_spec REAL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_FTR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS ftr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        test_num INTEGER NOT NULL,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        test_flg INTEGER,
        opt_flag INTEGER,
        cycl_cnt INTEGER,
        rel_vadr INTEGER,
        rept_cnt INTEGER,
        num_fail INTEGER,
        xfail_ad INTEGER,
        yfail_ad INTEGER,
        vect_off INTEGER,
        rtn_icnt INTEGER,
        pgm_icnt INTEGER,
        vect_nam TEXT,
        time_set TEXT,
        op_code TEXT,
        test_txt TEXT,
        alarm_id TEXT,
        prog_txt TEXT,
        rslt_txt TEXT,
        patg_num INTEGER,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_HBR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS hbr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        hbin_num INTEGER NOT NULL,
        hbin_cnt INTEGER NOT NULL,
        hbin_pf TEXT,
        hbin_nam TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_SBR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS sbr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_num INTEGER NOT NULL,
        sbin_num INTEGER NOT NULL,
        sbin_cnt INTEGER NOT NULL,
        sbin_pf TEXT,
        sbin_nam TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_WIR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS wir_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_grp INTEGER NOT NULL,
        start_t INTEGER NOT NULL,
        wafer_id TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

const char* Database::CREATE_WRR_TABLE = R"(
    CREATE TABLE IF NOT EXISTS wrr_records (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        head_num INTEGER NOT NULL,
        site_grp INTEGER NOT NULL,
        finish_t INTEGER NOT NULL,
        part_cnt INTEGER NOT NULL,
        rtst_cnt INTEGER NOT NULL,
        abrt_cnt INTEGER NOT NULL,
        good_cnt INTEGER NOT NULL,
        func_cnt INTEGER NOT NULL,
        wafer_id TEXT,
        fabwf_id TEXT,
        frame_id TEXT,
        mask_id TEXT,
        usr_desc TEXT,
        exc_desc TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
)";

// Insert statement definitions
const char* Database::INSERT_FAR_SQL = 
    "INSERT INTO far_records (cpu_typ, stdf_ver) VALUES (?, ?);";

const char* Database::INSERT_MIR_SQL = R"(
    INSERT INTO mir_records (
        setup_t, start_t, stat_num, mode_cod, rtst_cod, prot_cod, burn_tim, cmod_cod,
        lot_id, part_typ, node_nam, tstr_typ, job_nam, job_rev, sblot_id, oper_nam,
        exec_typ, exec_ver, test_cod, tst_temp, user_txt, aux_file, pkg_typ, famly_id,
        date_cod, facil_id, floor_id, proc_id, oper_frq, spec_nam, spec_ver, flow_id,
        setup_id, dsgn_rev, eng_id, rom_cod, serl_num, supr_nam
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_PIR_SQL = 
    "INSERT INTO pir_records (head_num, site_num) VALUES (?, ?);";

const char* Database::INSERT_PRR_SQL = R"(
    INSERT INTO prr_records (
        head_num, site_num, part_flg, num_test, hard_bin, soft_bin,
        x_coord, y_coord, test_t, part_id, part_txt
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_PTR_SQL = R"(
    INSERT INTO ptr_records (
        test_num, head_num, site_num, test_flg, parm_flg, result, test_txt, alarm_id,
        opt_flag, res_scal, llm_scal, hlm_scal, lo_limit, hi_limit, units,
        c_resfmt, c_llmfmt, c_hlmfmt, lo_spec, hi_spec
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_FTR_SQL = R"(
    INSERT INTO ftr_records (
        test_num, head_num, site_num, test_flg, opt_flag, cycl_cnt, rel_vadr,
        rept_cnt, num_fail, xfail_ad, yfail_ad, vect_off, rtn_icnt, pgm_icnt,
        vect_nam, time_set, op_code, test_txt, alarm_id, prog_txt, rslt_txt, patg_num
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_HBR_SQL = R"(
    INSERT INTO hbr_records (
        head_num, site_num, hbin_num, hbin_cnt, hbin_pf, hbin_nam
    ) VALUES (?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_SBR_SQL = R"(
    INSERT INTO sbr_records (
        head_num, site_num, sbin_num, sbin_cnt, sbin_pf, sbin_nam
    ) VALUES (?, ?, ?, ?, ?, ?);
)";

const char* Database::INSERT_WIR_SQL = R"(
    INSERT INTO wir_records (
        head_num, site_grp, start_t, wafer_id
    ) VALUES (?, ?, ?, ?);
)";

const char* Database::INSERT_WRR_SQL = R"(
    INSERT INTO wrr_records (
        head_num, site_grp, finish_t, part_cnt, rtst_cnt, abrt_cnt, good_cnt, func_cnt,
        wafer_id, fabwf_id, frame_id, mask_id, usr_desc, exc_desc
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
)";

Database::Database(const std::string& dbPath) : dbPath_(dbPath), db_(nullptr) {
}

Database::~Database() {
    close();
}

bool Database::open() {
    int result = sqlite3_open(dbPath_.c_str(), &db_);
    if (result != SQLITE_OK) {
        setLastSQLiteError();
        return false;
    }
    return true;
}

void Database::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool Database::createTables() {
    if (!db_) {
        setLastError("Database not open");
        return false;
    }

    // Create all tables
    if (!executeSQL(CREATE_FAR_TABLE) ||
        !executeSQL(CREATE_MIR_TABLE) ||
        !executeSQL(CREATE_PIR_TABLE) ||
        !executeSQL(CREATE_PRR_TABLE) ||
        !executeSQL(CREATE_PTR_TABLE) ||
        !executeSQL(CREATE_FTR_TABLE) ||
        !executeSQL(CREATE_HBR_TABLE) ||
        !executeSQL(CREATE_SBR_TABLE) ||
        !executeSQL(CREATE_WIR_TABLE) ||
        !executeSQL(CREATE_WRR_TABLE)) {
        return false;
    }

    return true;
}

bool Database::insertFAR(const FARRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_FAR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.CPU_TYP);
    sqlite3_bind_int(stmt, 2, record.STDF_VER);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertMIR(const MIRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_MIR_SQL, &stmt)) {
        return false;
    }

    int param = 1;
    sqlite3_bind_int(stmt, param++, record.SETUP_T);
    sqlite3_bind_int(stmt, param++, record.START_T);
    sqlite3_bind_int(stmt, param++, record.STAT_NUM);
    sqlite3_bind_text(stmt, param++, &record.MODE_COD, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, &record.RTST_COD, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, &record.PROT_COD, 1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, param++, record.BURN_TIM);
    sqlite3_bind_text(stmt, param++, &record.CMOD_COD, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.LOT_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.PART_TYP.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.NODE_NAM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.TSTR_TYP.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.JOB_NAM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.JOB_REV.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SBLOT_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.OPER_NAM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.EXEC_TYP.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.EXEC_VER.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.TEST_COD.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.TST_TEMP.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.USER_TXT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.AUX_FILE.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.PKG_TYP.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FAMLY_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.DATE_COD.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FACIL_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FLOOR_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.PROC_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.OPER_FRQ.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SPEC_NAM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SPEC_VER.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FLOW_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SETUP_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.DSGN_REV.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.ENG_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.ROM_COD.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SERL_NUM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.SUPR_NAM.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertPIR(const PIRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_PIR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.HEAD_NUM);
    sqlite3_bind_int(stmt, 2, record.SITE_NUM);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertPRR(const PRRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_PRR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.HEAD_NUM);
    sqlite3_bind_int(stmt, 2, record.SITE_NUM);
    sqlite3_bind_int(stmt, 3, record.PART_FLG);
    sqlite3_bind_int(stmt, 4, record.NUM_TEST);
    sqlite3_bind_int(stmt, 5, record.HARD_BIN);
    sqlite3_bind_int(stmt, 6, record.SOFT_BIN);
    sqlite3_bind_int(stmt, 7, record.X_COORD);
    sqlite3_bind_int(stmt, 8, record.Y_COORD);
    sqlite3_bind_int(stmt, 9, record.TEST_T);
    sqlite3_bind_text(stmt, 10, record.PART_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, record.PART_TXT.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertPTR(const PTRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_PTR_SQL, &stmt)) {
        return false;
    }

    int param = 1;
    sqlite3_bind_int(stmt, param++, record.TEST_NUM);
    sqlite3_bind_int(stmt, param++, record.HEAD_NUM);
    sqlite3_bind_int(stmt, param++, record.SITE_NUM);
    sqlite3_bind_int(stmt, param++, record.TEST_FLG);
    sqlite3_bind_int(stmt, param++, record.PARM_FLG);
    sqlite3_bind_double(stmt, param++, record.RESULT);
    sqlite3_bind_text(stmt, param++, record.TEST_TXT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.ALARM_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, param++, record.OPT_FLAG);
    sqlite3_bind_int(stmt, param++, record.RES_SCAL);
    sqlite3_bind_int(stmt, param++, record.LLM_SCAL);
    sqlite3_bind_int(stmt, param++, record.HLM_SCAL);
    sqlite3_bind_double(stmt, param++, record.LO_LIMIT);
    sqlite3_bind_double(stmt, param++, record.HI_LIMIT);
    sqlite3_bind_text(stmt, param++, record.UNITS.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.C_RESFMT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.C_LLMFMT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.C_HLMFMT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, param++, record.LO_SPEC);
    sqlite3_bind_double(stmt, param++, record.HI_SPEC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertFTR(const FTRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_FTR_SQL, &stmt)) {
        return false;
    }

    int param = 1;
    sqlite3_bind_int(stmt, param++, record.TEST_NUM);
    sqlite3_bind_int(stmt, param++, record.HEAD_NUM);
    sqlite3_bind_int(stmt, param++, record.SITE_NUM);
    sqlite3_bind_int(stmt, param++, record.TEST_FLG);
    sqlite3_bind_int(stmt, param++, record.OPT_FLAG);
    sqlite3_bind_int(stmt, param++, record.CYCL_CNT);
    sqlite3_bind_int(stmt, param++, record.REL_VADR);
    sqlite3_bind_int(stmt, param++, record.REPT_CNT);
    sqlite3_bind_int(stmt, param++, record.NUM_FAIL);
    sqlite3_bind_int(stmt, param++, record.XFAIL_AD);
    sqlite3_bind_int(stmt, param++, record.YFAIL_AD);
    sqlite3_bind_int(stmt, param++, record.VECT_OFF);
    sqlite3_bind_int(stmt, param++, record.RTN_ICNT);
    sqlite3_bind_int(stmt, param++, record.PGM_ICNT);
    sqlite3_bind_text(stmt, param++, record.VECT_NAM.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.TIME_SET.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.OP_CODE.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.TEST_TXT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.ALARM_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.PROG_TXT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.RSLT_TXT.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, param++, record.PATG_NUM);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertHBR(const HBRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_HBR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.HEAD_NUM);
    sqlite3_bind_int(stmt, 2, record.SITE_NUM);
    sqlite3_bind_int(stmt, 3, record.HBIN_NUM);
    sqlite3_bind_int(stmt, 4, record.HBIN_CNT);
    sqlite3_bind_text(stmt, 5, &record.HBIN_PF, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, record.HBIN_NAM.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertSBR(const SBRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_SBR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.HEAD_NUM);
    sqlite3_bind_int(stmt, 2, record.SITE_NUM);
    sqlite3_bind_int(stmt, 3, record.SBIN_NUM);
    sqlite3_bind_int(stmt, 4, record.SBIN_CNT);
    sqlite3_bind_text(stmt, 5, &record.SBIN_PF, 1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, record.SBIN_NAM.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertWIR(const WIRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_WIR_SQL, &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, record.HEAD_NUM);
    sqlite3_bind_int(stmt, 2, record.SITE_GRP);
    sqlite3_bind_int(stmt, 3, record.START_T);
    sqlite3_bind_text(stmt, 4, record.WAFER_ID.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertWRR(const WRRRecord& record) {
    sqlite3_stmt* stmt;
    if (!prepareStatement(INSERT_WRR_SQL, &stmt)) {
        return false;
    }

    int param = 1;
    sqlite3_bind_int(stmt, param++, record.HEAD_NUM);
    sqlite3_bind_int(stmt, param++, record.SITE_GRP);
    sqlite3_bind_int(stmt, param++, record.FINISH_T);
    sqlite3_bind_int(stmt, param++, record.PART_CNT);
    sqlite3_bind_int(stmt, param++, record.RTST_CNT);
    sqlite3_bind_int(stmt, param++, record.ABRT_CNT);
    sqlite3_bind_int(stmt, param++, record.GOOD_CNT);
    sqlite3_bind_int(stmt, param++, record.FUNC_CNT);
    sqlite3_bind_text(stmt, param++, record.WAFER_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FABWF_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.FRAME_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.MASK_ID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.USR_DESC.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, param++, record.EXC_DESC.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        setLastSQLiteError();
        return false;
    }

    return true;
}

bool Database::insertRecord(const STDFRecord& record) {
    switch (record.getRecordType()) {
        case RecordType::FAR:
            return insertFAR(static_cast<const FARRecord&>(record));
        case RecordType::MIR:
            return insertMIR(static_cast<const MIRRecord&>(record));
        case RecordType::PIR:
            return insertPIR(static_cast<const PIRRecord&>(record));
        case RecordType::PRR:
            return insertPRR(static_cast<const PRRRecord&>(record));
        case RecordType::PTR:
            return insertPTR(static_cast<const PTRRecord&>(record));
        case RecordType::FTR:
            return insertFTR(static_cast<const FTRRecord&>(record));
        case RecordType::HBR:
            return insertHBR(static_cast<const HBRRecord&>(record));
        case RecordType::SBR:
            return insertSBR(static_cast<const SBRRecord&>(record));
        case RecordType::WIR:
            return insertWIR(static_cast<const WIRRecord&>(record));
        case RecordType::WRR:
            return insertWRR(static_cast<const WRRRecord&>(record));
        default:
            setLastError("Unsupported record type for insertion");
            return false;
    }
}

bool Database::beginTransaction() {
    return executeSQL("BEGIN TRANSACTION;");
}

bool Database::commitTransaction() {
    return executeSQL("COMMIT;");
}

bool Database::rollbackTransaction() {
    return executeSQL("ROLLBACK;");
}

std::vector<std::string> Database::getAvailableLots() const {
    std::vector<std::string> lots;
    
    const char* sql = "SELECT DISTINCT lot_id FROM mir_records WHERE lot_id != '';";
    sqlite3_stmt* stmt;
    
    if (const_cast<Database*>(this)->prepareStatement(sql, &stmt)) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* lot_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (lot_id) {
                lots.push_back(lot_id);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    return lots;
}

std::vector<std::string> Database::getAvailablePartTypes() const {
    std::vector<std::string> partTypes;
    
    const char* sql = "SELECT DISTINCT part_typ FROM mir_records WHERE part_typ != '';";
    sqlite3_stmt* stmt;
    
    if (const_cast<Database*>(this)->prepareStatement(sql, &stmt)) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* part_typ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (part_typ) {
                partTypes.push_back(part_typ);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    return partTypes;
}

int Database::getRecordCount(const std::string& tableName) const {
    std::string sql = "SELECT COUNT(*) FROM " + tableName + ";";
    sqlite3_stmt* stmt;
    
    if (const_cast<Database*>(this)->prepareStatement(sql, &stmt)) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            return count;
        }
        sqlite3_finalize(stmt);
    }
    
    return -1;
}

Database::TestStatistics Database::getTestStatistics() const {
    TestStatistics stats = {0, 0, 0, 0.0, 0};
    
    // Get total parts
    sqlite3_stmt* stmt;
    if (const_cast<Database*>(this)->prepareStatement("SELECT COUNT(*) FROM prr_records;", &stmt)) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.totalParts = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    // Get passed parts (assuming hard_bin = 1 means pass)
    if (const_cast<Database*>(this)->prepareStatement("SELECT COUNT(*) FROM prr_records WHERE hard_bin = 1;", &stmt)) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.passedParts = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    // Calculate failed parts and yield
    stats.failedParts = stats.totalParts - stats.passedParts;
    if (stats.totalParts > 0) {
        stats.yieldPercent = (static_cast<double>(stats.passedParts) / stats.totalParts) * 100.0;
    }
    
    // Get total tests
    if (const_cast<Database*>(this)->prepareStatement("SELECT COUNT(*) FROM ptr_records;", &stmt)) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.totalTests = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    return stats;
}

// Helper methods
bool Database::executeSQL(const std::string& sql) {
    if (!db_) {
        setLastError("Database not open");
        return false;
    }

    char* errorMsg = nullptr;
    int result = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errorMsg);
    
    if (result != SQLITE_OK) {
        if (errorMsg) {
            setLastError(errorMsg);
            sqlite3_free(errorMsg);
        } else {
            setLastSQLiteError();
        }
        return false;
    }
    
    return true;
}

bool Database::prepareStatement(const std::string& sql, sqlite3_stmt** stmt) {
    if (!db_) {
        setLastError("Database not open");
        return false;
    }

    int result = sqlite3_prepare_v2(db_, sql.c_str(), -1, stmt, nullptr);
    if (result != SQLITE_OK) {
        setLastSQLiteError();
        return false;
    }
    
    return true;
}

void Database::setLastError(const std::string& error) {
    lastError_ = error;
}

void Database::setLastSQLiteError() {
    if (db_) {
        lastError_ = sqlite3_errmsg(db_);
    } else {
        lastError_ = "Database not open";
    }
}

} // namespace STDF