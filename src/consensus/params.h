// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include "uint256.h"
#include <map>
#include <string>

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_CSV, // Deployment of BIP68, BIP112, and BIP113.
    DEPLOYMENT_SEGWIT, // Deployment of BIP141, BIP143, and BIP147.
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
    MAX_VERSION_BITS_DEPLOYMENTS
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargetting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
    uint256 nMinimumChainWork;

    // ToDo: bitspill, metacoin. hacked in.
    unsigned int nPowTargetTimespan_Version1;
    unsigned int nInterval_Version1;
    unsigned int nHeight_Version2;
    unsigned int nInterval_Version2;
    unsigned int nPowTargetTimespan_Version2;
    unsigned int nHeight_Version3;
    unsigned int nInterval_Version3;
    unsigned int nPowTargetTimespan_Version3;
    unsigned int nMaxAdjustDown_Version1;
    unsigned int nMaxAdjustUp_Version1;
    unsigned int nMaxAdjustDown_Version2;
    unsigned int nMaxAdjustUp_Version2;
    unsigned int nMaxAdjustDown_Version3;
    unsigned int nMaxAdjustUp_Version3;
    unsigned int nPowTargetTimespanAdjDown_Version1;
    unsigned int nPowTargetTimespanAdjDown_Version2;
    unsigned int nPowTargetTimespanAdjDown_Version3;
    unsigned int nAveragingInterval_Version1;
    unsigned int nAveragingInterval_Version2;
    unsigned int nAveragingInterval_Version3;
    unsigned int nAveragingTargetTimespan_Version1;
    unsigned int nAveragingTargetTimespan_Version2;
    unsigned int nAveragingTargetTimespan_Version3;

};
} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
