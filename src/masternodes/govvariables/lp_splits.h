// Copyright (c) 2020 The DeFi Foundation
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#ifndef DEFI_MASTERNODES_GOVVARIABLES_LP_SPLITS_H
#define DEFI_MASTERNODES_GOVVARIABLES_LP_SPLITS_H

#include <masternodes/gv.h>
#include <amount.h>

class LP_SPLITS : public GovVariable, public AutoRegistrator<GovVariable, LP_SPLITS>
{
public:
    Res Import(UniValue const &val) override;
    UniValue Export() const override;
    Res Validate(CCustomCSView const &mnview) const override;
    Res Apply(CCustomCSView &mnview, uint32_t height) override;

    std::string GetName() const override { return TypeName(); }
    static constexpr char const * TypeName() { return "LP_SPLITS"; }
    static GovVariable * Create() { return new LP_SPLITS(); }

    ADD_OVERRIDE_VECTOR_SERIALIZE_METHODS
    ADD_OVERRIDE_SERIALIZE_METHODS(CDataStream)

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        static_assert(std::is_same<decltype(splits), std::map<DCT_ID, CAmount>>::value, "Following code is invalid");
        std::map<uint32_t, CAmount> serliazedSplits;
        if (ser_action.ForRead()) {
            READWRITE(serliazedSplits);
            splits.clear();
            for (auto it = serliazedSplits.begin(); it != serliazedSplits.end(); /* advance */) {
                splits.emplace(DCT_ID{it->first}, it->second);
                serliazedSplits.erase(it++);
            }
        } else {
            for (const auto& it : splits) {
                serliazedSplits.emplace(it.first.v, it.second);
            }
            READWRITE(serliazedSplits);
        }
    }

    std::map<DCT_ID, CAmount> splits;
};

#endif // DEFI_MASTERNODES_GOVVARIABLES_LP_SPLITS_H
