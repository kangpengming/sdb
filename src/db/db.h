#ifndef DB_DB_H
#define DB_DB_H

#include <string>
#include <unordered_set>
#include <utility>
#include "table.h"
#include <boost/thread.hpp>
#include "../sql/ast.h"

namespace sdb {

class DB {
public:
    // type
    // using DBPtr = std::shared_ptr<DB>;
    using TablePtr = std::shared_ptr<Table>;

public:
    DB(const std::string &db_name);

    // db op
    static void create_db(const std::string &db_name);
    static void drop_db(const std::string &db_name);
    void execute(AstNodePtr ptr);

private:
    // check integrity
    // template <typename T>
    // void check_referenced(const Table &table, T t);
    // void check_referencing(const Table &table, const SDB::Type::TupleData &tuple_data);

    // add meta table to db_map
    void add_table_list();
    void add_col_list();
    // void add_index();
    // void add_reference();
    std::vector<std::string> table_name_lst(TransInfo ti);

    // tp
    TableProperty get_tp(TransInfo ti, const std::string &table_name);

    // table
    void create_table(TransInfo ti, const TableProperty &tp);
    void drop_table(TransInfo ti, const std::string &table_name);

    // transaction check
    void trans_check(TransInfo t_info);

    // op
    TransInfo begin();
    void commit(TransInfo t_info);

private:
    std::string db_name;
    // TODO concurrent map
    // TODO deadlock maybe
    // <name, tablePtr>
    std::map<std::string, boost::upgrade_mutex> mutex_map;
    std::map<std::string, TablePtr> table_map;
    // int8_t => 
    // -3 : apply upgrade_lock
    // -2 : apply hared_lock
    // -1 : apply unque_lock
    // 0  : init
    // 1  : has shared_lock
    // 2  : has unique_lock
    //
    // <transaction info, <table name, <lock info, table ptr>>>
    std::map<TransInfo, std::map<std::string, std::pair<int8_t, TablePtr>>> t_snapshot;
};

} // namespace sdb

#endif //DB_DB_H
