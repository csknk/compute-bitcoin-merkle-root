Calculate Bitcoin Merkle Root
=============================

[Sample block].

Get sample transactions:

Start `bitcoind`.

Fetch block and process results with `jq` to obtain a file comprised of `txid` hashes for all transactions in the block, ech on a separate line. The file `/tmp/tx` can then be used as input to the programme via input redirection 

```bash
bitcoin-cli getblock 00000000000000000002f5b1c49b9ddf5537d418b6c5b835172b3987a09a4b13 | jq -r '.tx[]' > /tmp/tx
```

[1]: https://www.blockchain.com/btc/block/00000000000000000002f5b1c49b9ddf5537d418b6c5b835172b3987a09a4b13
