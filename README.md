# sqlite3zz
for sqlite3 , a c++11 stream style helper. help you get out from sql syntax and c apis. easy binding parameters, and leave from how many `?` to bind. help you generate sql commands when you forgot the sql syntax and do not want to read the manual document again.

### classes
* select_para<T>
* index_para
* where_para
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
  
### dataset
```
  auto tbl2 = make_zqlite3_table(
        select_para<int>("1"),
        select_para<string>("2"),
        select_para<double>("dbl"),
        select_para<vector<char>>("blob"),                           
        index_para("1", "2"),
        index_para("1", "2"));
```

