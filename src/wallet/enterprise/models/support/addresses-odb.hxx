// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef ADDRESSES_ODB_HXX
#define ADDRESSES_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "wallet/enterprise/models/addresses.h"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>
#include <odb/view-image.hxx>
#include <odb/view-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // eAddresses
  //
  template <>
  struct class_traits< ::eAddresses >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::eAddresses >
  {
    public:
    typedef ::eAddresses object_type;
    typedef ::eAddresses* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef unsigned int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };

  // address_stats
  //
  template <>
  struct class_traits< ::address_stats >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::address_stats >
  {
    public:
    typedef ::address_stats view_type;
    typedef ::address_stats* pointer_type;

    static void
    callback (database&, view_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/pgsql/version.hxx>
#include <odb/pgsql/forward.hxx>
#include <odb/pgsql/binding.hxx>
#include <odb/pgsql/pgsql-types.hxx>
#include <odb/pgsql/query.hxx>

namespace odb
{
  // eAddresses
  //
  template <typename A>
  struct query_columns< ::eAddresses, id_pgsql, A >
  {
    // name
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    name_type_;

    static const name_type_ name;

    // purpose
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    purpose_type_;

    static const purpose_type_ purpose;

    // time
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::int64_t,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    time_type_;

    static const time_type_ time;

    // is_used
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        bool,
        pgsql::id_boolean >::query_type,
      pgsql::id_boolean >
    is_used_type_;

    static const is_used_type_ is_used;

    // id
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    id_type_;

    static const id_type_ id;

    // address
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    address_type_;

    static const address_type_ address;
  };

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::name_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  name (A::table_name, "\"name\"", 0);

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::purpose_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  purpose (A::table_name, "\"purpose\"", 0);

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::time_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  time (A::table_name, "\"time\"", 0);

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::is_used_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  is_used (A::table_name, "\"is_used\"", 0);

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::id_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::eAddresses, id_pgsql, A >::address_type_
  query_columns< ::eAddresses, id_pgsql, A >::
  address (A::table_name, "\"address\"", 0);

  template <typename A>
  struct pointer_query_columns< ::eAddresses, id_pgsql, A >:
    query_columns< ::eAddresses, id_pgsql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::eAddresses, id_pgsql >:
    public access::object_traits< ::eAddresses >
  {
    public:
    struct id_image_type
    {
      int id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // name
      //
      details::buffer name_value;
      std::size_t name_size;
      bool name_null;

      // purpose
      //
      details::buffer purpose_value;
      std::size_t purpose_size;
      bool purpose_null;

      // time
      //
      long long time_value;
      bool time_null;

      // is_used
      //
      bool is_used_value;
      bool is_used_null;

      // id
      //
      int id_value;
      bool id_null;

      // address
      //
      details::buffer address_value;
      std::size_t address_size;
      bool address_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&,
          pgsql::statement_kind);

    static void
    bind (pgsql::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          pgsql::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef pgsql::object_statements<object_type> statements_type;

    typedef pgsql::query_base query_base_type;

    static const std::size_t column_count = 6UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    static const char persist_statement_name[];
    static const char find_statement_name[];
    static const char update_statement_name[];
    static const char erase_statement_name[];
    static const char query_statement_name[];
    static const char erase_query_statement_name[];

    static const unsigned int persist_statement_types[];
    static const unsigned int find_statement_types[];
    static const unsigned int update_statement_types[];

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::eAddresses, id_common >:
    public access::object_traits_impl< ::eAddresses, id_pgsql >
  {
  };

  // address_stats
  //
  template <>
  class access::view_traits_impl< ::address_stats, id_pgsql >:
    public access::view_traits< ::address_stats >
  {
    public:
    struct image_type
    {
      // count
      //
      long long count_value;
      bool count_null;

      std::size_t version;
    };

    typedef pgsql::view_statements<view_type> statements_type;

    typedef pgsql::query_base query_base_type;
    struct query_columns;

    static const bool versioned = false;

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&);

    static void
    init (view_type&,
          const image_type&,
          database*);

    static const std::size_t column_count = 1UL;

    static query_base_type
    query_statement (const query_base_type&);

    static result<view_type>
    query (database&, const query_base_type&);

    static const char query_statement_name[];
  };

  template <>
  class access::view_traits_impl< ::address_stats, id_common >:
    public access::view_traits_impl< ::address_stats, id_pgsql >
  {
  };

  // eAddresses
  //
  // address_stats
  //
  struct access::view_traits_impl< ::address_stats, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::eAddresses,
      id_pgsql,
      odb::access::object_traits_impl< ::eAddresses, id_pgsql > >
  {
  };
}

#include "wallet/enterprise/models/support/addresses-odb.ixx"

#include <odb/post.hxx>

#endif // ADDRESSES_ODB_HXX
