// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <string>
#include <map>
#include <stdio.h>

#include "../uint256.h"

namespace Collections {
	
	const std::string LOG_PATH_BLOCKS = "/home/ubuntu/.zcash/collections/blocks/";
  const std::string LOG_PATH_PEERS = "/home/ubuntu/.zcash/collections/peers/";

  // How many children blocks per block
  std::map<uint256, int> BlockNumChildren;

  // main.cpp:5973
  void BlockAdd(uint256 hash, uint256 parent_hash, std::time_t miner_time) {
    if (BlockNumChildren.find(hash) != BlockNumChildren.end()) {
      // BLOCK ALREADY SEEN
      std::cout << "ERROR: Block already seen " << hash.ToString() << "\n";;
      return;
    }

    //-- Timestamp --//
    std::time_t validated_time = std::time(0);
    
    //-- Offload data to file --//
		std::string file_path = LOG_PATH_BLOCKS + hash.ToString() + ".log";
		FILE *block_file = fopen(file_path.c_str(), "w+");
    if (block_file != NULL) {
      const char *block_format = 
        "Block Hash:\t%s\n"
        "Parent Hash:\t%s\n"
        "Miner Time:\t%d\n"
        "Valid Time:\t%d\n";

      fprintf(block_file, block_format,
        hash.ToString().c_str(),
        parent_hash.ToString().c_str(),
        miner_time,
        validated_time);
      fclose(block_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open block file - " << file_path << "\n";
    }

    //-- Store Num Children --//
    BlockNumChildren[hash] = 0;

    // Check for Fork
    BlockNumChildren[parent_hash] += 1;
    if (BlockNumChildren[parent_hash] > 1) {
      block_file = fopen(file_path.c_str(), "a");
      if (block_file != NULL) {
        fprintf(block_file, "FORK\n");
        fclose(block_file);
      } else {
        // TODO better error handler here
        std::cout << "ERROR: could not open block file - " << file_path << "\n";
      }

      std::cout<<"ALERT: FORK!\n";
    }
  }

  // main.cpp:5609
  void PeerBlockSeen(uint256 hash, std::string node_ip) {
    //-- Timestamp --//
    std::time_t seen_time = std::time(0);

    //-- Offload data to file --//
    std::string file_path = LOG_PATH_PEERS + node_ip + ".log";
    FILE *peer_file = fopen(file_path.c_str(), "a");
    if (peer_file != NULL) {
      const char *peer_format = "%s\t%d\n";

      fprintf(peer_file, peer_format, hash.ToString().c_str(), seen_time);
      fclose(peer_file);
    } else {
      // TODO better error handler here
      std::cout << "ERROR: could not open peer file" << file_path << "\n";
    }
  }
}

