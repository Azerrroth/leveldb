#include "interface.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "leveldb/db.h"

using namespace std;

int main(void) {
  leveldb::DB* db;
  ReDB* redb;
  leveldb::Options options;
  options.create_if_missing = true;

  // open
  leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &redb);
  assert(status.ok());

  string key = "keytest";
  string value = "this is the value of keytest";

  // write
  status = db->Put(leveldb::WriteOptions(), key, value);
  assert(status.ok());

  // read
  status = db->Get(leveldb::ReadOptions(), key, &value);
  assert(status.ok());

  cout << "[Key] " << key << " [Value] " << value << endl;

  // delete
  status = db->Delete(leveldb::WriteOptions(), key);
  assert(status.ok());

  status = db->Get(leveldb::ReadOptions(), key, &value);
  assert(!status.ok());

  // close
  delete db;

  return 0;
}
