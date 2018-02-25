// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#include <odb/pre.hxx>

#include "wallet/enterprise/models/support/transactions-odb.hxx"

#include <cassert>
#include <cstring>  // std::memcpy


#include <odb/pgsql/traits.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/pgsql/transaction.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/statement.hxx>
#include <odb/pgsql/statement-cache.hxx>
#include <odb/pgsql/simple-object-statements.hxx>
#include <odb/pgsql/container-statements.hxx>
#include <odb/pgsql/exceptions.hxx>
#include <odb/pgsql/simple-object-result.hxx>

namespace odb
{
  // eTransactions
  //

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  persist_statement_name[] = "persist_eTransactions";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  find_statement_name[] = "find_eTransactions";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  update_statement_name[] = "update_eTransactions";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  erase_statement_name[] = "erase_eTransactions";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  query_statement_name[] = "query_eTransactions";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::
  erase_query_statement_name[] = "erase_query_eTransactions";

  const unsigned int access::object_traits_impl< ::eTransactions, id_pgsql >::
  persist_statement_types[] =
  {
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::bool_oid,
    pgsql::int4_oid,
    pgsql::int8_oid,
    pgsql::int8_oid,
    pgsql::text_oid,
    pgsql::uuid_oid
  };

  const unsigned int access::object_traits_impl< ::eTransactions, id_pgsql >::
  find_statement_types[] =
  {
    pgsql::int4_oid
  };

  const unsigned int access::object_traits_impl< ::eTransactions, id_pgsql >::
  update_statement_types[] =
  {
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::bool_oid,
    pgsql::int4_oid,
    pgsql::int8_oid,
    pgsql::int8_oid,
    pgsql::text_oid,
    pgsql::uuid_oid,
    pgsql::int4_oid
  };

  struct access::object_traits_impl< ::eTransactions, id_pgsql >::extra_statement_cache_type
  {
    extra_statement_cache_type (
      pgsql::connection&,
      image_type&,
      id_image_type&,
      pgsql::binding&,
      pgsql::binding&,
      pgsql::native_binding&,
      const unsigned int*)
    {
    }
  };

  access::object_traits_impl< ::eTransactions, id_pgsql >::id_type
  access::object_traits_impl< ::eTransactions, id_pgsql >::
  id (const id_image_type& i)
  {
    pgsql::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        id,
        i.id_value,
        i.id_null);
    }

    return id;
  }

  access::object_traits_impl< ::eTransactions, id_pgsql >::id_type
  access::object_traits_impl< ::eTransactions, id_pgsql >::
  id (const image_type& i)
  {
    pgsql::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        id,
        i.id_value,
        i.id_null);
    }

    return id;
  }

  bool access::object_traits_impl< ::eTransactions, id_pgsql >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // id
    //
    t[0UL] = 0;

    // block_id
    //
    t[1UL] = 0;

    // block_index
    //
    t[2UL] = 0;

    // is_trusted
    //
    t[3UL] = 0;

    // size
    //
    t[4UL] = 0;

    // time
    //
    t[5UL] = 0;

    // time_received
    //
    t[6UL] = 0;

    // txid
    //
    if (t[7UL])
    {
      i.txid_value.capacity (i.txid_size);
      grew = true;
    }

    // wallet_id
    //
    t[8UL] = 0;

    return grew;
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  bind (pgsql::bind* b,
        image_type& i,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    std::size_t n (0);

    // id
    //
    if (sk != statement_insert && sk != statement_update)
    {
      b[n].type = pgsql::bind::integer;
      b[n].buffer = &i.id_value;
      b[n].is_null = &i.id_null;
      n++;
    }

    // block_id
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.block_id_value;
    b[n].is_null = &i.block_id_null;
    n++;

    // block_index
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.block_index_value;
    b[n].is_null = &i.block_index_null;
    n++;

    // is_trusted
    //
    b[n].type = pgsql::bind::boolean_;
    b[n].buffer = &i.is_trusted_value;
    b[n].is_null = &i.is_trusted_null;
    n++;

    // size
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.size_value;
    b[n].is_null = &i.size_null;
    n++;

    // time
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.time_value;
    b[n].is_null = &i.time_null;
    n++;

    // time_received
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.time_received_value;
    b[n].is_null = &i.time_received_null;
    n++;

    // txid
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.txid_value.data ();
    b[n].capacity = i.txid_value.capacity ();
    b[n].size = &i.txid_size;
    b[n].is_null = &i.txid_null;
    n++;

    // wallet_id
    //
    b[n].type = pgsql::bind::uuid;
    b[n].buffer = i.wallet_id_value;
    b[n].is_null = &i.wallet_id_null;
    n++;
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  bind (pgsql::bind* b, id_image_type& i)
  {
    std::size_t n (0);
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.id_value;
    b[n].is_null = &i.id_null;
  }

  bool access::object_traits_impl< ::eTransactions, id_pgsql >::
  init (image_type& i,
        const object_type& o,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    bool grew (false);

    // block_id
    //
    {
      unsigned int const& v =
        o.block_id;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.block_id_value, is_null, v);
      i.block_id_null = is_null;
    }

    // block_index
    //
    {
      int const& v =
        o.block_index;

      bool is_null (false);
      pgsql::value_traits<
          int,
          pgsql::id_integer >::set_image (
        i.block_index_value, is_null, v);
      i.block_index_null = is_null;
    }

    // is_trusted
    //
    {
      bool const& v =
        o.is_trusted;

      bool is_null (false);
      pgsql::value_traits<
          bool,
          pgsql::id_boolean >::set_image (
        i.is_trusted_value, is_null, v);
      i.is_trusted_null = is_null;
    }

    // size
    //
    {
      unsigned int const& v =
        o.size;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.size_value, is_null, v);
      i.size_null = is_null;
    }

    // time
    //
    {
      ::int64_t const& v =
        o.time;

      bool is_null (false);
      pgsql::value_traits<
          ::int64_t,
          pgsql::id_bigint >::set_image (
        i.time_value, is_null, v);
      i.time_null = is_null;
    }

    // time_received
    //
    {
      ::int64_t const& v =
        o.time_received;

      bool is_null (false);
      pgsql::value_traits<
          ::int64_t,
          pgsql::id_bigint >::set_image (
        i.time_received_value, is_null, v);
      i.time_received_null = is_null;
    }

    // txid
    //
    {
      ::std::string const& v =
        o.txid;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.txid_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.txid_value,
        size,
        is_null,
        v);
      i.txid_null = is_null;
      i.txid_size = size;
      grew = grew || (cap != i.txid_value.capacity ());
    }

    // wallet_id
    //
    {
      ::boost::uuids::uuid const& v =
        o.wallet_id;

      bool is_null (false);
      pgsql::value_traits<
          ::boost::uuids::uuid,
          pgsql::id_uuid >::set_image (
        i.wallet_id_value, is_null, v);
      i.wallet_id_null = is_null;
    }

    return grew;
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // id
    //
    {
      unsigned int& v =
        o.id;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.id_value,
        i.id_null);
    }

    // block_id
    //
    {
      unsigned int& v =
        o.block_id;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.block_id_value,
        i.block_id_null);
    }

    // block_index
    //
    {
      int& v =
        o.block_index;

      pgsql::value_traits<
          int,
          pgsql::id_integer >::set_value (
        v,
        i.block_index_value,
        i.block_index_null);
    }

    // is_trusted
    //
    {
      bool& v =
        o.is_trusted;

      pgsql::value_traits<
          bool,
          pgsql::id_boolean >::set_value (
        v,
        i.is_trusted_value,
        i.is_trusted_null);
    }

    // size
    //
    {
      unsigned int& v =
        o.size;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.size_value,
        i.size_null);
    }

    // time
    //
    {
      ::int64_t& v =
        o.time;

      pgsql::value_traits<
          ::int64_t,
          pgsql::id_bigint >::set_value (
        v,
        i.time_value,
        i.time_null);
    }

    // time_received
    //
    {
      ::int64_t& v =
        o.time_received;

      pgsql::value_traits<
          ::int64_t,
          pgsql::id_bigint >::set_value (
        v,
        i.time_received_value,
        i.time_received_null);
    }

    // txid
    //
    {
      ::std::string& v =
        o.txid;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.txid_value,
        i.txid_size,
        i.txid_null);
    }

    // wallet_id
    //
    {
      ::boost::uuids::uuid& v =
        o.wallet_id;

      pgsql::value_traits<
          ::boost::uuids::uuid,
          pgsql::id_uuid >::set_value (
        v,
        i.wallet_id_value,
        i.wallet_id_null);
    }
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  init (id_image_type& i, const id_type& id)
  {
    {
      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.id_value, is_null, id);
      i.id_null = is_null;
    }
  }

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::persist_statement[] =
  "INSERT INTO \"wallet\".\"eTransactions\" "
  "(\"id\", "
  "\"block_id\", "
  "\"block_index\", "
  "\"is_trusted\", "
  "\"size\", "
  "\"time\", "
  "\"time_received\", "
  "\"txid\", "
  "\"wallet_id\") "
  "VALUES "
  "(DEFAULT, $1, $2, $3, $4, $5, $6, $7, $8) "
  "RETURNING \"id\"";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::find_statement[] =
  "SELECT "
  "\"wallet\".\"eTransactions\".\"id\", "
  "\"wallet\".\"eTransactions\".\"block_id\", "
  "\"wallet\".\"eTransactions\".\"block_index\", "
  "\"wallet\".\"eTransactions\".\"is_trusted\", "
  "\"wallet\".\"eTransactions\".\"size\", "
  "\"wallet\".\"eTransactions\".\"time\", "
  "\"wallet\".\"eTransactions\".\"time_received\", "
  "\"wallet\".\"eTransactions\".\"txid\", "
  "\"wallet\".\"eTransactions\".\"wallet_id\" "
  "FROM \"wallet\".\"eTransactions\" "
  "WHERE \"wallet\".\"eTransactions\".\"id\"=$1";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::update_statement[] =
  "UPDATE \"wallet\".\"eTransactions\" "
  "SET "
  "\"block_id\"=$1, "
  "\"block_index\"=$2, "
  "\"is_trusted\"=$3, "
  "\"size\"=$4, "
  "\"time\"=$5, "
  "\"time_received\"=$6, "
  "\"txid\"=$7, "
  "\"wallet_id\"=$8 "
  "WHERE \"id\"=$9";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::erase_statement[] =
  "DELETE FROM \"wallet\".\"eTransactions\" "
  "WHERE \"id\"=$1";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::query_statement[] =
  "SELECT "
  "\"wallet\".\"eTransactions\".\"id\", "
  "\"wallet\".\"eTransactions\".\"block_id\", "
  "\"wallet\".\"eTransactions\".\"block_index\", "
  "\"wallet\".\"eTransactions\".\"is_trusted\", "
  "\"wallet\".\"eTransactions\".\"size\", "
  "\"wallet\".\"eTransactions\".\"time\", "
  "\"wallet\".\"eTransactions\".\"time_received\", "
  "\"wallet\".\"eTransactions\".\"txid\", "
  "\"wallet\".\"eTransactions\".\"wallet_id\" "
  "FROM \"wallet\".\"eTransactions\"";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::erase_query_statement[] =
  "DELETE FROM \"wallet\".\"eTransactions\"";

  const char access::object_traits_impl< ::eTransactions, id_pgsql >::table_name[] =
  "\"wallet\".\"eTransactions\"";

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  persist (database& db, object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::pre_persist);

    image_type& im (sts.image ());
    binding& imb (sts.insert_image_binding ());

    if (init (im, obj, statement_insert))
      im.version++;

    if (im.version != sts.insert_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_insert);
      sts.insert_image_version (im.version);
      imb.version++;
    }

    {
      id_image_type& i (sts.id_image ());
      binding& b (sts.id_image_binding ());
      if (i.version != sts.id_image_version () || b.version == 0)
      {
        bind (b.bind, i);
        sts.id_image_version (i.version);
        b.version++;
      }
    }

    insert_statement& st (sts.persist_statement ());
    if (!st.execute ())
      throw object_already_persistent ();

    obj.id = id (sts.id_image ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::post_persist);
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  update (database& db, const object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;
    using pgsql::update_statement;

    callback (db, obj, callback_event::pre_update);

    pgsql::transaction& tr (pgsql::transaction::current ());
    pgsql::connection& conn (tr.connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    const id_type& id (
      obj.id);
    id_image_type& idi (sts.id_image ());
    init (idi, id);

    image_type& im (sts.image ());
    if (init (im, obj, statement_update))
      im.version++;

    bool u (false);
    binding& imb (sts.update_image_binding ());
    if (im.version != sts.update_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_update);
      sts.update_image_version (im.version);
      imb.version++;
      u = true;
    }

    binding& idb (sts.id_image_binding ());
    if (idi.version != sts.update_id_image_version () ||
        idb.version == 0)
    {
      if (idi.version != sts.id_image_version () ||
          idb.version == 0)
      {
        bind (idb.bind, idi);
        sts.id_image_version (idi.version);
        idb.version++;
      }

      sts.update_id_image_version (idi.version);

      if (!u)
        imb.version++;
    }

    update_statement& st (sts.update_statement ());
    if (st.execute () == 0)
      throw object_not_persistent ();

    callback (db, obj, callback_event::post_update);
    pointer_cache_traits::update (db, obj);
  }

  void access::object_traits_impl< ::eTransactions, id_pgsql >::
  erase (database& db, const id_type& id)
  {
    using namespace pgsql;

    ODB_POTENTIALLY_UNUSED (db);

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    id_image_type& i (sts.id_image ());
    init (i, id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    if (sts.erase_statement ().execute () != 1)
      throw object_not_persistent ();

    pointer_cache_traits::erase (db, id);
  }

  access::object_traits_impl< ::eTransactions, id_pgsql >::pointer_type
  access::object_traits_impl< ::eTransactions, id_pgsql >::
  find (database& db, const id_type& id)
  {
    using namespace pgsql;

    {
      pointer_type p (pointer_cache_traits::find (db, id));

      if (!pointer_traits::null_ptr (p))
        return p;
    }

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (l.locked ())
    {
      if (!find_ (sts, &id))
        return pointer_type ();
    }

    pointer_type p (
      access::object_factory<object_type, pointer_type>::create ());
    pointer_traits::guard pg (p);

    pointer_cache_traits::insert_guard ig (
      pointer_cache_traits::insert (db, id, p));

    object_type& obj (pointer_traits::get_ref (p));

    if (l.locked ())
    {
      select_statement& st (sts.find_statement ());
      ODB_POTENTIALLY_UNUSED (st);

      callback (db, obj, callback_event::pre_load);
      init (obj, sts.image (), &db);
      load_ (sts, obj, false);
      sts.load_delayed (0);
      l.unlock ();
      callback (db, obj, callback_event::post_load);
      pointer_cache_traits::load (ig.position ());
    }
    else
      sts.delay_load (id, obj, ig.position ());

    ig.release ();
    pg.release ();
    return p;
  }

  bool access::object_traits_impl< ::eTransactions, id_pgsql >::
  find (database& db, const id_type& id, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    reference_cache_traits::position_type pos (
      reference_cache_traits::insert (db, id, obj));
    reference_cache_traits::insert_guard ig (pos);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, false);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    reference_cache_traits::load (pos);
    ig.release ();
    return true;
  }

  bool access::object_traits_impl< ::eTransactions, id_pgsql >::
  reload (database& db, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    const id_type& id  (
      obj.id);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, true);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    return true;
  }

  bool access::object_traits_impl< ::eTransactions, id_pgsql >::
  find_ (statements_type& sts,
         const id_type* id)
  {
    using namespace pgsql;

    id_image_type& i (sts.id_image ());
    init (i, *id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    select_statement& st (sts.find_statement ());

    st.execute ();
    auto_result ar (st);
    select_statement::result r (st.fetch ());

    if (r == select_statement::truncated)
    {
      if (grow (im, sts.select_image_truncated ()))
        im.version++;

      if (im.version != sts.select_image_version ())
      {
        bind (imb.bind, im, statement_select);
        sts.select_image_version (im.version);
        imb.version++;
        st.refetch ();
      }
    }

    return r != select_statement::no_data;
  }

  result< access::object_traits_impl< ::eTransactions, id_pgsql >::object_type >
  access::object_traits_impl< ::eTransactions, id_pgsql >::
  query (database&, const query_base_type& q)
  {
    using namespace pgsql;
    using odb::details::shared;
    using odb::details::shared_ptr;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());

    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    std::string text (query_statement);
    if (!q.empty ())
    {
      text += " ";
      text += q.clause ();
    }

    q.init_parameters ();
    shared_ptr<select_statement> st (
      new (shared) select_statement (
        sts.connection (),
        query_statement_name,
        text,
        false,
        true,
        q.parameter_types (),
        q.parameter_count (),
        q.parameters_binding (),
        imb));

    st->execute ();
    st->deallocate ();

    shared_ptr< odb::object_result_impl<object_type> > r (
      new (shared) pgsql::object_result_impl<object_type> (
        q, st, sts, 0));

    return result<object_type> (r);
  }

  unsigned long long access::object_traits_impl< ::eTransactions, id_pgsql >::
  erase_query (database&, const query_base_type& q)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());

    std::string text (erase_query_statement);
    if (!q.empty ())
    {
      text += ' ';
      text += q.clause ();
    }

    q.init_parameters ();
    delete_statement st (
      conn,
      erase_query_statement_name,
      text,
      q.parameter_types (),
      q.parameter_count (),
      q.parameters_binding ());

    return st.execute ();
  }
}

#include <odb/post.hxx>
