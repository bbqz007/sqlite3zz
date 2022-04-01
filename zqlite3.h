/**
 * project: zqlite3 or sqlite3zz
 * 
 * MIT License
 * Copyright (c) 2022 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef __ZhelpEr_ZQLite3_H_
#define __ZhelpEr_ZQLite3_H_
//// classes:
////    select_para, index_para, where_para
////    zqlite3_table, declares a dataset about how parameter types and columns are binding, 
////    zqlite3_ostream, binds parameters for insert and update
////    zqlite3_istream, stores results form select
//// factory functions:
////    make_zqlite3_table
//// helper metafunctions:
////    for_each_of_tuple
////    split_types 

#include <string.h>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#if 1
#include <sqlite3.h>
#else
struct sqlite3;
struct sqlite3_stmt;
#define sqlite3_open(...) 0
#define sqlite3_close(...) 0
#define sqlite3_bind_int(...) 0
#define sqlite3_bind_int64(...) 0
#define sqlite3_bind_double(...) 0
#define sqlite3_bind_text(...) 0
#define sqlite3_bind_text16(...) 0
#define sqlite3_bind_blob(...) 0
#define sqlite3_bind_zeroblob(...) 0
#define sqlite3_column_int(...) 0
#define sqlite3_column_int64(...) 0
#define sqlite3_column_double(...) 0
#define sqlite3_column_text(...) 0
#define sqlite3_column_text16(...) 0
#define sqlite3_column_blob(...) 0
#define sqlite3_step(...) 0
#define sqlite3_reset(...) 0
#define sqlite3_finalize(...) 0
#define sqlite3_prepare(...) 0
#endif // 0

namespace zhelper
{
namespace zqlite3
{

#ifdef TRACE_DEBUG
#define tracout std::cout
#define wtracout std::wcout
#else
struct null_ostream : protected std::ostream
{
    template <typename T>
    null_ostream& operator<< (T const&)
    {
        return *this;
    }
    null_ostream& operator<< (std::ostream&(*)(std::ostream&))
    {
        return *this;
    }
};

struct null_wostream : protected std::wostream
{
    template <typename T>
    null_wostream& operator<< (T const&)
    {
        return *this;
    }
    null_wostream& operator<< (std::wostream&(*)(std::wostream&))
    {
        return *this;
    }
};

null_ostream& cnulout = *(null_ostream*)&std::clog;
null_wostream& wcnulout = *(null_wostream*)&std::wclog;

#define tracout cnulout
#define wtracout wcnulout
#endif // TRACE_DEBUG

using std::ios_base;
using ios_base = std::ios_base;

typedef sqlite3 Db;
typedef sqlite3_stmt DbStmt;
/**
NULL. The value is a NULL value.

INTEGER. The value is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.

REAL. The value is a floating point value, stored as an 8-byte IEEE floating point number.

TEXT. The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).

BLOB. The value is a blob of data, stored exactly as it was input.
*/
struct select_para_base
{
    select_para_base(const char* s) : name_(s) { expr_ = 0;}
    select_para_base(std::string&& s) : name_(std::move(s)) { expr_ = 0; }
    const std::string& name() { return name_; }
    std::string name_;
    struct {
        int expr_ : 1;
    };
    int err_ = 0;
};

template<typename _T, size_t _SZ = sizeof(_T)>
struct select_para;

template<>
struct select_para<int> : public select_para_base
{
    typedef select_para<int> stype;
    typedef int datatype;
    typedef datatype datatype2;
    typedef datatype const cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "INTEGER"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_int " << el << std::endl;
        err_ = sqlite3_bind_int(stmt, idx, el);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        tracout << "call sqlite3_column_int " << el << std::endl;
        el = sqlite3_column_int(stmt, idx);
    }

    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<>
struct select_para<int64_t> : public select_para_base
{
    typedef select_para<int64_t> stype;
    typedef int64_t datatype;
    typedef datatype datatype2;
    typedef datatype const cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "INTEGER"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_int64 " << el << std::endl;
        err_ = sqlite3_bind_int64(stmt, idx, el);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        tracout << "call sqlite3_column_int64 " << el << std::endl;
        el = sqlite3_column_int64(stmt, idx);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<>
struct select_para<double> : public select_para_base
{
    typedef select_para<double> stype;
    typedef double datatype;
    typedef datatype datatype2;
    typedef datatype const cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "REAL"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_double" << std::endl;
#ifdef SHOW_WRONG_BINDING_CHANGE_THE_COLUMN_TYPE
        err_ = sqlite3_bind_text(stmt, idx, "abc", -1, 0);
#else
        err_ = sqlite3_bind_double(stmt, idx, el);
#endif
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        tracout << "call sqlite3_column_double " << el << std::endl;
        el = sqlite3_column_double(stmt, idx);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

/***
 * As documentation (of SQLite) points out all string types are stored all the same in SQLite
 * Also SQLite does not enforce lengths.
 * https://www.sqlite.org/c3ref/bind_blob.html
 * N bytes, not N characters
 * UTF8 or UTF16 only
 */
template<size_t _SZ>
struct select_para<char[_SZ], _SZ> : public select_para_base
{
    typedef select_para<char[_SZ], _SZ> stype;
    typedef char* datatype;
    typedef std::string datatype2;
    typedef const char* cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "TEXT"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_text(-1) " << el << std::endl;
        err_ = sqlite3_bind_text(stmt, idx, el, -1, 0);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        tracout << "call sqlite3_bind_text" << std::endl;
        err_ = sqlite3_bind_text(stmt, idx, el.c_str(), el.size(), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_text " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el, (void*)res, bs);
        el[bs] = 0;
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_text " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.reserve(bs);
        auto res = sqlite3_column_text(stmt, idx);
        el.assign(res, res + bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<>
struct select_para<std::string> : public select_para_base
{
    typedef select_para<std::string> stype;
    typedef char* datatype;
    typedef std::string datatype2;
    typedef const char* cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "VARCHAR"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_text(-1) " << el << std::endl;
        err_ = sqlite3_bind_text(stmt, idx, el, -1, 0);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        tracout << "call sqlite3_bind_text" << std::endl;
        err_ = sqlite3_bind_text(stmt, idx, el.c_str(), el.size(), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_text " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el, (void*)res, bs);
        el[bs] = 0;
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_text " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.reserve(bs);
        auto res = sqlite3_column_text(stmt, idx);
        el.assign(res, res + bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<size_t _SZ>
struct select_para<wchar_t[_SZ], _SZ> : public select_para_base
{
    typedef select_para<wchar_t[_SZ], _SZ> stype;
    typedef wchar_t* datatype;
    typedef std::wstring datatype2;
    typedef const wchar_t* cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "TEXT"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        wtracout << L"call sqlite3_bind_text16(-1) " << el << std::endl;
        err_ = sqlite3_bind_text16(stmt, idx, el, -1, 0);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        wtracout << L"call sqlite3_bind_text16" << el << std::endl;
        err_ = sqlite3_bind_text16(stmt, idx, el.c_str(), el.size() * sizeof(wchar_t), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_text16 " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el, (void*)res, bs);
        el[bs >> 1] = 0;
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_text16 " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.resize(bs >> 1);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el.data(), (void*)res, bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<>
struct select_para<std::wstring> : public select_para_base
{
    typedef select_para<std::wstring> stype;
    typedef wchar_t* datatype;
    typedef std::wstring datatype2;
    typedef const wchar_t* cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "VARCHAR"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        wtracout << L"call sqlite3_bind_text16(-1) " << el << std::endl;
        err_ = sqlite3_bind_text16(stmt, idx, el, -1, 0);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        wtracout << "call sqlite3_bind_text16" << el << std::endl;
        err_ = sqlite3_bind_text16(stmt, idx, el.c_str(), el.size() * sizeof(wchar_t), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_text16 " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el, (void*)res, bs);
        el[bs >> 1] = 0;
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_text16 " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.resize(bs >> 1);
        auto res = sqlite3_column_text(stmt, idx);
        memcpy((void*)el.data(), (void*)res, bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<size_t _SZ>
struct select_para<unsigned char[_SZ], _SZ> : public select_para_base
{
    typedef select_para<unsigned char[_SZ], _SZ> stype;
    typedef std::pair<const char*, size_t> datatype;
    typedef std::vector<char> datatype2;
    typedef const datatype cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "BLOB"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_blob" << std::endl;
        if (el.first)
            err_ = sqlite3_bind_blob(stmt, idx, el.first, el.second, 0);
        else
            err_ = sqlite3_bind_zeroblob(stmt, idx, el.second);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        tracout << "call sqlite3_bind_blob" << std::endl;
        err_ = sqlite3_bind_blob(stmt, idx, el.data(), el.size(), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_blob " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_blob(stmt, idx);
        el.second = std::min(el.second, (size_t)bs);
        memcpy((void*)el.first, (void*)res, el.second);
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_blob " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.reserve(bs);
        auto res = sqlite3_column_text(stmt, idx);
        el.assign(res, res + bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

template<>
struct select_para<std::vector<char>> : public select_para_base
{
    typedef select_para<std::vector<char>> stype;
    typedef std::pair<const char*, size_t> datatype;
    typedef std::vector<char> datatype2;
    typedef const datatype cdatatype;
    select_para(const char* s) : select_para_base(s) {}
    select_para(select_para&& mv) :select_para_base(std::move((select_para_base&)mv)) { }
    const char* type() { return "BLOB"; }
    void assign(DbStmt* stmt, int idx, const datatype& el)
    {
        tracout << "call sqlite3_bind_blob" << std::endl;
        if (el.first)
            err_ = sqlite3_bind_blob(stmt, idx, el.first, el.second, 0);
        else
            err_ = sqlite3_bind_zeroblob(stmt, idx, el.second);
    }
    void assign(DbStmt* stmt, int idx, const datatype2& el)
    {
        tracout << "call sqlite3_bind_blob" << std::endl;
        err_ = sqlite3_bind_blob(stmt, idx, el.data(), el.size(), 0);
    }
    void store(DbStmt* stmt, int idx, datatype& el)
    {
        //// danger
        tracout << "call sqlite3_column_blob " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        auto res = sqlite3_column_blob(stmt, idx);
        el.second = std::min(el.second, (size_t)bs);
        memcpy((void*)el.first, (void*)res, el.second);
    }
    void store(DbStmt* stmt, int idx, datatype2& el)
    {
        tracout << "call sqlite3_column_blob " << std::endl;
        auto bs = sqlite3_column_bytes(stmt, idx);
        el.reserve(bs);
        auto res = sqlite3_column_text(stmt, idx);
        el.assign(res, res + bs);
    }
    
    select_para expr() { expr_ = 1; return std::move(*this); }
};

/*** https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple */
template<size_t N, typename F, typename Tp>
typename std::enable_if<N == std::tuple_size<Tp>::value, void>::type _for_each_of_tuple(Tp& tuple, F&& f)
{

}

template<size_t N, typename F, typename Tp>
typename std::enable_if<N < std::tuple_size<Tp>::value, void>::type _for_each_of_tuple(Tp& tuple, F&& f)
{
    f(std::get<N>(tuple));
    _for_each_of_tuple<N+1>(tuple, f);
}

template<typename F, typename Tp>
void for_each_of_tuple(Tp& tuple, F&& f)
{
    f(std::get<0>(tuple));
    _for_each_of_tuple<1>(tuple, f);
}

template<typename... Ts>
struct zqlite3_table;

/////////////////////////////
//// zqlite3_istream
/////////////////////////////
template<size_t _Op, typename... _Fields>
struct zqlite3_istream;

template<typename... _Fields>
struct zqlite3_istream_base
{
    typedef std::tuple<_Fields...> _Tuple;
    struct _Err;
    zqlite3_istream_base(_Tuple& fields, _Err& err) : fields_(fields), err_(err) {}
    zqlite3_istream_base(const zqlite3_istream_base&) = default;
    zqlite3_istream_base(zqlite3_istream_base&& mv) : fields_(mv.fields_), err_(mv.err_)
    {
        std::swap(stmt_, mv.stmt_);
        std::swap(db_, mv.db_);
    }
    zqlite3_istream_base(zqlite3_istream_base&& mv, _Err& err) : fields_(mv.fields_), err_(err)
    {
        std::swap(stmt_, mv.stmt_);
        std::swap(db_, mv.db_);
    }
    _Tuple& fields_;
    struct _Err {
        int err_step_ = 0, err_reset_ = 0, err_ = 0;
    } & err_;
    DbStmt* stmt_ = 0;
    Db* db_ = 0; // weak

    void reset()
    {

    }

    int total_changes()
    {
        return (db_) ? sqlite3_total_changes(db_) : 0;
    }
};

template<typename... _Fields>
struct zqlite3_istream<(size_t)-1, _Fields...> : public zqlite3_istream_base<_Fields...>
{
    //// root of istream toshite
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_istream_base<_Fields...> _Base;
    zqlite3_istream(_Tuple& base) : _Base(base, err__) {}
    zqlite3_istream(zqlite3_istream&& base) : _Base(std::move(base), err__) {}
    zqlite3_istream(const zqlite3_istream&) = delete;
    
    typename _Base::_Err err__;

    ~zqlite3_istream()
    {
        _Db_attach(0);
    }
    void _Db_attach(DbStmt* stmt)
    {
        if (stmt)
        {
            tracout << "call reset stmt" << std::endl;
            _Base::err_.err_reset_ = sqlite3_reset(stmt);
        }
        if (_Base::stmt_)
        {
            tracout << "call finalize stmt" << std::endl;
            sqlite3_finalize(_Base::stmt_);
        }
        _Base::stmt_ = stmt;
        _Base::err_.err_step_ = _Base::err_.err_reset_ = 0;
    }

    int begin_trans()
    {
        char* errmsg = 0;
        int err = sqlite3_exec(_Base::db_, "BEGIN TRANSACTION", 0,  0, &errmsg);
        if (err)
            tracout << "begin trans err: " << errmsg << std::endl;
        return err;
    }

    int commit_trans()
    {
        char* errmsg = 0;
        int err = sqlite3_exec(_Base::db_, "END TRANSACTION", 0,  0, &errmsg);
        if (err)
            tracout << "ends trans err: " << errmsg << std::endl;
        return err;
    }

    //// syntax >>
    zqlite3_istream<sizeof...(_Fields) - 1, _Fields...>
    operator >> (typename std::tuple_element<0, _Tuple>::type::datatype & el)
    {
        zqlite3_istream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).store(_Base::stmt_, 0, el);
        return next;
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<0, _Tuple>::type::datatype, typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_istream<sizeof...(_Fields) - 1, _Fields...>
    operator >> (typename std::enable_if<not _Enable, typename std::tuple_element<0, _Tuple>::type::datatype2>::type & el)
    {
        zqlite3_istream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).store(_Base::stmt_, 0, el);
        return next;
    }
    
    zqlite3_istream<0, _Fields...>
    operator >> (std::tuple<typename _Fields::datatype2...>& tuple)
    {
        zqlite3_istream<0, _Fields...> next = std::move(iterate_tuple<0>(*this, tuple));
        next >> std::ios_base::end;
        return next;
    }

    //// syntax ( )
    zqlite3_istream&
    operator () (typename _Fields::datatype const& ... vals)
    {
        iterate (*this, vals...);
        return *this;
    }

    zqlite3_istream&
    operator () (typename _Fields::datatype2 const& ... vals, void* __unsed = 0)
    {
        iterate (*this, vals...);
        return *this;
    }

    zqlite3_istream&
    operator >> (decltype(std::ios::end) const& opcode)
    {
        if (opcode == std::ios::beg)
        {
            tracout << "call next step" << std::endl;
            _Base::err_.err_step_ = sqlite3_step(_Base::stmt_);
        }
        return *this;
    }

    bool eof()
    {
        return _Base::err_.err_step_ != SQLITE_ROW;
    }

protected:
    template<typename _Is, typename _Head, typename... _Tails>
    void iterate (_Is& Iz, _Head const& head, _Tails const&... tails)
    {
        auto next = Iz >> (head);
        return iterate (next, tails...);
    }

    void iterate (zqlite3_istream<0, _Fields...>& exec)
    {
        exec >> std::ios_base::end;
    }
    
    template<size_t N, typename _Is, typename _Tuple>
    auto iterate_tuple (_Is& Iz, _Tuple& tp, typename std::enable_if<N == std::tuple_size<_Tuple>::value, void>::type* = 0)
    {
        return std::move(Iz);
    }
    
    template<size_t N, typename _Is, typename _Tuple>
    auto iterate_tuple (_Is& Iz, _Tuple& tp, typename std::enable_if<N < std::tuple_size<_Tuple>::value, void>::type* = 0)
    {
        auto next = Iz >> std::get<N>(tp);
        return iterate_tuple<N+1> (next, tp);
    }
};

template<size_t _Op, typename... _Fields>
struct zqlite3_istream : public zqlite3_istream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_istream_base<_Fields...> _Base;
    zqlite3_istream(const _Base& base) : _Base(base) {}
    zqlite3_istream(zqlite3_istream&& base) : _Base(std::move(base)) {}

    ~zqlite3_istream()
    {
        _Base::stmt_ = 0;
    }

	zqlite3_istream<_Op - 1, _Fields...>
	operator >> (typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::datatype & el)
    {
        zqlite3_istream<_Op - 1, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - _Op>(_Base::fields_).store(_Base::stmt_, sizeof...(_Fields) - _Op + 0, el);
        return std::move(next);
    }
    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::datatype, typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
	zqlite3_istream<_Op - 1, _Fields...>
	operator >> (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::datatype2>::type & el)
    {
        zqlite3_istream<_Op - 1, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - _Op>(_Base::fields_).store(_Base::stmt_, sizeof...(_Fields) - _Op + 0, el);
        return std::move(next);
    }
};

template<typename... _Fields>
struct zqlite3_istream<1, _Fields...> : public zqlite3_istream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_istream_base<_Fields...> _Base;
    zqlite3_istream(const _Base& base) : _Base(base) {}
    zqlite3_istream(zqlite3_istream&& base) : _Base(std::move(base)) {}

    ~zqlite3_istream()
    {
        _Base::stmt_ = 0;
    }

    zqlite3_istream<0, _Fields...>
    operator >> (typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::datatype & el)
    {
        zqlite3_istream<0, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - 1>(_Base::fields_).store(_Base::stmt_, sizeof...(_Fields) - 1 + 0, el);
        return std::move(next);
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::datatype, typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_istream<0, _Fields...>
    operator >> (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::datatype2>::type & el)
    {
        zqlite3_istream<0, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - 1>(_Base::fields_).store(_Base::stmt_, sizeof...(_Fields) - 1 + 0, el);
        return std::move(next);
    }
};

template<typename... _Fields>
struct zqlite3_istream<0, _Fields...> : public zqlite3_istream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_istream_base<_Fields...> _Base;
    zqlite3_istream(const _Base& base) : _Base(base) {}
    zqlite3_istream(zqlite3_istream&& base) : _Base(std::move(base)) {}

    ~zqlite3_istream()
    {
        _Base::stmt_ = 0;
    }

    void operator >> (std::ostream& os)
    {
        for_each_of_tuple(_Base::fields_, [&os](auto& i) { os << i.name() << " bind err=" << i.err_ << std::endl; });
        os << "stmt step err=" << _Base::err_.err_step_ << std::endl;
        os << "stmt reset err=" << _Base::err_.err_reset_ << std::endl;
        os << "total changs=" << _Base::total_changes() << std::endl;
    }

    zqlite3_istream<0, _Fields...>&
    operator >> (decltype(std::ios::end) const& opcode)
    {
        return *this;
    }
};

/////////////////////////////
//// zqlite3_ostream
/////////////////////////////

template<size_t _Op, typename... _Fields>
struct zqlite3_ostream;

template<typename... _Fields>
struct zqlite3_ostream_base
{
    typedef std::tuple<_Fields...> _Tuple;
    struct _Err;
    zqlite3_ostream_base(_Tuple& fields, _Err& err) : fields_(fields), err_(err) {}
    zqlite3_ostream_base(const zqlite3_ostream_base& other) = default;
    zqlite3_ostream_base(zqlite3_ostream_base&& mv) : fields_(mv.fields_), err_(mv.err_)
    {
        std::swap(stmt_, mv.stmt_);
        std::swap(db_, mv.db_);
    }
    zqlite3_ostream_base(zqlite3_ostream_base&& mv, _Err& err) : fields_(mv.fields_), err_(err)
    {
        std::swap(stmt_, mv.stmt_);
        std::swap(db_, mv.db_);
    }
    _Tuple& fields_;
    struct _Err {
        int err_step_ = 0, err_reset_ = 0, err_ = 0;
    } & err_;
    DbStmt* stmt_ = 0;  // only 1 strong, other weak;
    Db* db_ = 0; // weak

    void reset()
    {

    }

    int total_changes()
    {
        return (db_) ? sqlite3_total_changes(db_) : 0;
    }
};

template<typename... _Fields>
struct zqlite3_ostream<(size_t)-1, _Fields...> : public zqlite3_ostream_base<_Fields...>
{
    //// root of ostream toshite
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_ostream_base<_Fields...> _Base;
    zqlite3_ostream(_Tuple& base) : _Base(base, err__) {}
    zqlite3_ostream(zqlite3_ostream&& base) : _Base(std::move(base), err__) {}
    zqlite3_ostream(const zqlite3_ostream&) = delete;
    
    typename _Base::_Err err__;

    ~zqlite3_ostream()
    {
        _Db_attach(0);
    }
    void _Db_attach(DbStmt* stmt)
    {
        if (stmt)
        {
            tracout << "call reset stmt" << std::endl;
            _Base::err_.err_reset_ = sqlite3_reset(stmt);
        }
        if (_Base::stmt_)
        {
            tracout << "call finalize stmt" << std::endl;
            sqlite3_finalize(_Base::stmt_);
        }
        _Base::stmt_ = stmt;
        _Base::err_.err_step_ = _Base::err_.err_reset_ = 0;
    }

    int begin_trans()
    {
        char* errmsg = 0;
        int err = sqlite3_exec(_Base::db_, "BEGIN TRANSACTION", 0,  0, &errmsg);
        if (err)
            tracout << "begin trans err: " << errmsg << std::endl;
        return err;
    }

    int commit_trans()
    {
        char* errmsg = 0;
        int err = sqlite3_exec(_Base::db_, "END TRANSACTION", 0,  0, &errmsg);
        if (err)
            tracout << "ends trans err: " << errmsg << std::endl;
        return err;
    }

    //// syntax <<
    zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...>
    operator << (typename std::tuple_element<0, _Tuple>::type::cdatatype const& el)
    {
        zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).assign(_Base::stmt_, 1, el);
        return std::move(next);
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<0, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...>
    operator << (typename std::enable_if<not _Enable, typename std::tuple_element<0, _Tuple>::type::datatype2>::type const& el)
    {
        zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).assign(_Base::stmt_, 1, el);
        return std::move(next);
    }

    //// syntax ( )
    zqlite3_ostream&
    operator () (typename _Fields::cdatatype const& ... vals)
    {
        iterate (*this, vals...);
        return *this;
    }

    zqlite3_ostream&
    operator () (typename _Fields::datatype2 const& ... vals, void* __unsed = 0)
    {
        iterate (*this, vals...);
        return *this;
    }
    
    //// syntax |
    zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...>
    operator | (typename std::tuple_element<0, _Tuple>::type::cdatatype const& el)
    {
        zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).assign(_Base::stmt_, 1, el);
        return std::move(next);
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<0, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...>
    operator | (typename std::enable_if<not _Enable, typename std::tuple_element<0, _Tuple>::type::datatype2>::type const& el)
    {
        zqlite3_ostream<sizeof...(_Fields) - 1, _Fields...> next(*this);
        std::get<0>(_Base::fields_).assign(_Base::stmt_, 1, el);
        return std::move(next);
    }
    
    bool ignored()
    {
        //// ignored by insertion of a duplicate key
        return err__.err_step_ == SQLITE_DENY;
    }
    
    int err() { return err__.err_; }

protected:
    template<typename _Os, typename _Head, typename... _Tails>
    void iterate (_Os& oz, _Head const& head, _Tails const&... tails)
    {
        auto next = oz << (head);
        return iterate (next, tails...);
    }

    void iterate (zqlite3_ostream<0, _Fields...>& exec)
    {
        exec << std::ios_base::end;
    }
};

template<size_t _Op, typename... _Fields>
struct zqlite3_ostream : public zqlite3_ostream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_ostream_base<_Fields...> _Base;
    zqlite3_ostream(const _Base& base) : _Base(base) {}
    zqlite3_ostream(zqlite3_ostream&& base) : _Base(std::move(base)) {}

    ~zqlite3_ostream()
    {
        _Base::stmt_ = 0;
    }

	zqlite3_ostream<_Op - 1, _Fields...>
	operator << (typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::cdatatype const& el)
    {
        zqlite3_ostream<_Op - 1, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - _Op>(_Base::fields_).assign(_Base::stmt_, sizeof...(_Fields) - _Op + 1, el);
        return std::move(next);
    }
    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
	zqlite3_ostream<_Op - 1, _Fields...>
	operator << (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::datatype2>::type const& el)
    {
        zqlite3_ostream<_Op - 1, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - _Op>(_Base::fields_).assign(_Base::stmt_, sizeof...(_Fields) - _Op + 1, el);
        return std::move(next);
    }
    
    zqlite3_ostream<_Op - 1, _Fields...>
	operator | (typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::cdatatype const& el)
    {
        return std::move(operator << (el));
    }
    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
	zqlite3_ostream<_Op - 1, _Fields...>
	operator | (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields)-_Op, _Tuple>::type::datatype2>::type const& el)
    {
        return std::move(operator << (el));
    }
};

template<typename... _Fields>
struct zqlite3_ostream<1, _Fields...> : public zqlite3_ostream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_ostream_base<_Fields...> _Base;
    zqlite3_ostream(const _Base& base) : _Base(base) {}
    zqlite3_ostream(zqlite3_ostream&& base) : _Base(std::move(base)) {}

    ~zqlite3_ostream()
    {
        _Base::stmt_ = 0;
    }

    zqlite3_ostream<0, _Fields...>
    operator << (typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::cdatatype const& el)
    {
        zqlite3_ostream<0, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - 1>(_Base::fields_).assign(_Base::stmt_, sizeof...(_Fields) - 1 + 1, el);
        return std::move(next);
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_ostream<0, _Fields...>
    operator << (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::datatype2>::type const& el)
    {
        zqlite3_ostream<0, _Fields...> next(*(_Base*)this);
        std::get<sizeof...(_Fields) - 1>(_Base::fields_).assign(_Base::stmt_, sizeof...(_Fields) - 1 + 1, el);
        return std::move(next);
    }
    
    zqlite3_ostream<0, _Fields...>
    operator | (typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::cdatatype const& el)
    {
        return std::move(operator << (el));
    }

    template<bool _Enable = std::is_same<typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::cdatatype, const typename std::tuple_element<0, _Tuple>::type::datatype2>::value>
    zqlite3_ostream<0, _Fields...>
    operator | (typename std::enable_if<not _Enable, typename std::tuple_element<sizeof...(_Fields) - 1, _Tuple>::type::datatype2>::type const& el)
    {
        return std::move(operator << (el));
    }
};

template<typename _T, typename _DT = typename select_para<_T>::datatype>
struct where_para;

template<typename _T>
struct where_para<_T, typename select_para<_T>::datatype>
{
    typedef typename select_para<_T>::cdatatype _DT;
    where_para(const _DT& t) : t_(t) {}
    const _DT t_;
    const _DT& operator * () const
    {
        return t_;
    }
};

template<typename... _Fields>
struct zqlite3_ostream<0, _Fields...> : public zqlite3_ostream_base<_Fields...>
{
    typedef std::tuple<_Fields...> _Tuple;
    typedef zqlite3_ostream_base<_Fields...> _Base;
    zqlite3_ostream(const _Base& base) : _Base(base) {}
    zqlite3_ostream(zqlite3_ostream&& base) : _Base(std::move(base)) {}

    size_t nwhere_para_ = 0;

    ~zqlite3_ostream()
    {
        _Base::stmt_ = 0;
    }

    void operator >> (std::ostream& os)
    {
        for_each_of_tuple(_Base::fields_, [&os](auto& i) { os << i.name() << " bind err=" << i.err_ << std::endl; });
        os << "stmt step err=" << _Base::err_.err_step_ << std::endl;
        os << "stmt reset err=" << _Base::err_.err_reset_ << std::endl;
        os << "total changs=" << _Base::total_changes() << std::endl;
    }
    
    void operator | (std::ostream& os)
    {
        operator >> (os);
    }

    zqlite3_ostream<0, _Fields...>&
    operator << (decltype(std::ios::end) const& opcode)
    {
        if (opcode == std::ios::end)
        {
            tracout << "call next step" << std::endl;
            _Base::err_.err_step_ = sqlite3_step(_Base::stmt_);
            tracout << "call reset statment" << std::endl;
            _Base::err_.err_reset_ = sqlite3_reset(_Base::stmt_);
        }
        return *this;
    }
    
    zqlite3_ostream<0, _Fields...>&
    operator | (decltype(std::ios::end) const& opcode)
    {
        return operator << (opcode);
    }

    template<typename _T, typename _DT>
    zqlite3_ostream<0, _Fields...>&
    operator << (where_para<_T, _DT>&& whpr)
    {
        tracout << "bind where_para" << "\n";
        int idx = ++nwhere_para_ + sizeof...(_Fields);
        static select_para<_T> op("");
        op.assign(_Base::stmt_, idx, *whpr);
        return *this;
    }
    
    template<typename _T, typename _DT>
    zqlite3_ostream<0, _Fields...>&
    operator | (where_para<_T, _DT>&& whpr)
    {
        return operator << (std::move(whpr));
    }
};

/////////////////////////////
//// zqlite3_table
/////////////////////////////

struct index_para
{
    typedef index_para itype;

    template<typename... _Str>
    index_para(_Str...  str)
    {
        _index_para<typename std::enable_if<std::is_same<_Str, _Str>::value, const std::string&>::type...>(str...);
    }

    template<typename... _Str>
    void _index_para(_Str...  str)
    {
        size_t predsize = 0;
        using expander = int[];
        (void)expander{0,
            (void(predsize += str.size() + 1), 0)...
        };
        buffer_.reserve(predsize);
        (void)expander{0,
            (void(buffer_ += str), void(buffer_ += '\0'), ++nstr_, 0)...
        };
    }

    index_para(index_para&& mv)
        : buffer_(std::move(mv.buffer_)), nstr_(mv.nstr_), flag_(mv.flag_) {}

    index_para pkey()
    {
        flag_.flag_pkey = 1;
        return std::move(*this);
    }

    index_para asc()
    {
        flag_.flag_asc = 1;
        return std::move(*this);
    }

    index_para desc()
    {
        flag_.flag_asc = 0;
        return std::move(*this);
    }

    index_para expr()
    {
        flag_.flag_expr = 1;
        return std::move(*this);
    }
private:
    std::string buffer_;
    size_t nstr_ = 0;
    struct {
        int flag_pkey : 1;
        int flag_asc : 1;
        int flag_expr : 1;
    } flag_ = {0};
    template<typename... Ts>
    friend struct zqlite3_table;
};

template<typename... Ts>
struct zqlite3_table
{
    std::shared_ptr<Db> db_;
    int err_;
    struct {
        int ctl_detach :1;
    } ctl_;

    template<typename... _Ts>
    zqlite3_table(_Ts... ts)
        : fields_(std::forward<_Ts>(ts)...) {}
    std::tuple<Ts...> fields_;
    std::vector<index_para> indices_;

    ~zqlite3_table()
    {
        ctl_.ctl_detach = 0;
        db_.reset();
    }

    zqlite3_table(zqlite3_table&& mv)
        : db_(std::move(mv.db_)), fields_(std::move(mv.fields_)), indices_(std::move(mv.indices_))
    {
        err_ = mv.err_;
        ctl_ = mv.ctl_;
    }

    auto create_row()
    {
        return std::tuple<typename Ts::datatype2...>();
    }
    
    int test_statment(const std::string& statment, std::string& errmsg)
    {
        const std::string& cmd = statment;
        DbStmt* stmt = 0;
        const char* problem = 0;
        int err = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (err == SQLITE_OK)
            sqlite3_finalize(stmt);
        else
        {
            const char* errstr = sqlite3_errstr(err);
            size_t predsize = cmd.size() + ::strlen(errstr);
            predsize += (problem) ? ::strlen(problem) : 0;
            errmsg.reserve(predsize + 16);
            errmsg = cmd;
            errmsg += "\n error: ";
            errmsg += errstr;
            errmsg += "\n at ";
            errmsg += problem;
        }
        return err;
    }

    std::string create_table_statment(const std::string& tblname)
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = tblname.size();
        for_each_fields([&predsize](auto& i){
                        predsize += i.name().size();
                        });
        for_each(indices_.begin(), indices_.end(),
            [&predsize](auto& i) {
                predsize += i.buffer_.size();
            });
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "CREATE TABLE IF NOT EXISTS `";
        statment += tblname;
        statment += "` (";

        for_each_fields([&statment](auto& i){
            statment += "`";
            statment += i.name();
            statment += "` ";
            statment += i.type();
            statment += ", ";
        });
        statment.resize(statment.size()-2);
        for_each(indices_.begin(), indices_.end(),
            [&statment](auto& i) {
                auto* p = i.buffer_.data();
                auto j = 0;
                statment += (i.flag_.flag_pkey) ? ", PRIMARY KEY (" : ", UNIQUE (";
                for (; j < i.nstr_ && p < &i.buffer_.back(); ++j)
                {
                    if (i.flag_.flag_expr)
                    {
                        statment += p;
                    }
                    else
                    {
                        statment += "`";
                        statment += p;
                        statment += "` ";
                    }
                    while(*++p != '\0');
                    ++p;
                    statment += (i.flag_.flag_asc) ? " ASC, " : " DESC, ";
                }
                statment.resize(statment.size() - 2);
                statment += ")";
            });
        statment += ");";
        return statment;
    }

    std::string insert_statment(const std::string& tblname)
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = tblname.size();
        for_each_fields([&predsize](auto& i){
            predsize += i.name().size();        // `%s`,
        });
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "INSERT INTO `";
        statment += tblname;
        statment += "` (";
        for_each_fields([&statment](auto& i){
            statment += "`";
            statment += i.name();
            statment += "`, ";
        });
        statment.resize(statment.size()-2);
        statment += ") VALUES (";
        for_each_fields([&statment](auto& i){
            statment += "?,";
        });
        statment.resize(statment.size()-1);
        statment += ");";
        return statment;
    }

    std::string select_statment(const std::string& tblname, const std::string& where = "")
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = 0;
        for_each_fields([&predsize](auto& i){
            predsize += i.name().size();        // `%s`,
        });
        predsize += tblname.size();
        predsize += where.size();
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "SELECT ";
        for_each_fields([&statment](auto& i){
            if (!i.expr_)
            {
                statment += "`";
                statment += i.name();
                statment += "`, ";
            }
            else
            {
                statment += i.name();
                statment += ", ";
            }
        });
        statment.resize(statment.size()-2);
        statment += " FROM `";
        statment += tblname;
        statment += "` ";
        statment += where;
        statment += ";";
        return statment;
    }
    
    std::string select_statment2(const std::string& tblnames_and_where)
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = 0;
        for_each_fields([&predsize](auto& i){
            predsize += i.name().size();        // `%s`,
        });
        predsize += tblnames_and_where.size();
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "SELECT ";
        for_each_fields([&statment](auto& i){
            if (!i.expr_)
            {
                statment += "`";
                statment += i.name();
                statment += "`, ";
            }
            else
            {
                statment += i.name();
                statment += ", ";
            }
        });
        statment.resize(statment.size()-2);
        statment += " FROM ";
        statment += tblnames_and_where;
        statment += ";";
        return statment;
    }

    std::string update_statment(const std::string& tblname)
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = tblname.size();
        for_each_fields([&predsize](auto& i){
            predsize += i.name().size();        // `%s`,
        });
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "UPDATE `";
        statment += tblname;
        statment += "` SET ";
        for_each_fields([&statment](auto& i){
            statment += "`";
            statment += i.name();
            statment += "` = ?, ";
        });
        statment.resize(statment.size()-2);
        statment += ";";
        return statment;
    }

    std::string update_where_statment(const std::string& tblname, const std::string& where)
    {
        auto NF = std::tuple_size<decltype(fields_)>::value;
        size_t predsize = tblname.size();
        for_each_fields([&predsize](auto& i){
            predsize += i.name().size();        // `%s`,
        });
        predsize += where.size();
        std::string statment;
        statment.reserve(predsize + NF * 12 + 128);
        statment += "UPDATE `";
        statment += tblname;
        statment += "` SET ";
        for_each_fields([&statment](auto& i){
            statment += "`";
            statment += i.name();
            statment += "` = ?, ";
        });
        statment.resize(statment.size()-2);
        statment += " ";
        statment += where;
        statment += ";";
        return statment;
    }

    int open_db(const std::string& dbname)
    {
        tracout << "call sqlite3_open " << dbname << std::endl;
        Db* db = 0; // TODO:
        err_ = sqlite3_open(dbname.c_str(), &db);
        db_.reset(db, [this](Db* db) {
                    if (!ctl_.ctl_detach)
                    {
                        tracout << "call sqlite3_close " << std::endl;
                        err_ = sqlite3_close(db);
                    }
                  });
        return err_;
    }

    bool attach_db(Db* db)
    {
        ctl_.ctl_detach = 0;
        db_.reset(db, [](Db*){});
    }

    Db* detach_db()
    {
        ctl_.ctl_detach = 1;
        Db* db = db_.get();
        db_.reset();
        return db;
    }

    int close_db()
    {
        ctl_.ctl_detach = 0;
        err_ = 0;
        db_.reset();
        return err_;
    }

    int create_table(const std::string& tblname)
    {
        auto cmd = create_table_statment(tblname);
        tracout << "call exec statment, " << cmd << std::endl;
        char* errmsg = 0;
        auto err = sqlite3_exec(db_.get(), cmd.c_str(), 0, 0, &errmsg);
        if (err)
            tracout << errmsg << std::endl;
        return err;
    }

    /*** https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple */
    template<typename F>
    void for_each_fields(F&& f)
    {
        f(std::get<0>(fields_));
        for_each_fields<1>(f);
    }

    template<size_t N, typename F>
    typename std::enable_if<N < std::tuple_size<decltype(fields_)>::value, void>::type for_each_fields(F&& f)
    {
        f(std::get<N>(fields_));
        for_each_fields<N+1>(f);
    }

    template<size_t N, typename F>
    typename std::enable_if<N == std::tuple_size<decltype(fields_)>::value, void>::type for_each_fields(F&& f)
    {

    }

    //// insert into
    zqlite3_ostream<(size_t)-1, Ts...> insert_into(const std::string& tblname)
    {
        zqlite3_ostream<(size_t)-1, Ts...> base(fields_);
        auto cmd = insert_statment(tblname);
        tracout << "call prepare insert statment, " << cmd << std::endl;
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_.err_)
            tracout << "error (" << sqlite3_errstr(base.err_.err_) << ")at " << problem << std::endl;
        return std::move ( base );
    }

    //// update
    zqlite3_ostream<(size_t)-1, Ts...> update(const std::string& tblname)
    {
        zqlite3_ostream<(size_t)-1, Ts...> base(fields_);
        std::string cmd = update_statment(tblname);
        tracout << "call prepare update statment, " << cmd << "\n";
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_)
            tracout << "error (" << sqlite3_errstr(base.err_) << ")at " << problem << std::endl;
        return std::move ( base );
    }

    zqlite3_ostream<(size_t)-1, Ts...> update_where(const std::string& tblname, const std::string& where)
    {
        zqlite3_ostream<(size_t)-1, Ts...> base(fields_);
        auto cmd = update_where_statment(tblname, where);
        tracout << "call prepare update statment, " << cmd << "\n";
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_.err_)
        {
            tracout << "error (" << sqlite3_errstr(base.err_.err_) << ")at " << problem << std::endl;
        }
        return std::move ( base );
    }

    //// select
    zqlite3_istream<(size_t)-1, Ts...> select_from(const std::string& tblname, const std::string& where = "")
    {
        zqlite3_istream<(size_t)-1, Ts...> base(fields_);
        auto cmd = select_statment(tblname, where);
        tracout << "call prepare select statment, " << cmd << "\n";
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_.err_)
        {
            tracout << "error (" << sqlite3_errstr(base.err_.err_) << ")at " << problem << std::endl;
        }
        return std::move ( base );
    }
    
    zqlite3_istream<(size_t)-1, Ts...> select_from2(const std::string& tblnames_and_where)
    {
        zqlite3_istream<(size_t)-1, Ts...> base(fields_);
        auto cmd = select_statment2(tblnames_and_where);
        tracout << "call prepare select statment, " << cmd << "\n";
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_.err_)
        {
            tracout << "error (" << sqlite3_errstr(base.err_.err_) << ")at " << problem << std::endl;
        }
        return std::move ( base );
    }
    
    //// pragma
    zqlite3_istream<(size_t)-1, Ts...> pragma()
    {
        zqlite3_istream<(size_t)-1, Ts...> base(fields_);
        std::string cmd;
        cmd.reserve(std::get<0>(fields_).name().size());
        cmd = "PRAGMA ";
        cmd += std::get<0>(fields_).name();
        cmd += ";";
        DbStmt*& stmt = base.stmt_;
        Db* db = db_.get();
        const char* problem = 0;
        base.db_ = db;
        base.err_.err_ = sqlite3_prepare(db_.get(), cmd.c_str(), cmd.size(), &stmt, &problem);
        if (base.err_.err_)
        {
            tracout << "error (" << sqlite3_errstr(base.err_.err_) << ")at " << problem << std::endl;
        }
        return std::move ( base );
    }
};



/////////////////////////////
//// helper make zqlite3_table
/////////////////////////////

template<typename... Ts>
zqlite3_table<typename Ts::stype...> make_zqlite3_table_0(Ts&&... ts)
{
    return zqlite3_table<typename Ts::stype...>(std::forward<Ts>(ts)...);
};


template<size_t N, size_t M, typename Head, typename... Ts>
auto make_zqlite3_table_2(std::vector<index_para>&& coll, Head&& head, Ts&&... ts);

template<size_t N, size_t M, typename Head, typename... Ts>
auto _make_zqlite3_table_2(std::vector<index_para>&& coll, std::enable_if_t<N != 0 && M != 0 && not std::is_same<Head, index_para>::value, Head>*, Head&& head, Ts&&... ts)
{
    //// shift the ts until first index_para
    return make_zqlite3_table_2<N, M>(std::move(coll), std::forward<Ts>(ts)..., std::forward<Head>(head));
}
template<size_t N, size_t M, typename Head, typename... Ts>
auto _make_zqlite3_table_2(std::vector<index_para>&& coll, std::enable_if_t<N==0 && M != 0, Head>*, Head&& head, Ts&&... ts)
{
    //// has no index_para
    return make_zqlite3_table_0(std::forward<Head>(head), std::forward<Ts>(ts)...);
}
template<size_t N, size_t M, typename Head, typename... Ts>
auto _make_zqlite3_table_2(std::vector<index_para>&& coll, std::enable_if_t<N==0 && M == 0, Head>*, Head&& head, Ts&&... ts)
{
    //// all the index_para have been collected.
    auto tbl = make_zqlite3_table_0(std::forward<Head>(head), std::forward<Ts>(ts)...);
    tbl.indices_ = std::move(coll);
    return std::move(tbl);
}
template<size_t N, size_t M, typename... Ts>
auto _make_zqlite3_table_2(std::vector<index_para>&& coll, index_para*, index_para&& head, Ts&&... ts)
{
    //// collect index_para
    coll.emplace_back(std::move(head));
    return make_zqlite3_table_2<N-1, 0>(std::move(coll), std::forward<Ts>(ts)...);
}

template<size_t N, size_t M, typename Head, typename... Ts>
auto make_zqlite3_table_2(std::vector<index_para>&& coll, Head&& head, Ts&&... ts)
{
    //// main entry and route to _make_zqlite3_table_2
    return _make_zqlite3_table_2<N, M>(std::move(coll), &head, std::forward<Head>(head), std::forward<Ts>(ts)...);
}

template<typename... Ts>
auto make_zqlite3_table(Ts&&... ts)
{
    auto* tp = split_types(ts...);
    typedef std::decay_t<typename std::tuple_element<0, std::decay_t<decltype(*tp)> >::type> tp1;
    typedef std::decay_t<typename std::tuple_element<1, std::decay_t<decltype(*tp)> >::type> tp2;
    
    std::vector<index_para> coll;
    coll.reserve(std::tuple_size<tp2>::value);
    return make_zqlite3_table_2<std::tuple_size<tp2>::value, std::tuple_size<tp1>::value>(std::move(coll), std::forward<Ts>(ts)...);
};

/////////////////////////////
//// meta programming split_types to two types container
/////////////////////////////

template<typename... Ts>
auto split_types(Ts&... ts)
{
    return _split_types(ts...);
}

template<typename Head, typename... Ts>
auto _split_types(Head& head, Ts&... ts)
{
    decltype(std::tie(head))* p = 0;
    return _split_types(p, ts...);
}

template<typename... Res, typename Head, typename... Ts>
auto _split_types(std::tuple<Res...>* res, Head& head, Ts&... ts)
{
    decltype(std::tuple_cat(*res, std::tie(head)))* p = 0;
    return _split_types(p, ts...);
}

template<typename... Res, typename Head>
auto _split_types(std::tuple<Res...>* res, Head& head)
{
    decltype(std::tuple_cat(*res, std::tie(head)))* p = 0;
    return _split_types(p);
}

template<typename... Res>
auto _split_types(std::tuple<Res...>* res)
{
    return (std::tuple<std::tuple<Res...>, std::tuple<> >*) 0;
}

template<typename... Res, typename... Ts>
auto _split_types(std::tuple<Res...>* res, index_para& head, Ts&... ts)
{
    decltype(std::tie(head))* p = 0;
    return _split_types2(res, p, ts...);
}

template<typename... Res, typename... Res2, typename... Ts>
auto _split_types2(std::tuple<Res...>* res, std::tuple<Res2...>* res2, index_para& head, Ts&... ts)
{
    decltype(std::tuple_cat(*res2, std::tie(head)))* p = 0;
    return _split_type2(res, p, ts...);
}

template<typename... Res, typename... Res2>
auto _split_types2(std::tuple<Res...>* res, std::tuple<Res2...>* res2, index_para& head)
{
    decltype(std::tuple_cat(*res2, std::tie(head)))* p = 0;
    return _split_types2(res, p);
}

template<typename... Res, typename... Res2>
auto _split_types2(std::tuple<Res...>* res, std::tuple<Res2...>* res2)
{
    return (std::tuple<std::tuple<Res...>, std::tuple<Res2...> >*)0;
}

}; // end NS zqlite3
}; // end NS zhelper

#endif // __ZhelpEr_ZQLite3_H_

