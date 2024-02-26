# sqlite3zz
* header only, template and inline.
* for sqlite3 c++ programming, help you **get out from sql syntax and c apis**. 
* easy way binding parameters, and leave from how many `?` to bind. 
* help you generate sql commands
  * maybe you forgot the sql syntax
  * and do not want to read the manual document again.

--------------------------------------
## how does sqlite3zz design
* wrap query procedure flow
* wrap insert procedure flow
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
