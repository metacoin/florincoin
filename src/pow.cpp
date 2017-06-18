// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{

    static const int nMinActualTimespan_Version1 = params.nAveragingTargetTimespan_Version1 * (100 - params.nMaxAdjustUp_Version1) / 100;
    static const int nMaxActualTimespan_Version1 = params.nAveragingTargetTimespan_Version1 * (100 + params.nMaxAdjustDown_Version1) / 100;
    static const int nMinActualTimespan_Version2 = params.nAveragingTargetTimespan_Version2 * (100 - params.nMaxAdjustUp_Version2) / 100;
    static const int nMaxActualTimespan_Version2 = params.nAveragingTargetTimespan_Version2 * (100 + params.nMaxAdjustDown_Version2) / 100;
    static const int nMinActualTimespan_Version3 = params.nAveragingTargetTimespan_Version3 * (100 - params.nMaxAdjustUp_Version3) / 100;
    static const int nMaxActualTimespan_Version3 = params.nAveragingTargetTimespan_Version3 * (100 + params.nMaxAdjustDown_Version3) / 100;

    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    unsigned int nInterval;
    unsigned int nTargetTimespan;
    unsigned int nAveragingInterval;
    unsigned int nMinActualTimespan;
    unsigned int nMaxActualTimespan;
    unsigned int nAveragingTargetTimespan;

    if (pindexLast->nHeight+1 < params.nHeight_Version2)
    {
        nAveragingInterval = params.nAveragingInterval_Version1;
        nInterval = params.nInterval_Version1;
        nTargetTimespan = params.nPowTargetTimespan_Version1;
        nMinActualTimespan = nMinActualTimespan_Version1;
        nMaxActualTimespan = nMaxActualTimespan_Version1;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version1;
    }
    else
    if (pindexLast->nHeight+1 < params.nHeight_Version3)
    {
        nAveragingInterval = params.nAveragingInterval_Version2;
        nInterval = params.nInterval_Version2;
        nTargetTimespan = params.nPowTargetTimespan_Version2;
        nMinActualTimespan = nMinActualTimespan_Version2;
        nMaxActualTimespan = nMaxActualTimespan_Version2;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version2;
    }
    else
    {
        nAveragingInterval = params.nAveragingInterval_Version3;
        nInterval = params.nInterval_Version3;
        nTargetTimespan = params.nPowTargetTimespan_Version3;
        nMinActualTimespan = nMinActualTimespan_Version3;
        nMaxActualTimespan = nMaxActualTimespan_Version3;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version3;
    }


    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % nInterval != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % nInterval != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // Florincoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = nAveragingInterval-1;
    if ((pindexLast->nHeight+1) != nAveragingInterval)
        blockstogoback = nAveragingInterval;

    // Go back by what we want to be nAveragingInterval worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    static const int nMinActualTimespan_Version1 = params.nAveragingTargetTimespan_Version1 * (100 - params.nMaxAdjustUp_Version1) / 100;
    static const int nMaxActualTimespan_Version1 = params.nAveragingTargetTimespan_Version1 * (100 + params.nMaxAdjustDown_Version1) / 100;
    static const int nMinActualTimespan_Version2 = params.nAveragingTargetTimespan_Version2 * (100 - params.nMaxAdjustUp_Version2) / 100;
    static const int nMaxActualTimespan_Version2 = params.nAveragingTargetTimespan_Version2 * (100 + params.nMaxAdjustDown_Version2) / 100;
    static const int nMinActualTimespan_Version3 = params.nAveragingTargetTimespan_Version3 * (100 - params.nMaxAdjustUp_Version3) / 100;
    static const int nMaxActualTimespan_Version3 = params.nAveragingTargetTimespan_Version3 * (100 + params.nMaxAdjustDown_Version3) / 100;

    unsigned int nInterval;
    unsigned int nTargetTimespan;
    unsigned int nAveragingInterval;
    unsigned int nMinActualTimespan;
    unsigned int nMaxActualTimespan;
    unsigned int nAveragingTargetTimespan;

    if (pindexLast->nHeight+1 < params.nHeight_Version2)
    {
        nAveragingInterval = params.nAveragingInterval_Version1;
        nInterval = params.nInterval_Version1;
        nTargetTimespan = params.nPowTargetTimespan_Version1;
        nMinActualTimespan = nMinActualTimespan_Version1;
        nMaxActualTimespan = nMaxActualTimespan_Version1;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version1;
    }
    else
    if (pindexLast->nHeight+1 < params.nHeight_Version3)
    {
        nAveragingInterval = params.nAveragingInterval_Version2;
        nInterval = params.nInterval_Version2;
        nTargetTimespan = params.nPowTargetTimespan_Version2;
        nMinActualTimespan = nMinActualTimespan_Version2;
        nMaxActualTimespan = nMaxActualTimespan_Version2;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version2;
    }
    else
    {
        nAveragingInterval = params.nAveragingInterval_Version3;
        nInterval = params.nInterval_Version3;
        nTargetTimespan = params.nPowTargetTimespan_Version3;
        nMinActualTimespan = nMinActualTimespan_Version3;
        nMaxActualTimespan = nMaxActualTimespan_Version3;
        nAveragingTargetTimespan = params.nAveragingTargetTimespan_Version3;
    }
    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Florincoin: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
