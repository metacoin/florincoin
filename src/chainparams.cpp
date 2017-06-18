// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 2;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.strTxComment = "text:Florincoin genesis block";

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Slashdot - 17 June 2013 - Saudi Arabia Set To Ban WhatsApp, Skype";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 710000;
        consensus.BIP34Hash = uint256S("fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        //consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4

        consensus.nPowTargetSpacing = 40; // Florincoin: 40 seconds (~1/4x Litecoin: 2.5 minutes)

        consensus.nPowTargetTimespan_Version1 = 60 * 60; // Florincoin: 60 minutes (Litecoin: 3.5 days)
        consensus.nInterval_Version1 = consensus.nPowTargetTimespan_Version1 / consensus.nPowTargetSpacing; // Florincoin: 90 blocks

        consensus.nHeight_Version2 = 208440;
        nInterval_Version2 = 15;
        consensus.nPowTargetTimespan_Version2 = nInterval_Version2 * consensus.nPowTargetSpacing; // 10 minutes

        consensus.nHeight_Version3 = 426000;
        nInterval_Version3 = 1;
        consensus.nPowTargetTimespan_Version3 = nInterval_Version3 * consensus.nPowTargetSpacing; // 40 seconds

        consensus.nPowTargetTimespanAdjDown_Version1 = consensus.nPowTargetTimespan_Version1 * (100 + consensus.nMaxAdjustDown_Version1) / 100;
        consensus.nPowTargetTimespanAdjDown_Version2 = consensus.nPowTargetTimespan_Version2 * (100 + consensus.nMaxAdjustDown_Version2) / 100;
        consensus.nPowTargetTimespanAdjDown_Version3 = consensus.nPowTargetTimespan_Version3 * (100 + consensus.nMaxAdjustDown_Version3) / 100;

        consensus.nAveragingInterval_Version1 = nInterval_Version1;
        consensus.nAveragingInterval_Version2 = nInterval_Version2;
        consensus.nAveragingInterval_Version3 = 6;

        consensus.nAveragingTargetTimespan_Version1 = consensus.nAveragingInterval_Version1 * consensus.nPowTargetSpacing;
        consensus.nAveragingTargetTimespan_Version2 = consensus.nAveragingInterval_Version2 * consensus.nPowTargetSpacing;
        consensus.nAveragingTargetTimespan_Version3 = consensus.nAveragingInterval_Version3 * consensus.nPowTargetSpacing;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000005c13f99f6d0b1a908");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 7313;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1371488396, 1000112548, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x09c7781c9df90708e278c35d38ea5c9041d7ecfcdd1c56ba67274b7cff3e1cea"));
        assert(genesis.hashMerkleRoot == uint256S("0x730f0c8ddc5a592d5512566890e2a73e45feaa6748b24b849d1c29a7ab2b2300"));

        vSeeds.push_back(CDNSSeedData("seed1.florincoin.org", "seed1.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed2.florincoin.org", "seed2.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed3.florincoin.org", "seed3.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed4.florincoin.org", "seed4.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed5.florincoin.org", "seed5.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed6.florincoin.org", "seed6.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed7.florincoin.org", "seed7.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed8.florincoin.org", "seed8.florincoin.org"));
        vSeeds.push_back(CDNSSeedData("seed1.florincoin.com", "seed1.florincoin.com"));
        vSeeds.push_back(CDNSSeedData("seed2.florincoin.com", "seed2.florincoin.com"));
        vSeeds.push_back(CDNSSeedData("seed3.florincoin.com", "seed3.florincoin.com"));
        vSeeds.push_back(CDNSSeedData("seed4.florincoin.com", "seed4.florincoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 35);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 8);

        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        // Florincoin: Mainnet v2 enforced, transition from 0.8.7 to 0.10.4
        nEnforceV2AfterHeight = 1679161;

	checkpointData = (CCheckpointData) {
        boost::assign::map_list_of

            (     0, uint256S("0x09c7781c9df90708e278c35d38ea5c9041d7ecfcdd1c56ba67274b7cff3e1cea"))
            (  8002, uint256S("0x73bc3b16d99bbf797f396c9532f80c3b73bb21304280de2efbc5edcb75739234"))
            ( 18001, uint256S("0x5a7a4821aa4fc7ee3dea2f8319e9fa4d991a8c6762e79cb624c64e4cf1031582"))
            ( 38002, uint256S("0x4962437c6d0a450f44c1e40cd38ff220f8122af1517e1329f1abd07fb7791e40"))
            (160002, uint256S("0x478d381c92298614c3a05fb934a4fffc4d3e5b573efbba9b3e8b2ce8d26a0f8f"))
            (208001, uint256S("0x2bb3f8b2d5081aefa0af9f5d8de42bd73a5d89eebf78aa7421cd63dc40a56d4c"))
            (270001, uint256S("0x74988a3179ae6bbc5986e63f71bafc855202502b07e4d9331015eee82df80860"))
            (290036, uint256S("0x145994381e5e4f0e5674adc1ace9a03b670838792f6bd6b650c80466453c2da3"))
            (344665, uint256S("0x40fe36d8dec357aa529b6b1d99b2989a37ed8c7b065a0e3345cd15a751b9c1ad"))
            (400236, uint256S("0xf9a4b8e21d410539e45ff3f11c28dee8966de7edffc45fd02dd1a5f4e7d4ef38"))
            (415000, uint256S("0x16ef8ab98a7300039a5755d5bdc00e31dada9d2f1c440ff7928f43c4ea41c0a8"))
            (420937, uint256S("0x48a75e4687021ec0dda2031439de50b61933e197a4e1a1185d131cc2b59b8444"))
            (425606, uint256S("0x62c8d811b1a49f6fdaffded704dc48b1c98d6f8dd736d8afb96c9b097774a85e"))
            (508694, uint256S("0x65cde197e9118e5164c4dcdcdc6fcfaf8c0de605d569cefd56aa220e7739da6a"))
            (696454, uint256S("0x8cfb75684405e22f8f69522ec11f1e5206758e37f25db13880548f69fe6f1976"))
            (955000, uint256S("0xb5517a50aee6af59eb0ab4ee3262bcbaf3f6672b9301cdd3302e4bab491e7526"))
            (1505017, uint256S("0xd38b306850bb26a5c98400df747d4391bb4e359e95e20dc79b50063ed3c5bfa7")),
            1449731549, // * UNIX timestamp of last checkpoint block
            1993612,   // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            2160.0     // * estimated number of transactions per day after checkpoint
    };

    }
};
static CMainParams mainParams;

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256S("");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 40;
        consensus.nPowTargetTimespan = consensus.nPowTargetSpacing; // 3.5 days
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        //consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // Never / undefined
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000000000872d04d7");

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0x5a;
        pchMessageStart[3] = 0xf2;
        nDefaultPort = 17313;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1486949366, 293345, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x4966625a4b2851d9fdee139e56211a0d88575f59ed816ff5e6a63deb4e3e29a0"));

        // todo: verify FLO testnet merkle root
        assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testseed.florincoin.org", "testseed.florincoin.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,115);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,198);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x9b7bc86236c34b5e3a39367c036b7fe8807a966c22a7a1f0da2a198a27e03731")),
            1371387277,
            547,
            2160
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 40; // two weeks
        consensus.nPowTargetSpacing = 40;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0x66;
        pchMessageStart[1] = 0x66;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0x66;
        nDefaultPort = 17314;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x327ce75b073d20561340facc578c53408c10472f363fcafaf8d1221a083cd319"));
        assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

	checkpointData = (CCheckpointData){
		boost::assign::map_list_of
			( 0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")),
			0,
			0,
			0
	};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}

