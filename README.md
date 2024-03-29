# zqlite3 Library

The provided code is part of a C++ project named either "zqlite3" or "sqlite3zz", as indicated by the initial comment block. This project aims to simplify interactions with SQLite databases in C++ by serving as a wrapper or utility library. It encapsulates SQLite's C API into a more C++-friendly interface, leveraging modern C++ features such as templates, tuples, and smart pointers to provide a safer and more convenient way to work with SQLite databases.

## Key Components and Features

1. **Parameter Binding and Result Storing Classes**: `select_para`, `index_para`, and `where_para` are template classes designed for binding parameters to SQL statements and storing results from queries. These classes are specialized for different data types such as `int`, `int64_t`, `double`, `std::string`, `std::wstring`, and blobs.

2. **Table and Stream Classes**: `zqlite3_table` represents a database table, allowing for the binding of parameter types and columns. `zqlite3_ostream` and `zqlite3_istream` are used for parameter binding in insert/update operations and storing results from select queries, respectively.

3. **Factory Functions**: The `make_zqlite3_table` function is a factory function that creates `zqlite3_table` objects, facilitating the setup of tables with specific columns and types.

4. **Helper Metafunctions**: Utility metafunctions like `for_each_of_tuple` and `split_types` operate on tuples, aiding in the implementation of the library's functionality.

5. **Database Management**: Mechanisms for opening, closing, and managing SQLite databases (`open_db`, `close_db`, `attach_db`, `detach_db`) are provided, including executing SQL commands for operations like table creation, insertion, update, selection, and upsert.

6. **Debugging and Logging**: Conditional compilation is used to include debugging and logging capabilities (`tracout`, `wtracout`), enabling the library to output diagnostic information based on the build configuration.

7. **Error Handling**: Basic error handling capabilities are included, with classes and functions returning error codes or setting error flags upon encountering issues.

8. **Pragma and SQLite Function Execution**: Support for executing SQLite PRAGMA commands and other SQLite functions directly through the `pragma` and `select_function_only` methods.

This library abstracts much of the boilerplate and manual resource management associated with using the SQLite C API directly, offering a more type-safe and convenient interface for C++ developers working with SQLite databases.

# zqlite3 Library

提供的代码是一个名为 "zqlite3" 或 "sqlite3zz" 的 C++ 项目的一部分，如初始注释块所示。该项目旨在通过作为包装器或实用库来简化 C++ 中与 SQLite 数据库的交互。它将 SQLite 的 C API 封装到更适合 C++ 的接口中，利用现代 C++ 特性（如模板、元组和智能指针）提供了一种更安全、更方便的方式来处理 SQLite 数据库。

## 关键组件和特性

1. **参数绑定和结果存储类**：`select_para`、`index_para` 和 `where_para` 是模板类，旨在将参数绑定到 SQL 语句并存储查询结果。这些类专门针对不同的数据类型进行了特化，如 `int`、`int64_t`、`double`、`std::string`、`std::wstring` 和 blobs。

2. **表和流类**：`zqlite3_table` 表示数据库表，允许绑定参数类型和列。`zqlite3_ostream` 和 `zqlite3_istream` 用于插入/更新操作的参数绑定和存储选择查询结果。

3. **工厂函数**：`make_zqlite3_table` 函数是一个工厂函数，用于创建 `zqlite3_table` 对象，便于设置具有特定列和类型的表。

4. **辅助元函数**：像 `for_each_of_tuple` 和 `split_types` 这样的实用元函数操作元组，有助于实现库的功能。

5. **数据库管理**：提供了打开、关闭和管理 SQLite 数据库的机制（`open_db`、`close_db`、`attach_db`、`detach_db`），包括执行 SQL 命令以进行表创建、插入、更新、选择和 upsert 等操作。

6. **调试和日志记录**：使用条件编译包含调试和日志记录功能（`tracout`、`wtracout`），使库能够根据构建配置输出诊断信息。

7. **错误处理**：包括基本的错误处理功能，类和函数在遇到问题时返回错误代码或设置错误标志。

8. **Pragma 和 SQLite 函数执行**：支持通过 `pragma` 和 `select_function_only` 方法直接执行 SQLite PRAGMA 命令和其他 SQLite 函数。

该库抽象了与直接使用 SQLite C API 相关的大部分样板和手动资源管理，为 C++ 开发人员提供了一个更类型安全和方便的接口，用于处理 SQLite 数据库。

# sqlite3zz
* header only, template and inline.
* for sqlite3 c++ programming, help you **get out from sql syntax and c apis**. 
* easy way binding parameters, and leave from how many `?` to bind. 
* help you generate sql commands
  * maybe you forgot the sql syntax
  * and do not want to read the manual document again.

--------------------------------------
## how does sqlite3zz design
* wrap define a table schema or a dataset schema
* wrap query procedure flow
* wrap insert procedure flow
### wrap define a table schema or a dataset schema
```c++
/** define a table schema */
auto schema = make_zqlite3_table(
    select_para<int>("col_name1"),                        // col_name1 INT
    select_para<double>("col_name2"),                     // col_name2 REAL
    select_para<std::string>("col_vchar_name3"),          // col_vchar_name3 VCHAR
    select_para<std::vector<char>>("col_blob_name4"),     // col_blob_name4 BLOB
    index_para("col_name1").pkey().asc());                // PRIMARY KEY(col_name1, ASC)
schema.open_db("your.db");
schema.create_table("table_name");                        // use the table schema to create table on your.db
```
### wrap query procedure flow
```c++
auto schema = make_zqlite3_table(...);  // define which columns to access, or dataset
schema.open_db("your.db");
auto istream = schema.select_from("table_name");  // generate sql statment and sqlite3_prepare it.
istream >> std::ios::beg;              // calling sqlite3_step;
istream >> value1;                     // sqlite3_column_xxx, read data of first column of result record
istream >> value2;                     // sqlite3_column_xxx, read data of second column of result record
...
istream >> valueN;                     // sqlite3_column_xxx, read data of last column of result record
```
### wrap insert procedure flow
```c++
auto schema = make_zqlite3_table(...);  // define which columns to access
schema.open_db("your.db");
auto ostream = schema.insert_into("table_name");  // generate sql statment and sqlite3_prepare it.
ostream.begin_trans();
ostream << value1;                     // sqlite3_bind_xxx, bind data of first column of record
ostream << value2;                     // sqlite3_bind_xxx, bind data of second column of record
...
ostream << valueN;                     // sqlite3_bind_xxx, bind data of last column of record
ostream << std::ios::end;              // calling sqlite3_step and sqlite3_reset, loop to insert another row;
ostream.commit_trans();
```
--------------------------------------
![img](https://github.com/bbqz007/KTL/blob/master/resources/GIF_KTL_ZQLITE3_STREAMING2.gif)
### classes
* **select_para**\<FieldType\>
* **index_para**
* **where_para**\<T\>
* **zqlite3_table**<Fields...>
  * methods
    * **`create_table`**
    * **`insert_into`**, generate an ostream
    * **`update_where`**, generate an ostream
    * **`select_from`**, generate an istream
* **zqlite3_ostream**
  * operators
    * **`<<`**, insert or update one row: `oz << val1 << val2 << val3 << val4 << ios::end;`
    * **`| `**, insert or update one row like building table: `oz | val1 | val2 | val3 | val4 | ios::end;`
    * **`<< std::ios::end`**, fire `sqlite3_step`
* **zqlite3_istream**
  * operators
    * **`>>`**, read result of one row: `iz >> ios::beg >> val1 >> val2 >> val3 >> val4 >> ios:end;`
    * **`>> std::ios::beg`**, fire `sqlite3_step`
 
### usage
* just include `zqlite3.h`
* using namespace `zhelper::zqlite3`
* `c++14` at least required
  
### dataset and table
```c++
  auto tbl2 = make_zqlite3_table(
        select_para<int>("1"),
        select_para<string>("2"),
        select_para<double>("dbl"),
        select_para<vector<char>>("blob"),                           
        index_para("1", "2"),
        index_para("1", "2"));
        
   
    cout << tbl2.create_table_statment("abc") << endl;
    cout << tbl2.insert_statment("abc") << endl;
    cout << tbl2.update_statment("abc") << endl;
    cout << tbl2.select_statment("abc") << endl;
```
when you call `create_table`, that means a table.

when you call `insert_into`, `update_where` or `select_from`, that means a dataset you are operating. the subset of the whole fields of table.

```sql
CREATE TABLE IF NOT EXISTS `abc` (`1` INTEGER, `2` VARCHAR, `dbl` REAL, `blob` BLOB, UNIQUE (`1`  DESC, `2`  DESC), UNIQUE (`1`  DESC, `2`  DESC));
INSERT INTO `abc` (`1`, `2`, `dbl`, `blob`) VALUES (?,?,?,?);
UPDATE `abc` SET `1` = ?, `2` = ?, `dbl` = ?, `blob` = ?;
SELECT `1`, `2`, `dbl`, `blob` FROM `abc` ;
```
### streaming
```c++
    tbl2.open_db("db");
    tbl2.create_table("abc");
    {
        auto oz = tbl2.insert_into("abc");
        auto ou = tbl2.update_where("abc", "where `1`=? AND `2` = ?"); 
        oz.begin_trans();
        oz(1, "2", 4., {0, 0});
        oz << 2 << "e" << 1. << make_pair((char*)NULL, 0) << ios_base::end >> cout;
        if (oz.ignored())
            ou << 2 << "e" << 1. << make_pair((char*)NULL, 0) << where_para<int>(2) << where_para<string>("e") << ios_base::end >> cout;
        oz.commit_trans();    
    }
```
