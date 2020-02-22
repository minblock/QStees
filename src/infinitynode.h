// Copyright (c) 2018-2019 QTIPARRAY developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef QTIPARRAY_INFINITYNODE_H
#define QTIPARRAY_INFINITYNODE_H

#include <key.h> // for typr int65_t
#include <validation.h>
#include <script/standard.h>
#include <key_io.h>

using namespace std;

class CInfinitynode;
class CConnman;

struct infinitynode_info_t
{
    infinitynode_info_t() = default;
    infinitynode_info_t(infinitynode_info_t const&) = default;

    infinitynode_info_t(int protoVer, int64_t sTime) :
        nProtocolVersion{protoVer}, sigTime{sTime} 
    {}
    infinitynode_info_t(int protoVer, int64_t sTime, COutPoint const& outpointBurnFund):
        nProtocolVersion{protoVer}, sigTime{sTime} , vinBurnFund{outpointBurnFund}
    {}

    int nProtocolVersion = 0;
    int64_t sigTime = 0;
    CTxIn vinBurnFund{};

    int nHeight = -1;
    int nExpireHeight = -1;
    int nLastRewardHeight = -1;
    int nNextRewardHeight = -1;
    CAmount nBurnValue = 0;
    int nQTIPARRAYType = 0;
    std::string collateralAddress = "";
    CScript scriptPubKey{};
    std::string backupAddress = "BackupAddress";
    int nRank=0;
    int nMetadataHeight=0;
    std::string metadataNodeAddress = "NodeAddress";
    CService metadataService{};
};

class CInfinitynode : public infinitynode_info_t
{
private:
    // critical section to protect the inner data structures
    mutable CCriticalSection cs;
public:
    enum QtipArrayType {
        QTIPARRAYNODE_1 = 1, QTIPARRAYNODE_5 = 5, QTIPARRAYNODE_10 = 10, QTIPARRAYNODE_UNKNOWN = 0
    };

    CInfinitynode();
    CInfinitynode(const CInfinitynode& other);
    CInfinitynode(int nProtocolVersionIn, COutPoint outpointBurnFund);

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        LOCK(cs);
        READWRITE(vinBurnFund);
        READWRITE(sigTime);
        READWRITE(nProtocolVersion);
        READWRITE(nHeight);
        READWRITE(nExpireHeight);
        READWRITE(nLastRewardHeight);
        READWRITE(nNextRewardHeight);
        READWRITE(nBurnValue);
        READWRITE(nQTIPARRAYType);
        READWRITE(collateralAddress);
        READWRITE(scriptPubKey);
        READWRITE(backupAddress);
        READWRITE(nMetadataHeight);
        READWRITE(metadataNodeAddress);
        READWRITE(metadataService);
    }

    void setHeight(int nInHeight){nHeight = nInHeight; nExpireHeight=nInHeight + 720*365;}
    void setCollateralAddress(std::string address) { collateralAddress = address;}
    void setScriptPublicKey(CScript scriptpk){scriptPubKey = scriptpk;}
    void setBurnValue(CAmount burnFund){nBurnValue = burnFund;}
    void setQTIPARRAYType(int QTIPARRAYType){nQTIPARRAYType = QTIPARRAYType;}
    void setLastRewardHeight(int nReward){nLastRewardHeight = nReward;}
    void setRank(int nRankIn){nRank=nRankIn;}
    void setBackupAddress(std::string address) { backupAddress = address;}
    void setNodeAddress(std::string address) { metadataNodeAddress = address;}
    void setService(CService addrNew) { metadataService = addrNew;}
    void setMetadataHeight(int nHeight) { nMetadataHeight = nHeight;}

    infinitynode_info_t GetInfo();
    std::string getCollateralAddress(){return collateralAddress;}
    std::string getBackupAddress(){return backupAddress;}
    CScript getScriptPublicKey(){return scriptPubKey;}
    int getHeight(){return nHeight;}
    int getExpireHeight(){return nExpireHeight ;}
    int getRoundBurnValue(){CAmount nBurnAmount = nBurnValue / COIN + 1; return nBurnAmount;}
    int getQTIPARRAYType(){return nQTIPARRAYType;}
    int getLastRewardHeight(){return nLastRewardHeight;}
    int getRank(){return nRank;}
    int getMetadataHeight(){return nMetadataHeight;}

    CInfinitynode& operator=(CInfinitynode const& from)
    {
        static_cast<infinitynode_info_t&>(*this)=from;
        nHeight = from.nHeight;
        nExpireHeight = from.nExpireHeight;
        return *this;
    }
};

inline bool operator==(const CInfinitynode& a, const CInfinitynode& b)
{
    return a.vinBurnFund == b.vinBurnFund;
}
inline bool operator!=(const CInfinitynode& a, const CInfinitynode& b)
{
    return !(a.vinBurnFund == b.vinBurnFund);
}
#endif // QTIPARRAY_INFINITYNODE_H
