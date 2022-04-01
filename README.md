# sqlite3zz
for sqlite3 , a c++11 **stream style** helper. help you **get out from sql syntax and c apis**. 

easy binding parameters, and leave from how many `?` to bind. 

help you generate sql commands when you forgot the sql syntax and do not want to read the manual document again.

--------------------------------------
![img](https://github.com/bbqz007/KTL/blob/master/resources/GIF_KTL_ZQLITE3_STREAMING2.gif)
### classes
* select_para\<FieldType\>
* index_para
* where_para\<T\>
* zqlite3_table<Fields...>
  * methods
    * create_table
    * insert_into, generate an ostream
    * update_where, generate an ostream
    * select_from, generate an istream
* zqlite3_ostream
  * operators
    * <<, insert or update one row: `oz << val1 << val2 << val3 << val4 << ios::end;`
    * |, `oz | val1 | val2 | val3 | val4 | ios::end;` like marking table
* zqlite3_istream
  * operators
    * \>\>, read result of one row: `iz >> ios::beg >> val1 >> val2 >> val3 >> val4 >> ios:end;`
 
### usage
* just include `zqlite3.h`
* using namespace `zhelper::zqlite3`
  
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
