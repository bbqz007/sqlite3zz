
#include "template.h"
#include "zqlite3.h"

using namespace std;
using namespace zhelper::zqlite3;
static char _1[] = "SSSSS";
void test_template()
{
    //st_temp_string<"SSSSS">::extract();

    //st_temp_string<_1>::extract();

    select_para<double> a("double");

    select_para<char[4]> b("char[4]");
#if 1
    auto ztbl = make_zqlite3_table(
        select_para<double>("double"),
        select_para<int>("int"),
        select_para<char[4]>("char[4]"),
        select_para<wstring>("wchar_t[4]"),
        select_para<unsigned char[4]>("blob"));

    vector<char> blob;

    cout << ztbl.create_table_statment("some table") << endl;
    cout << ztbl.insert_statment("some table") << endl;
    cout << ztbl.select_statment("some table") << endl;
    cout << ztbl.update_statment("some table") << endl;

    ztbl.open_db("db");
    ztbl.create_table("table1");
    auto oz = ztbl.insert_into("table1");
    pair<const char*, size_t> pr;
    string str;
    wstring wstr;
    oz.begin_trans();
    //// syntax << 
    oz << 1 << 1 << "char" << L"wchar" << make_pair(blob.data(), blob.size()) << ios_base::end >> cout;
    oz << 2.f << 1 << "char" << L"wchar" << make_pair(blob.data(), blob.size()) << ios_base::end >> cout;
    oz << 3 << 1 << "char" << wstr << make_pair(blob.data(), blob.size()) << ios_base::end >> cout;
    oz << 4. << 1 << str << wstr << blob << ios_base::end >> cout;
    
    //// syntax |
    oz | 11 | 12. | "11" | L"12." | make_pair(blob.data(), blob.size()) | ios_base::end | cout;
    oz | 12 | 12. | "11" | L"12." | make_pair(blob.data(), blob.size()) | ios_base::end | cout;
    oz | 13 | 12. | "11" | wstr   | make_pair(blob.data(), blob.size()) | ios_base::end | cout;
    oz | 14 | 12. | str  | wstr   | blob                                | ios_base::end | cout;

    //// syntax (, , , ...)
    oz(1, 1, u8"z\u6c34\U0001d10b", L"wchar", {blob.data(), blob.size()})
    (2, 2, "string", L"wchar", make_pair(blob.data(), blob.size()))
    (3, 3, str, wstr, {blob.data(), blob.data() + blob.size()});
    oz.commit_trans();

    cout << string(u8"z\u6c34\U0001d10b").size() << endl;

    auto oz2 = ztbl.update_where("table1", "WHERE `char[4]`< ?");
    oz2.begin_trans();
    oz2 << 1 << 1 << "char update" << L"wchar update" << make_pair(blob.data(), blob.size()) << where_para<int>(2) << ios_base::end >> cout;
    oz2.commit_trans();

    {
        //// select statment would not lock the database until finalized.
        auto iz4 = ztbl.select_from("table1");
        iz4 >> ios_base::beg;
        auto row = ztbl.create_row();
        while (!iz4.eof())
        {
            iz4 >> ios_base::beg
#if 0
            >> std::get<0>(row)
            >> std::get<1>(row)
            >> std::get<2>(row)
            >> std::get<3>(row)
            >> std::get<4>(row)
#else
            >> row 
#endif
            >> ios_base::end >> cout;
            cout << std::get<0>(row) << ", "
            << std::get<1>(row) << ", "
            << std::get<2>(row) << ", "
            << endl;
        }
    }

#endif
#if 1
    auto cnttbl = make_zqlite3_table(select_para<int>("count(0)").expr());
    cnttbl.open_db("db");
    {
        //// select statment would not lock the database until finalized.
        auto iz6 = cnttbl.select_from("table1");
        int cnt = 0;
        iz6 >> ios_base::beg >> cnt >> ios_base::end >> cout;
        cout << cnt << endl;
    }
    index_para("1", "2");
#endif
#if 1
    auto tbl2 = make_zqlite3_table(
        select_para<int>("1"),
        select_para<string>("2"),
        select_para<double>("dbl"),
        select_para<vector<char>>("blob"),                           
        index_para("1", "2"),
        index_para("1", "2"));

    cout << (string("") + '\0' + '\0' + "abc").size() << endl;
    cout << tbl2.create_table_statment("abc") << endl;
    cout << tbl2.insert_statment("abc") << endl;
    cout << tbl2.update_statment("abc") << endl;
    cout << tbl2.select_statment("abc") << endl;
    tbl2.open_db("db");
    tbl2.create_table("abc");
    {
        auto oz = tbl2.insert_into("abc");
        auto ou = tbl2.update_where("abc", "where `1`=? AND `2` = ?"); 
        oz(1, "2", 4., {0, 0});
        oz << 2 << "e" << 1. << make_pair((char*)NULL, 0) << ios_base::end >> cout;
        if (oz.ignored())
            ou << 2 << "e" << 1. << make_pair((char*)NULL, 0) << where_para<int>(2) << where_para<string>("e") << ios_base::end >> cout;
    }
#endif
    auto join = make_zqlite3_table(
        select_para<int>("a.`1`").expr(),
        select_para<int>("b.`double`").expr());
    cout << join.select_statment2("abc a, table1 b") << endl;
    join.open_db("db");
    {
        auto ij = join.select_from2("abc a, table1 b limit 1");
        auto row = join.create_row();
        ij >> ios_base::beg;
        for (; !ij.eof(); ij >> ios_base::beg)
        {
            ij >> std::get<0>(row)
                >> std::get<1>(row)
                >> ios_base::end
                >> cout;
            cout << std::get<0>(row) << ", " 
                << std::get<1>(row) << endl;
        }
    }
    
    auto schema = make_zqlite3_table(select_para<string>("table_info(table1)"));
    schema.open_db("db");
    {
        auto isch = schema.pragma();
        auto row = schema.create_row();
        isch >> ios_base::beg;
        for (; !isch.eof(); isch >> ios_base::beg)
        {
            isch >> std::get<0>(row)
                >> cout;
            cout << std::get<0>(row) << endl;
        }
    }
    
    auto ztock = make_zqlite3_table(
			select_para<int>("DATE"),
			select_para<double>("OPEN"),
			select_para<double>("HIGH"),
			select_para<double>("LOW"),
			select_para<double>("CLOSE"),
			select_para<int>("VOL"),
			index_para("DATE"));
    
    ztock.open_db("stock.db");
    ztock.create_table("sh000001");
    auto oztk = ztock.insert_into("sh000001");	
}

template<typename... Ts1, typename... Ts2, typename Tp = std::tuple<Ts1...> > 
void capture(Tp*, Ts1... ts1, Ts2... ts2)
{
    auto t1 = std::make_tuple(ts1...);
    auto t2 = std::make_tuple(ts2...);
   // for_each_of_tuple(t1, [](auto& i) { cout << i << ","; });
   // cout << endl;
    for_each_of_tuple(t2, [](auto& i) { cout << i << ","; });
    cout << endl;
}

void test_parameter_pack()
{
    std::tuple<> a;
    cout << std::tuple_size<decltype(a)>::value << endl;
    std::tuple<int, int, int, int>* tp = 0;
    capture(tp, 1, 2, 3, 4, "stf", "std");
}

int main()
{
    test_parameter_pack();
    test_template();
    return 0;
}
