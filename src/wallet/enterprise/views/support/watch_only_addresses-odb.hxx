// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef WATCH_ONLY_ADDRESSES_ODB_HXX
#define WATCH_ONLY_ADDRESSES_ODB_HXX

// Begin prologue.
//
#include <odb/boost/version.hxx>
#if ODB_BOOST_VERSION != 2040000 // 2.4.0
#  error ODB and C++ compilers see different libodb-boost interface versions
#endif
#include <odb/boost/uuid/pgsql/uuid-traits.hxx>
//
// End prologue.

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "wallet/enterprise/views/watch_only_addresses.h"

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
#include <odb/view-image.hxx>
#include <odb/view-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // watch_only_addresses
  //
  template <>
  struct class_traits< ::watch_only_addresses >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::watch_only_addresses >
  {
    public:
    typedef ::watch_only_addresses view_type;
    typedef ::watch_only_addresses* pointer_type;

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
  // watch_only_addresses
  //
  template <>
  class access::view_traits_impl< ::watch_only_addresses, id_pgsql >:
    public access::view_traits< ::watch_only_addresses >
  {
    public:
    struct image_type
    {
      // wallet_id
      //
      unsigned char wallet_id_value[16];
      bool wallet_id_null;

      // address
      //
      details::buffer address_value;
      std::size_t address_size;
      bool address_null;

      // source
      //
      details::buffer source_value;
      std::size_t source_size;
      bool source_null;

      // watch_only_address_id
      //
      int watch_only_address_id_value;
      bool watch_only_address_id_null;

      std::size_t version;
    };

    typedef pgsql::view_statements<view_type> statements_type;

    typedef pgsql::query_base query_base_type;
    struct query_columns
    {
    };

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

    static const std::size_t column_count = 4UL;

    static query_base_type
    query_statement (const query_base_type&);

    static result<view_type>
    query (database&, const query_base_type&);

    static const char query_statement_name[];
  };

  template <>
  class access::view_traits_impl< ::watch_only_addresses, id_common >:
    public access::view_traits_impl< ::watch_only_addresses, id_pgsql >
  {
  };
}

#include "wallet/enterprise/views/watch_only_addresses-odb.ixx"

#include <odb/post.hxx>

#endif // WATCH_ONLY_ADDRESSES_ODB_HXX
