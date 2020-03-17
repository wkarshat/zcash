// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <sys/time.h>
#include <string>
#include <map>
#include <stdio.h>

#include "../uint256.h"

namespace Collections {

  typedef unsigned long long timestamp_t;

  const std::string LOG_PATH_BLOCKS = "/home/ubuntu/.zcash/collections/blocks/";
  const std::string LOG_PATH_PEERS = "/home/ubuntu/.zcash/collections/peers/";

  // get current time in milliseconds since epoch
  timestamp_t CurrentTimeMilli() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (timestamp_t)(tv.tv_sec) * 1000 +
    (timestamp_t)(tv.tv_usec) / 1000;
  }

  // main.cpp:5973
  void BlockAdd(uint256 hash, uint256 parent_hash, std::time_t miner_time) {
    static uint256 last_block_hash;

    //-- Timestamp --//
    timestamp_t validated_time = CurrentTimeMilli();

    //-- Fork Decection --//
    bool is_fork = false;
    if (last_block_hash.IsNull() || last_block_hash == parent_hash) {
      last_block_hash = hash;
    } else {
      // FORK
      is_fork = true;
      std::cout << "FORK\n\tParent Hash: " << parent_hash.ToString() << "\n\tLast Seen Hash: " << last_block_hash.ToString() << "\n";
      last_block_hash.SetNull();
    }

    // TEST
    std::cout << "LAST BLOCK: " << last_block_hash.ToString() << "\n";
    
    //-- Offload data to file --//
		std::string file_path = LOG_PATH_BLOCKS + hash.ToString() + ".log";
		FILE *block_file = fopen(file_path.c_str(), "w+");
    if (block_file != NULL) {
      const char *block_format = 
        "Block Hash:\t%s\n"
        "Parent Hash:\t%s\n"
        "Miner Time:\t%d\n"
        "Valid Time:\t%llu\n";

      fprintf(block_file, block_format,
        hash.ToString().c_str(),
        parent_hash.ToString().c_str(),
        miner_time,
        validated_time);
      if (is_fork) {
        fprintf(block_file, "FORK\n");
      }
      fclose(block_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open block file - " << file_path << "\n";
    }
  }

  // main.cpp:5609
  void PeerBlockSeen(uint256 hash, std::string node_ip) {
    //-- Timestamp --//
    timestamp_t seen_time = CurrentTimeMilli();

    //-- Offload data to file --//
    std::string file_path = LOG_PATH_PEERS + node_ip + ".log";
    FILE *peer_file = fopen(file_path.c_str(), "a");
    if (peer_file != NULL) {
      const char *peer_format = "%s\t%llu\n";

      fprintf(peer_file, peer_format, hash.ToString().c_str(), seen_time);
      fclose(peer_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open peer file" << file_path << "\n";
    }
  }
}

