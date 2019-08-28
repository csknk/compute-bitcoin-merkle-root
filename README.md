Calculate Bitcoin Merkle Root
=============================
Educational project to compute Bitcoin Merkle root from block transaction ids.

Built on Ubuntu 16.04. Requires OpenSSL.

Merkle Trees
------------
Merkle trees are an efficient way to verify that an element is in a set, without having to store the full set.

The leaf nodes (lowest level nodes) of the Merkle tree are made up of hashes of individual data members of the set (in the case of Bitcoin, blocks of transaction data). Adjacent leaves are concatenated pairwise, and the hash of the concatenation constitutes the node's parent.

Parent nodes are concatenated and hashed in a similar way to generate another level of parent nodes. This process is repeated until a single hash remains - the Merkle root.

Each non-leaf node of a merkle tree is a hash of the concatenation of it's immediate children. The leaves of the tree are the elements of the set to which the Merkle tree proves membership.

Merkle Trees as Implemented By Bitcoin
--------------------------------------
In the case of Bitcoin, the leaves of the Merkle tree are the transaction identifiers (the hash of a raw transaction).

If there are an odd number of nodes at any level, the final node is concatenated with itself.  Parent nodes are in turn concatenated in pairs and hashed, with the process repeated until a single Merkle hash remains - the Merkle root.

The Merkle root is stored in a block header, where it serves to make transactions tamper-proof - if a transaction is changed, the Merkle root would be thrown off. Because the hash of each block is included in subsequent blocks, the tamper would be immediately evident and the block with the tampered transaction would not be accepted as valid by the Bitcoin consensus rules.

To verify a transaction - check that a transaction is in a valid block - you just need the hashes of Merkle branch to compute the Merkle root, not the entire set of transactions in the block. 

C++ Implementation
------------------

Code from [bitcoin.cpp][3] in this repo:
```c++

/**
 * Compute the Merkle root 
 * */
void merkleRoot(std::vector<Bytes> txids, Bytes& result)
{
	if (txids.empty()) {
		throw;
	}
	while (txids.size() > 1) {
		// If odd number, add the last element to end of vector.
		// Note that this is required at every level of the tree.
		if (txids.size() & 1) {
			txids.push_back(txids.back());
		}
		std::vector<Bytes> tmp;
		for (auto it = std::begin(txids); it != std::end(txids) && std::next(it) != txids.end(); it += 2) {
			Bytes concat = *it;
			Bytes result(hash_size);
			concat.insert(concat.end(), (*(it + 1)).begin(), (*(it + 1)).end());
			doubleSHA256(concat.data(), 64/*concat.size()*/, result);
			tmp.push_back(result);
			concat.clear();
		}
		txids = tmp;
	}
	result = txids[0];
}

```

Build
-----
Build the project:

* Clone this repo.
* `cd` into project directory.
* Run `make`.

Test
----
Get the transactions from a [sample block][1]. Run programme, and compare the computed Merkle root with the value held in the block header.

Sample Transactions
-------------------
Get sample transactions:

Start `bitcoind`.

Fetch block and process results with `jq` to obtain a file comprised of `txid` hashes for all transactions in the block, ech on a separate line. The file `/tmp/tx` can then be used as input to the programme via input redirection 

```bash
bitcoin-cli getblock 00000000000000000002f5b1c49b9ddf5537d418b6c5b835172b3987a09a4b13 | jq -r '.tx[]' > /tmp/txid.manifest
```

Build the programme and run, passing in your `txid.manifest` to stdin by file redirection:

```bash
./bin/main < /tmp/txid.manifest
```
References
----------
* [Bitcoin core Merkle root computation][2]


[1]: https://www.blockchain.com/btc/block/00000000000000000002f5b1c49b9ddf5537d418b6c5b835172b3987a09a4b13
[2]: https://github.com/bitcoin/bitcoin/blob/master/src/consensus/merkle.cpp
[3]: bitcoin.cpp
