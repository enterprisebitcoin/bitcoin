/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "wallet"."eTransactions" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "wallet"."eTransactions" (
  "id" SERIAL NOT NULL PRIMARY KEY,
  "block_index" INTEGER NOT NULL,
  "is_trusted" BOOLEAN NOT NULL,
  "size" INTEGER NOT NULL,
  "time" BIGINT NOT NULL,
  "time_received" BIGINT NOT NULL,
  "txid" TEXT NOT NULL,
  "wallet_id" UUID NOT NULL);

CREATE UNIQUE INDEX "eTransactions_txid_i"
  ON "wallet"."eTransactions" ("txid");

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

