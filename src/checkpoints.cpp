// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
    (     0, uint256("0x00000da06cd87b66c2dd4f8df68031815af3297c713e962f72af0349d53216d1") )
    (     1, uint256("0x00000db4be834e9b5615c7d57d463d31c9ac97e22166f763444b6e54164bb3dd") )
    (     2, uint256("0x00000f55632d78f57adaf4b54df8dbb092d219379ebbdd4e99e637cf0cc7ad95") )
    (     3, uint256("0x0000021bfb7dfa609fca2f3d29344c25bc69c175819174b46fd70b83fffa939b") )
    (     4, uint256("0x000004ecb20ca01864ac45d4e4a64507ccde88a6e6e75daac1639382c45d1163") )
    (     5, uint256("0x000001c2b69925e76f5d1c3e4f398bce88ce08adcbeca54209578ea60cb09047") )
    (     6, uint256("0x000004031be3dec954b5aef07af663d80793e1d8d0e096c452004a7416f16b23") )
    (     7, uint256("0x00000189c84b75c6adc4bb8b8c5bc47e36bd9787215f10ca22bebc381e8f373d") )
    (     8, uint256("0x000001181add9211ef50b27835a2cd5c3df9c0bf818dba70ed45170d687f314f") )
    (     9, uint256("0x000000ac3cf05cbcc090fdeb8f25b7c41fd0f439842ddf35260ca7cab969173f") )
				
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
