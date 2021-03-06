#!/usr/bin/env python3
# Copyright (c) 2018-2019 The Unit-e developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import UnitETestFramework


class SpendGenensisTest(UnitETestFramework):

    def set_test_params(self):
        self.setup_clean_chain = True
        self.num_nodes = 1

    def run_test(self):
        node = self.nodes[0]
        block = node.getblock(node.getblockhash(0))
        assert block['hash']

        self.setup_stake_coins(node)
        node.rescanblockchain(0, 0)  # UNIT-E this can be removed as soon as importmasterkey correctly perform a rescan

        # Make the coinbase mature
        node.generate(100)
        assert node.getbalance() == 10000

        # Send a tx to yourself spending the tx
        node.sendtoaddress(node.getnewaddress(), 5000)


if __name__ == '__main__':
    SpendGenensisTest().main()
