#ifndef __ELECTIONGUARD_CPP_TOOLS_GENERATORS_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TOOLS_GENERATORS_ELECTION_HPP_INCLUDED__

#include <electionguard/election.hpp>
#include <electionguard/export.h>
#include <electionguard/manifest.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace electionguard;
using namespace std;

namespace electionguard::tools::generators
{
    class ElectionGenerator
    {
      public:
        static unique_ptr<CiphertextElectionContext>
        getFakeContext(const InternalManifest &manifest, const ElementModP &publicKey)
        {
            auto context =
              CiphertextElectionContext::make(1UL, 1UL, publicKey.clone(), TWO_MOD_Q().clone(),
                                              manifest.getManifestHash()->clone());
            return context;
        }

        static unique_ptr<CiphertextElectionContext> getJsonContext()
        {
            string contextData =
              "{\"commitment_hash\":\"02\",\"crypto_base_hash\":"
              "\"4872F08189255656301A22A60AAA64C1034EFB6B7B2055EC945E859CBB3A41A4\",\"crypto_"
              "extended_base_hash\":"
              "\"AA022F67A9C51369E57400DF25B8303F02DA5A38D04846423753C223B6B0E95B\",\"elgamal_"
              "public_key\":"
              "\"CB9B95A3B2BB20A189F71117D74B88F39E52514EF5EADCD4C0D41219D36CDD68F9E06C049F354A6CCB"
              "508B100B90B7A268D1C1EA5CE075A6B3DE58E5209AB80512C41F6C3C3170C70D2119198E679668277EED"
              "1361D2A378530892A05364D811D0FD97D1EEF4A354F9199C48EF4E35452DB557CCBD01CF0CC5EC5B0AB1"
              "16D03E9385E42A299A947BC137697E6CEB70837FDA8A20D5AFB53517B6378BF0AE8003F1C052D976236C"
              "1CC6BC07BA8ED340E469249E3875ADB7E5B081A21A3E02B7C04E004D4C31923F3641428D894D06B66A60"
              "C13337FFEAE29850E2FF3D00032799B17DD79861F3827B5F8DA0070349578655560CD21B0355E862BCFE"
              "D16B0A9B5C8F8C13F4641EBAE161E9DDB81C04A31215A8473F4926ACA2A9604A61394469815EDEF5F464"
              "43F1FE7B8A1760EEEF72E3B70FDF80EDA92C566343F04BF48C74179CE1B4A5A900F1E569A11118650C72"
              "66303C25CAF3891B4070BBA2D6F4E1C7A584241ABCAA1244245CBCC76130E293E66A54F7B0BB3F632886"
              "13D66B8CF3313AFCB1DA99BD35CCDE00D3128CC02003E9907F8F5C8672CDF50880E37820F0BF68D38E2A"
              "B8BB2AFDD3292B897689DC6CC923DC902782681486B25172406A012103FB4C5636439E9C1C7B11482A52"
              "D11F9FC0639EE7DABED8B37CA6B3E9B0F4472B67347E05444635C0BFA82C5ABA5CA7F78569A81F7A4766"
              "6313A598D92C2F6342\",\"manifest_hash\":"
              "\"851E8E570EA8823734530341842DD3293D2C552013F73B4DFF8AF2E371396E6D\",\"number_of_"
              "guardians\":1,\"quorum\":1}";

            auto context = CiphertextElectionContext::fromJson(contextData);
            return context;
        }
    };
} // namespace electionguard::tools::generators

#endif /* __ELECTIONGUARD_CPP_TOOLS_GENERATORS_ELECTION_HPP_INCLUDED__ */
