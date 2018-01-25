/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "wallet"."eAddresses" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "wallet"."eAddresses" (
  "p2pkh_address" TEXT NOT NULL,
  "sw_bech32_address" TEXT NOT NULL,
  "sw_p2sh_address" TEXT NOT NULL,
  "name" TEXT NOT NULL,
  "purpose" TEXT NOT NULL,
  "time" BIGINT NOT NULL,
  "is_used" BOOLEAN NOT NULL DEFAULT FALSE,
  "wallet_id" UUID NOT NULL,
  "id" SERIAL NOT NULL PRIMARY KEY);

CREATE UNIQUE INDEX "eAddresses_p2pkh_address_i"
  ON "wallet"."eAddresses" ("p2pkh_address");

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

