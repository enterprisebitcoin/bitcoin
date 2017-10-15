// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef OUTPUT_ENTRIES_ODB_HXX
#define OUTPUT_ENTRIES_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "output_entries.h"

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

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // eOutputEntries
  //
  template <>
  struct class_traits< ::eOutputEntries >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::eOutputEntries >
  {
    public:
    typedef ::eOutputEntries object_type;
    typedef ::eOutputEntries* pointer_type;
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
}

#include <odb/details/buffer.hxx>

#include <odb/pgsql/version.hxx>
#include <odb/pgsql/forward.hxx>
#include <odb/pgsql/binding.hxx>
#include <odb/pgsql/pgsql-types.hxx>
#include <odb/pgsql/query.hxx>

namespace odb
{
  // eOutputEntries
  //
  template <typename A>
  struct query_columns< ::eOutputEntries, id_pgsql, A >
  {
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

    // etransaction_id
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    etransaction_id_type_;

    static const etransaction_id_type_ etransaction_id;

    // vector
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    vector_type_;

    static const vector_type_ vector;

    // amount
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::int64_t,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    amount_type_;

    static const amount_type_ amount;

    // category
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    category_type_;

    static const category_type_ category;

    // destination
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    destination_type_;

    static const destination_type_ destination;
  };

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::id_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::etransaction_id_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  etransaction_id (A::table_name, "\"etransaction_id\"", 0);

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::vector_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  vector (A::table_name, "\"vector\"", 0);

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::amount_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  amount (A::table_name, "\"amount\"", 0);

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::category_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  category (A::table_name, "\"category\"", 0);

  template <typename A>
  const typename query_columns< ::eOutputEntries, id_pgsql, A >::destination_type_
  query_columns< ::eOutputEntries, id_pgsql, A >::
  destination (A::table_name, "\"destination\"", 0);

  template <typename A>
  struct pointer_query_columns< ::eOutputEntries, id_pgsql, A >:
    query_columns< ::eOutputEntries, id_pgsql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::eOutputEntries, id_pgsql >:
    public access::object_traits< ::eOutputEntries >
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
      // id_
      //
      int id_value;
      bool id_null;

      // etransaction_id_
      //
      int etransaction_id_value;
      bool etransaction_id_null;

      // vector_
      //
      int vector_value;
      bool vector_null;

      // amount_
      //
      long long amount_value;
      bool amount_null;

      // category_
      //
      details::buffer category_value;
      std::size_t category_size;
      bool category_null;

      // destination_
      //
      details::buffer destination_value;
      std::size_t destination_size;
      bool destination_null;

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
  class access::object_traits_impl< ::eOutputEntries, id_common >:
    public access::object_traits_impl< ::eOutputEntries, id_pgsql >
  {
  };

  // eOutputEntries
  //
}

#include "output_entries-odb.ixx"

#include <odb/post.hxx>

#endif // OUTPUT_ENTRIES_ODB_HXX
