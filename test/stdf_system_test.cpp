// Copyright (C) 2025 ComputingStudios
// Project Director: Sushanth Sivaram
// Author: AI Assistant (Claude-4-Sonnet) with Cursor IDE
// Generated: July 2025
//
// Comprehensive Google Test suite for the STDF system
// Covers: STDFRecord types, STDFParser, Database, Logger, and integration

#include <gtest/gtest.h>
#include "stdf_types.h"
#include "stdf_parser.h"
#include "database.h"
#include "logger.h"
#include <fstream>
#include <filesystem>

using namespace STDF;

// Helper: Create a temporary database file path
auto temp_db_path = []() {
    std::string path = "test_temp_" + std::to_string(rand()) + ".db";
    return path;
};

// === STDFRecord Type Tests ===
TEST(STDFRecordTest, FARRecordToStringAndSize) {
    FARRecord rec;
    rec.CPU_TYP = 1;
    rec.STDF_VER = 4;
    EXPECT_EQ(rec.getRecordType(), RecordType::FAR);
    EXPECT_EQ(rec.getSize(), 2u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("FAR Record"), std::string::npos);
}

TEST(STDFRecordTest, MIRRecordToStringAndSize) {
    MIRRecord rec{};
    rec.LOT_ID = "LOT123";
    rec.PART_TYP = "TYPEA";
    EXPECT_EQ(rec.getRecordType(), RecordType::MIR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("MIR Record"), std::string::npos);
}

TEST(STDFRecordTest, PIRRecordToStringAndSize) {
    PIRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    EXPECT_EQ(rec.getRecordType(), RecordType::PIR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("PIR Record"), std::string::npos);
}

TEST(STDFRecordTest, PRRRecordToStringAndSize) {
    PRRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    rec.PART_FLG = 0;
    rec.NUM_TEST = 10;
    EXPECT_EQ(rec.getRecordType(), RecordType::PRR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("PRR Record"), std::string::npos);
}

TEST(STDFRecordTest, PTRRecordToStringAndSize) {
    PTRRecord rec;
    rec.TEST_NUM = 100;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    rec.TEST_FLG = 0;
    rec.RESULT = 3.14f;
    EXPECT_EQ(rec.getRecordType(), RecordType::PTR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("PTR Record"), std::string::npos);
}

TEST(STDFRecordTest, FTRRecordToStringAndSize) {
    FTRRecord rec;
    rec.TEST_NUM = 200;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    rec.TEST_FLG = 0;
    EXPECT_EQ(rec.getRecordType(), RecordType::FTR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("FTR Record"), std::string::npos);
}

TEST(STDFRecordTest, HBRRecordToStringAndSize) {
    HBRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    rec.HBIN_NUM = 1;
    rec.HBIN_CNT = 100;
    rec.HBIN_PF = 'P';
    rec.HBIN_NAM = "PASS_BIN";
    EXPECT_EQ(rec.getRecordType(), RecordType::HBR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("HBR Record"), std::string::npos);
}

TEST(STDFRecordTest, SBRRecordToStringAndSize) {
    SBRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_NUM = 2;
    rec.SBIN_NUM = 1;
    rec.SBIN_CNT = 100;
    rec.SBIN_PF = 'P';
    rec.SBIN_NAM = "PASS_BIN";
    EXPECT_EQ(rec.getRecordType(), RecordType::SBR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("SBR Record"), std::string::npos);
}

TEST(STDFRecordTest, WIRRecordToStringAndSize) {
    WIRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_GRP = 0;
    rec.START_T = 1234567890;
    rec.WAFER_ID = "WAFER_001";
    EXPECT_EQ(rec.getRecordType(), RecordType::WIR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("WIR Record"), std::string::npos);
}

TEST(STDFRecordTest, WRRRecordToStringAndSize) {
    WRRRecord rec;
    rec.HEAD_NUM = 1;
    rec.SITE_GRP = 0;
    rec.FINISH_T = 1234567890;
    rec.PART_CNT = 100;
    rec.RTST_CNT = 0;
    rec.ABRT_CNT = 0;
    rec.GOOD_CNT = 95;
    rec.FUNC_CNT = 90;
    EXPECT_EQ(rec.getRecordType(), RecordType::WRR);
    EXPECT_GT(rec.getSize(), 0u);
    std::string s = rec.toString();
    EXPECT_NE(s.find("WRR Record"), std::string::npos);
}

// === Logger Tests (header-only, syslog side effects not checked) ===
TEST(LoggerTest, InfoWarningErrorDebug) {
    Logger::init("test_logger");
    Logger::info("Info message");
    Logger::warning("Warning message");
    Logger::error("Error message");
    Logger::debug("Debug message");
    Logger::cleanup();
    // Stream interface
    Logger::log_info() << "Stream info" << std::endl;
    Logger::log_warning() << "Stream warning" << std::endl;
    Logger::log_error() << "Stream error" << std::endl;
    Logger::log_debug() << "Stream debug" << std::endl;
}

// === Database Tests ===
TEST(DatabaseTest, OpenCloseCreateTables) {
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    EXPECT_TRUE(db.open());
    EXPECT_TRUE(db.createTables());
    db.close();
    std::filesystem::remove(dbPath);
}

TEST(DatabaseTest, InsertAndQueryRecords) {
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    ASSERT_TRUE(db.open());
    ASSERT_TRUE(db.createTables());
    FARRecord far;
    far.CPU_TYP = 1;
    far.STDF_VER = 4;
    EXPECT_TRUE(db.insertFAR(far));
    MIRRecord mir{}; mir.LOT_ID = "L"; mir.PART_TYP = "P";
    EXPECT_TRUE(db.insertMIR(mir));
    EXPECT_GT(db.getRecordCount("far_records"), 0);
    EXPECT_GT(db.getRecordCount("mir_records"), 0);
    db.close();
    std::filesystem::remove(dbPath);
}

TEST(DatabaseTest, InsertAllRecordTypes) {
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    ASSERT_TRUE(db.open());
    ASSERT_TRUE(db.createTables());
    
    // Insert one of each record type
    PIRRecord pir; pir.HEAD_NUM = 1; pir.SITE_NUM = 1;
    EXPECT_TRUE(db.insertPIR(pir));
    
    PRRRecord prr; prr.HEAD_NUM = 1; prr.SITE_NUM = 1; prr.PART_FLG = 0; prr.NUM_TEST = 5;
    EXPECT_TRUE(db.insertPRR(prr));
    
    PTRRecord ptr; ptr.TEST_NUM = 100; ptr.HEAD_NUM = 1; ptr.SITE_NUM = 1; ptr.TEST_FLG = 0; ptr.RESULT = 1.5f;
    EXPECT_TRUE(db.insertPTR(ptr));
    
    FTRRecord ftr; ftr.TEST_NUM = 200; ftr.HEAD_NUM = 1; ftr.SITE_NUM = 1; ftr.TEST_FLG = 0;
    EXPECT_TRUE(db.insertFTR(ftr));
    
    HBRRecord hbr; hbr.HEAD_NUM = 1; hbr.SITE_NUM = 1; hbr.HBIN_NUM = 1; hbr.HBIN_CNT = 50; hbr.HBIN_PF = 'P';
    EXPECT_TRUE(db.insertHBR(hbr));
    
    SBRRecord sbr; sbr.HEAD_NUM = 1; sbr.SITE_NUM = 1; sbr.SBIN_NUM = 1; sbr.SBIN_CNT = 50; sbr.SBIN_PF = 'P';
    EXPECT_TRUE(db.insertSBR(sbr));
    
    WIRRecord wir; wir.HEAD_NUM = 1; wir.SITE_GRP = 0; wir.START_T = 1234567890; wir.WAFER_ID = "W001";
    EXPECT_TRUE(db.insertWIR(wir));
    
    WRRRecord wrr; wrr.HEAD_NUM = 1; wrr.SITE_GRP = 0; wrr.FINISH_T = 1234567890; wrr.PART_CNT = 100;
    EXPECT_TRUE(db.insertWRR(wrr));
    
    // Verify all tables have records
    EXPECT_GT(db.getRecordCount("pir_records"), 0);
    EXPECT_GT(db.getRecordCount("prr_records"), 0);
    EXPECT_GT(db.getRecordCount("ptr_records"), 0);
    EXPECT_GT(db.getRecordCount("ftr_records"), 0);
    EXPECT_GT(db.getRecordCount("hbr_records"), 0);
    EXPECT_GT(db.getRecordCount("sbr_records"), 0);
    EXPECT_GT(db.getRecordCount("wir_records"), 0);
    EXPECT_GT(db.getRecordCount("wrr_records"), 0);
    
    db.close();
    std::filesystem::remove(dbPath);
}

TEST(DatabaseTest, TransactionSupport) {
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    ASSERT_TRUE(db.open());
    ASSERT_TRUE(db.createTables());
    
    EXPECT_TRUE(db.beginTransaction());
    FARRecord far; far.CPU_TYP = 1; far.STDF_VER = 4;
    EXPECT_TRUE(db.insertFAR(far));
    EXPECT_TRUE(db.commitTransaction());
    
    EXPECT_GT(db.getRecordCount("far_records"), 0);
    
    db.close();
    std::filesystem::remove(dbPath);
}

TEST(DatabaseTest, GetTestStatistics) {
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    ASSERT_TRUE(db.open());
    ASSERT_TRUE(db.createTables());
    
    // Insert some test data - hard_bin = 1 means pass, others fail
    for (int i = 0; i < 10; ++i) {
        PRRRecord prr;
        prr.HEAD_NUM = 1;
        prr.SITE_NUM = 1;
        prr.PART_FLG = 0;
        prr.NUM_TEST = 5;
        prr.HARD_BIN = (i < 8) ? 1 : 2; // 8 pass (bin 1), 2 fail (bin 2)
        prr.SOFT_BIN = prr.HARD_BIN;
        db.insertPRR(prr);
    }
    
    auto stats = db.getTestStatistics();
    EXPECT_EQ(stats.totalParts, 10);
    EXPECT_EQ(stats.passedParts, 8);
    EXPECT_EQ(stats.failedParts, 2);
    EXPECT_NEAR(stats.yieldPercent, 80.0, 0.1);
    
    db.close();
    std::filesystem::remove(dbPath);
}

// === STDFParser Tests ===
TEST(STDFParserTest, ParseSampleFile) {
    // Use a real sample file from data/
    std::string sample = "../data/benchmark.stdf";
    if (!std::filesystem::exists(sample)) GTEST_SKIP();
    STDFParser parser(sample);
    auto records = parser.parseFile();
    EXPECT_GT(records.size(), 0u);
    // Check at least one record of each major type
    bool hasFAR = false, hasMIR = false, hasPIR = false, hasPRR = false;
    for (const auto& rec : records) {
        if (rec->getRecordType() == RecordType::FAR) hasFAR = true;
        if (rec->getRecordType() == RecordType::MIR) hasMIR = true;
        if (rec->getRecordType() == RecordType::PIR) hasPIR = true;
        if (rec->getRecordType() == RecordType::PRR) hasPRR = true;
    }
    EXPECT_TRUE(hasFAR);
    EXPECT_TRUE(hasMIR);
    EXPECT_TRUE(hasPIR);
    EXPECT_TRUE(hasPRR);
}

TEST(STDFParserTest, FileNotFound) {
    EXPECT_THROW(STDFParser parser("nonexistent_file.stdf"), std::runtime_error);
}

TEST(STDFParserTest, GetFileInfo) {
    std::string sample = "../data/benchmark.stdf";
    if (!std::filesystem::exists(sample)) GTEST_SKIP();
    STDFParser parser(sample);
    EXPECT_EQ(parser.getFilename(), sample);
    EXPECT_GT(parser.getFileSize(), 0u);
    EXPECT_EQ(parser.getCurrentPosition(), 0u);
}

TEST(STDFParserTest, StreamingParse) {
    std::string sample = "../data/benchmark.stdf";
    if (!std::filesystem::exists(sample)) GTEST_SKIP();
    STDFParser parser(sample);
    
    int recordCount = 0;
    while (!parser.isEndOfFile()) {
        auto record = parser.parseNextRecord();
        if (record) {
            recordCount++;
            EXPECT_GT(record->getSize(), 0u);
            EXPECT_FALSE(record->toString().empty());
        }
    }
    EXPECT_GT(recordCount, 0);
}

// === Integration Test: Parse and Insert All Records ===
TEST(SystemIntegrationTest, ParseAndInsertAllRecords) {
    std::string sample = "../data/benchmark.stdf";
    if (!std::filesystem::exists(sample)) GTEST_SKIP();
    std::string dbPath = temp_db_path();
    Database db(dbPath);
    ASSERT_TRUE(db.open());
    ASSERT_TRUE(db.createTables());
    STDFParser parser(sample);
    auto records = parser.parseFile();
    int inserted = 0;
    for (const auto& rec : records) {
        if (db.insertRecord(*rec)) ++inserted;
    }
    EXPECT_EQ(inserted, records.size());
    db.close();
    std::filesystem::remove(dbPath);
}

// === Main ===
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}