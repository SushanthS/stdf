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
 * Description: SQLite interface with transaction support
 *              Database operations and schema management for STDF records
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "stdf_types.h"
#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>

namespace STDF {

class Database {
public:
    explicit Database(const std::string& dbPath);
    ~Database();

    // Database operations
    bool open();
    void close();
    bool isOpen() const { return db_ != nullptr; }
    
    // Schema creation
    bool createTables();
    
    // Record insertion methods
    bool insertFAR(const FARRecord& record);
    bool insertMIR(const MIRRecord& record);
    bool insertPIR(const PIRRecord& record);
    bool insertPRR(const PRRRecord& record);
    bool insertPTR(const PTRRecord& record);
    bool insertFTR(const FTRRecord& record);
    bool insertHBR(const HBRRecord& record);
    bool insertSBR(const SBRRecord& record);
    bool insertWIR(const WIRRecord& record);
    bool insertWRR(const WRRRecord& record);
    
    // Generic record insertion
    bool insertRecord(const STDFRecord& record);
    
    // Batch operations
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    // Query operations
    std::vector<std::string> getAvailableLots() const;
    std::vector<std::string> getAvailablePartTypes() const;
    int getRecordCount(const std::string& tableName) const;
    
    // Statistics
    struct TestStatistics {
        int totalParts;
        int passedParts;
        int failedParts;
        double yieldPercent;
        int totalTests;
    };
    
    TestStatistics getTestStatistics() const;
    
    // Error handling
    std::string getLastError() const { return lastError_; }

private:
    std::string dbPath_;
    sqlite3* db_;
    std::string lastError_;
    
    // Helper methods
    bool executeSQL(const std::string& sql);
    bool prepareStatement(const std::string& sql, sqlite3_stmt** stmt);
    void setLastError(const std::string& error);
    void setLastSQLiteError();
    
    // Schema definitions
    static const char* CREATE_FAR_TABLE;
    static const char* CREATE_MIR_TABLE;
    static const char* CREATE_PIR_TABLE;
    static const char* CREATE_PRR_TABLE;
    static const char* CREATE_PTR_TABLE;
    static const char* CREATE_FTR_TABLE;
    static const char* CREATE_HBR_TABLE;
    static const char* CREATE_SBR_TABLE;
    static const char* CREATE_WIR_TABLE;
    static const char* CREATE_WRR_TABLE;
    
    // Insert statement definitions
    static const char* INSERT_FAR_SQL;
    static const char* INSERT_MIR_SQL;
    static const char* INSERT_PIR_SQL;
    static const char* INSERT_PRR_SQL;
    static const char* INSERT_PTR_SQL;
    static const char* INSERT_FTR_SQL;
    static const char* INSERT_HBR_SQL;
    static const char* INSERT_SBR_SQL;
    static const char* INSERT_WIR_SQL;
    static const char* INSERT_WRR_SQL;
};

} // namespace STDF

#endif // DATABASE_H