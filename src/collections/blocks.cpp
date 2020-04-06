// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <sys/time.h>
#include <string>
#include <map>
#include <stdio.h>

namespace Collections {

  typedef unsigned long long timestamp_t;

  const std::string LOG_VERSION = "v0";
  const std::string LOG_PATH_BLOCKS = "/collections/blocks/";
  const std::string LOG_PATH_INV = "/collections/inv/";
  const std::string LOG_PATH_PEERS = "/collections/peers/";

  // get current time in milliseconds since epoch
  static timestamp_t CurrentTimeMilli() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (timestamp_t)(tv.tv_sec) * 1000 +
    (timestamp_t)(tv.tv_usec) / 1000;
  }

  // main.cpp:5973
  void BlockAdd(const CBlock* block, const uint256 hash, const int height, const bool accepted) {
    timestamp_t validated_time = CurrentTimeMilli();

    //-- Decode miner difficulty --//
    arith_uint256 bnTarget;
    bnTarget.SetCompact(block->nBits);
    
    //-- Offload data to file --//

    // Create file
    std::string hash_str = hash.ToString();
		std::string file_path = GetDataDir(false).string() + LOG_PATH_BLOCKS
      + std::to_string(height) + "_"
      + hash_str.substr(hash_str.length() - 4) + "_"
      + LOG_VERSION + ".log";
		FILE *block_file = fopen(file_path.c_str(), "w+");

    if (block_file != NULL) {

      // Header
      const char* block_format =
        "Block Height:\t%d\n"
        "Block Hash:\t%s\n"
        "Prev Hash:\t%s\n"
        "Miner Target:\t%s\n"
        "Miner Time:\t%d\n"
        "Valid Time:\t%llu\n"
        "Accepted:\t%s\n";
      fprintf(block_file, block_format,
        height,
        hash_str.c_str(),
        block->hashPrevBlock.ToString().c_str(),
        bnTarget.ToString().c_str(),
        std::time_t(block->nTime),
        validated_time,
        accepted ? "true" : "false");

      // Transactions
      fprintf(block_file, "Transactions:\n");
      const char* tx_format = "\t%d:\t%s\n";
      int vtx_len = block->vtx.size();
      for (int i = 0; i < vtx_len; i ++) {
        fprintf(block_file, tx_format, i, block->vtx[i].GetHash().ToString().c_str());
      }

      fclose(block_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open block file - " << file_path << "\n";
    }
  }

  // main.cpp:5609
  // Blocks and Transactions
  void InvAdd(uint256 hash, std::string node_ip) {
    //-- Timestamp --//
    timestamp_t seen_time = CurrentTimeMilli();

    //-- Offload data to file --//
    std::replace(node_ip.begin(), node_ip.end(), '.', '_');
    std::string file_path = GetDataDir(false).string() + LOG_PATH_INV
      + node_ip + "_"
      + LOG_VERSION + ".log";
    FILE *inv_file = fopen(file_path.c_str(), "a");
    if (inv_file != NULL) {
      const char *inv_format = "%s\t%llu\n";

      fprintf(inv_file, inv_format, hash.ToString().c_str(), seen_time);
      fclose(inv_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open inv file" << file_path << "\n";
    }
  }

 void PeerAdd(CNode *peer, long long nTime, unsigned long long nNonce) {
    timestamp_t add_time = CurrentTimeMilli();

    // Create file
    std::string file_path = GetDataDir(false).string() + LOG_PATH_PEERS + "peers_" + LOG_VERSION + ".log";
    FILE *peer_file = fopen(file_path.c_str(), "a");

    if (peer_file != NULL) {

      // Order: IP, Services, peer_time, cur_time
      const char* peer_format = "%s,%llu,%lld,%llu,%llu\n";
      fprintf(peer_file, peer_format,
          peer->addr.ToStringIP().c_str(),
          (unsigned long long)(peer->nServices),
          nTime,
          add_time,
          nNonce);
    }

    fclose(peer_file);
  }
}
