// Block Collection
// /src/collections/blocks.cpp

#include <ctime>
#include <string>
#include <map>
#include <stdio.h>

#include "../uint256.h"

namespace Collections {
	
	const std::string LOG_PATH = "~/.zcash/collections/";

	typedef struct Block_t {
		uint256 hash;
		uint256 parent_hash;
		std::time_t miner_time;
		std::time_t peer_time;
		std::time_t validated_time;
	} Block;

	// List of blocks
	std::map<uint256, Block*> blocks;

	void AddBlock(uint256 hash) {
		std::cout<< "ADD BLOCK\n";
		if (Collections::blocks.find(hash) == Collections::blocks.end()) {
			// Block not created yet
			Collections::blocks[hash] = (Block*)malloc(sizeof(Block));
		}
	}

	// main.cpp:5975
	void OffloadBlock(uint256 hash) {
		if (Collections::blocks.find(hash) == Collections::blocks.end()) {
			// Block does not exist
			return;
		}

		Block *block = blocks[hash];

		// Open file
		std::string file_path = LOG_PATH + hash.ToString() + ".block";
		FILE *block_file = fopen(file_path.c_str(), "w");

		const char *block_format = "Block Hash:\t%s\n"
					"Parent Hash:\t%s\n"
					"Miner Time:\t%d\n"
					"Peer Time:\t%d\n"
					"Validation Time:\t%d\n";

		fprintf(block_file, block_format,
				block->hash,
				block->parent_hash,
			       	block->miner_time,
			       	block->peer_time,
			       	block->validated_time);
		fclose(block_file);

		Collections::blocks.erase(hash);
	}

	// main.cpp:5609
	void BlockHeaderSet(uint256 hash) {
		AddBlock(hash);
		
		Collections::blocks[hash]->hash = hash;
	}

	// main.cpp:5970
	void BlockParentSet(uint256 hash, uint256 parent_hash) {
		AddBlock(hash);
		
		Collections::blocks[hash]->parent_hash = parent_hash;
	}

	// main.cpp:5971
	void BlockMinerTimeSet(uint256 hash, std::time_t miner_time) {
		AddBlock(hash);

		Collections::blocks[hash]->miner_time = miner_time;
	}

	// main.cpp:5910
	void BlockPeerTimeSet(uint256 hash) {
		AddBlock(hash);

		// Create timestamp
		Collections::blocks[hash]->peer_time = std::time(0); 
	} 

	// main.cpp:5972
	void BlockValidatedTimeSet(uint256 hash) {
		AddBlock(hash);

		// Create timestamp
		Collections::blocks[hash]->validated_time = std::time(0); 
	}
}

