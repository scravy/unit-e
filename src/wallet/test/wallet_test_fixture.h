// Copyright (c) 2016-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UNITE_WALLET_TEST_WALLET_TEST_FIXTURE_H
#define UNITE_WALLET_TEST_WALLET_TEST_FIXTURE_H

#include <test/test_unite.h>
#include <test/test_unite_mocks.h>

#include <proposer/block_builder.h>
#include <wallet/wallet.h>

#include <memory>

//! Testing setup and teardown for wallet.
struct WalletTestingSetup : public TestingSetup {
  explicit WalletTestingSetup(
      const std::string &chainName = CBaseChainParams::TESTNET,
      UnitEInjectorConfiguration config = UnitEInjectorConfiguration());

  explicit WalletTestingSetup(
      std::function<void(Settings&)> f,
      const std::string &chainName = CBaseChainParams::TESTNET,
      UnitEInjectorConfiguration config = UnitEInjectorConfiguration());

  ~WalletTestingSetup();

  Settings settings;
  std::shared_ptr<CWallet> m_wallet;
  mocks::StakeValidatorMock stake_validator_mock;
};

//
// Testing fixture that pre-creates a
// 100-block REGTEST-mode block chain
//
struct TestChain100Setup : public WalletTestingSetup {
  TestChain100Setup(UnitEInjectorConfiguration config = UnitEInjectorConfiguration());

  //! \brief Create a new block with given transactions
  std::shared_ptr<const CBlock> CreateBlock(const std::vector<CMutableTransaction>& txns,
                                            const CScript& coinbase_script,
                                            boost::optional<staking::Coin> stake = boost::none);

  //! \brief Add given block to the current chain
  bool ProcessBlock(std::shared_ptr<const CBlock> block);

  //! \brief Create new block and add it to the current chain
  CBlock CreateAndProcessBlock(const std::vector<CMutableTransaction>& txns,
                               const CScript& coinbase_script,
                               boost::optional<staking::Coin> stake = boost::none,
                               bool *processed = nullptr);

  ~TestChain100Setup();

  std::vector<CTransaction> m_coinbase_txns; // For convenience, coinbase transactions
  CKey coinbaseKey; // private/public key needed to spend coinbase transactions
  std::unique_ptr<proposer::BlockBuilder> m_block_builder;
  std::unique_ptr<staking::ActiveChain> m_active_chain;
  std::unique_ptr<blockchain::Behavior> m_behavior;
};

#endif // UNITE_WALLET_TEST_WALLET_TEST_FIXTURE_H
