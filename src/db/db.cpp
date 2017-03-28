//
// Created by sven on 17-3-23.
//

#include "db.h"
#include "io.h"
#include "util.h"
#include "table.h"

using namespace SDB;

// ========== Public =======
void DB::create_db(const std::string &db_name) {
    IO::create_dir(db_name);
    IO::create_file(get_meta_path(db_name));
    write_meta_data(db_name, TableNameSet());
}

void DB::create_table(const SDB::Type::TableProperty &table_property) {
    Table::create_table(table_property);
}

void DB::drop_table(const SDB::Type::TableProperty &table_property) {
    Table::drop_table(table_property);
}

void DB::insert(const std::string &table_name, const Tuple &tuple) {
    Table table(db_name, table_name);
    table.insert(tuple);
}

void DB::remove(const std::string &table_name,
                const std::string &col_name,
                const Value &value) {
    Table table(db_name, table_name);
    table.remove(col_name, value);
}

void DB::remove(const std::string &table_name,
                const std::string &col_name,
                SDB::Type::BVFunc predicate) {
    Table table(db_name, table_name);
    table.remove(col_name, predicate);
}

void DB::update(const std::string &table_name,
                const std::string &pred_col_name, SDB::Type::BVFunc predicate,
                const std::string &op_col_name, SDB::Type::VVFunc op) {
    Table table(db_name, table_name);
    table.update(pred_col_name, predicate, op_col_name, op);
}

DB::TupleLst DB::find(const std::string &table_name,
                      const std::string &col_name,
                      const SDB::Type::Value &value) {
    Table table(db_name, table_name);
    return table.find(col_name, value);
}

DB::TupleLst DB::find(const std::string &table_name,
                      const std::string &col_name,
                      std::function<bool(DB::Value)> predicate) {
    Table table(db_name, table_name);
    return table.find(col_name, predicate);
}

// ========== Private =======
void DB::write_meta_data(const std::string &db_name, const TableNameSet &set) {
    Type::Bytes bytes;
    Function::bytes_append(bytes, db_name);
    Function::bytes_append(bytes, set);
    IO io(get_meta_path(db_name));
    io.write_file(bytes);
}

void DB::read_meta_data() {
    IO io(get_meta_path(db_name));
    Type::Bytes bytes = io.read_file();
    size_t offset = 0;
    Function::de_bytes(db_name, bytes, offset);
    Function::de_bytes(table_name_set, bytes, offset);
}

std::string DB::get_meta_path(const std::string &db_name) {
    return db_name + "/meta.sdb";
}
