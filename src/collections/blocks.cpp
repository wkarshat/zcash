// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <string>
#include <map>
#include <stdio.h>

#include "../uint256.h"

namespace Collections {
	
	const std::string LOG_PATH_BLOCKS = "/home/ubuntu/.zcash/collections/blocks/";

	typedef struct Block_t {
		uint256 parent_hash;
		std::time_t miner_time;
		std::time_t peer_time;
		std::time_t validated_time;
	} Block;

	// List of blocks
	std::map<uint256, Block*> blocks;

	void AddBlock(uint256 hash) {

		if (blocks.find(hash) == blocks.end()) {
			// Block not created yet
			blocks[hash] = (Block*)calloc(1, sizeof(Block));
		}
	}

	// main.cpp:5975
	void OffloadBlock(uint256 hash) {

		if (blocks.find(hash) == blocks.end()) {
			// Block does not exist
			return;
		}

		// Open file
		std::string file_path = LOG_PATH_BLOCKS + hash.ToString() + ".log";
		FILE *block_file = fopen(file_path.c_str(), "w+");
    if (block_file == NULL) {
      // ERROR FILE COULD NOT BE OPEN
      return;
    }

		const char *block_format = 
      "Block Hash:\t%s\n"
      "Parent Hash:\t%s\n"
      "Miner Time:\t%d\n"
      "Peer Time:\t%d\n"
      "Valid Time:\t%d\n";

		fprintf(block_file, block_format,
      hash.ToString().c_str(),
      blocks[hash]->parent_hash.ToString().c_str(),
      blocks[hash]->miner_time,
      blocks[hash]->peer_time,
      blocks[hash]->validated_time);
		fclose(block_file);

    // dump block
    delete blocks[hash];
		blocks.erase(hash);
	}

	// main.cpp:5970
	void BlockParentSet(uint256 hash, uint256 parent_hash) {
		AddBlock(hash);
		
		blocks[hash]->parent_hash = parent_hash;
	}

	// main.cpp:5971
	void BlockMinerTimeSet(uint256 hash, std::time_t miner_time) {
		AddBlock(hash);

		blocks[hash]->miner_time = miner_time;
	}

	// main.cpp:5910
	void BlockPeerTimeSet(uint256 hash) {
		AddBlock(hash);

		// Create timestamp
		blocks[hash]->peer_time = std::time(0); 
	} 

	// main.cpp:5972
	void BlockValidatedTimeSet(uint256 hash) {
		AddBlock(hash);

		// Create timestamp
		blocks[hash]->validated_time = std::time(0); 
	}
}

