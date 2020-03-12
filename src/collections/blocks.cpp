// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <string>
#include <map>
#include <stdio.h>

#include "../uint256.h"

namespace Collections {
	
	const std::string LOG_PATH_BLOCKS = "/home/ubuntu/.zcash/collections/blocks/";

  // How mant children blocks per block
  std::map<uint256, int> BlockNumChildren;

  void BlockAdd(uint256 hash, uint256 parent_hash, std::time_t miner_time) {
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
      std::cout << "ERROR: could not open file\n";
    }

    //-- Store Num Children --//
    BlockNumChildren[hash] = 0;

    BlockNumChildren[parent_hash] += 1;
    if (BlockNumChildren[parent_hash] > 1) {
      // FORK
      std::cout<<"ALERT: FORK!\n";
    }
  }
}

