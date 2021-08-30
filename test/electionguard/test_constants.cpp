#include "../../src/electionguard/convert.hpp"
#include "../../src/electionguard/facades/Hacl_Bignum256.hpp"
#include "../../src/electionguard/facades/Hacl_Bignum4096.hpp"
#include "../../src/electionguard/log.hpp"
#include "../../src/electionguard/utils.hpp"
#include "utils/byte_logger.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>

using namespace std;
using namespace electionguard;
using namespace hacl;

// This test prints out the standard constants that are loaded into constants.h
// if you wish to use different primes then add a test like this one to calculate
// the constant values and add a conditional override in constants.h
TEST_CASE("Print Standard Constants")
{
    Log::debug("\n-------- Print Standard Constants ---------\n");
    Log::debug("Endianness of your system:");
    if (is_little_endian()) {
        Log::debug("Little");
    } else {
        Log::debug("Big");
    }

    // Small Prime

    Log::debug("\n----- small_prime Q -----\n");
    auto q_hex = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF43";
    Log::debug(q_hex);

    auto q_bytes = hex_to_bytes(q_hex);
    ByteLogger::print("bytes:", q_bytes);

    // copy the byte array to a bignum representation
    Log::debug("uint64_t array");
    auto q_bignum = copy_bytes_to_bignum_64(q_bytes, MAX_Q_LEN);
    ByteLogger::print("", q_bignum);

    // hacl expects constants to be laid out this way
    Log::debug("uint64_t array (inverted hacl format)");
    auto q_hacl = ElementModQ::fromHex(q_hex, true);
    ByteLogger::print("", q_hacl->get(), MAX_Q_LEN);

    // 2^256 - Q + 1
    // this is specific to the limb offset in hacl
    uint64_t q_inverse_offset[MAX_Q_LEN_DOUBLE] = {};
    auto q_carry = Bignum256::sub(const_cast<uint64_t *>(MAX_256), q_hacl->get(), q_inverse_offset);
    q_carry = Bignum256::add(static_cast<uint64_t *>(q_inverse_offset),
                             const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                             static_cast<uint64_t *>(q_inverse_offset));
    ByteLogger::print("hex inverse offset:", q_inverse_offset, MAX_Q_LEN);

    // Large Prime

    Log::debug("\n----- large_prime P -----\n");
    auto p_hex =
      "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF93C467E37DB0C7A4D1BE3F810152"
      "CB56A1CECC3AF65CC0190C03DF34709AFFBD8E4B59FA03A9F0EED0649CCB621057D11056AE9132135A08E43B4673"
      "D74BAFEA58DEB878CC86D733DBE7BF38154B36CF8A96D1567899AAAE0C09D4C8B6B7B86FD2A1EA1DE62FF8643EC7"
      "C271827977225E6AC2F0BD61C746961542A3CE3BEA5DB54FE70E63E6D09F8FC28658E80567A47CFDE60EE741E5D8"
      "5A7BD46931CED8220365594964B839896FCAABCCC9B31959C083F22AD3EE591C32FAB2C7448F2A057DB2DB49EE52"
      "E0182741E53865F004CC8E704B7C5C40BF304C4D8C4F13EDF6047C555302D2238D8CE11DF2424F1B66C2C5D238D0"
      "744DB679AF2890487031F9C0AEA1C4BB6FE9554EE528FDF1B05E5B256223B2F09215F3719F9C7CCC69DDF172D0D6"
      "234217FCC0037F18B93EF5389130B7A661E5C26E54214068BBCAFEA32A67818BD3075AD1F5C7E9CC3D1737FB2817"
      "1BAF84DBB6612B7881C1A48E439CD03A92BF52225A2B38E6542E9F722BCE15A381B5753EA842763381CCAE83512B"
      "30511B32E5E8D80362149AD030AABA5F3A5798BB22AA7EC1B6D0F17903F4E22D840734AA85973F79A93FFB82A75C"
      "47C03D43D2F9CA02D03199BACEDDD4533A52566AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
      "FFFFFFFFFFFF";
    Log::debug(p_hex);

    auto p_bytes = hex_to_bytes(p_hex);
    ByteLogger::print("bytes:", p_bytes);
    Log::debug("bytes_size:", p_bytes.size());

    Log::debug("uint64_t array");
    auto p_bignum = copy_bytes_to_bignum_64(p_bytes, MAX_P_LEN);
    ByteLogger::print("", p_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto p_hacl = ElementModP::fromHex(p_hex, true);
    ByteLogger::print("", p_hacl->get(), MAX_P_LEN);

    uint64_t p_inverse_offset[MAX_P_LEN_DOUBLE] = {};
    auto p_carry =
      Bignum4096::sub(const_cast<uint64_t *>(MAX_4096), p_hacl->get(), p_inverse_offset);
    p_carry = Bignum4096::add(static_cast<uint64_t *>(p_inverse_offset),
                              const_cast<uint64_t *>(ONE_MOD_P_ARRAY),
                              static_cast<uint64_t *>(p_inverse_offset));
    ByteLogger::print("hex inverse offset:", p_inverse_offset, MAX_P_LEN);

    // Cofactor

    Log::debug("\n----- cofactor R -----\n");
    auto r_hex =
      "0100000000000000000000000000000000000000000000000000000000000000BC93C467E37DB0C7A4D1BE3F8101"
      "52CB56A1CECC3AF65CC0190C03DF34709B8AF6A64C0CEDCF2D559DA9D97F095C3076C686037619148D2C86C31710"
      "2AFA2148031F04440AC0FF0C9A417A89212512E7607B2501DAA4D38A2C1410C4836149E2BDB8C8260E627C464696"
      "3EFFE9E16E495D48BD215C6D8EC9D1667657A2A1C8506F2113FFAD19A6B2BC7C45760456719183309F874BC9ACE5"
      "70FFDA877AA2B23A2D6F291C1554CA2EB12F12CD009B8B8734A64AD51EB893BD891750B85162241D908F0C970987"
      "9758E7E8233EAB3BF2D6AB53AFA32AA153AD6682E5A0648897C9BE18A0D50BECE030C3432336AD9163E33F8E7DAF"
      "498F14BB2852AFFA814841EB18DD5F0E89516D557776285C16071D211194EE1C3F34642036AB886E3EC28882CE40"
      "03DEA335B4D935BAE4B58235B9FB2BAB713C8F705A1C7DE42220209D6BBCACC467318601565272E4A63E38E24997"
      "54AE493AC1A8E83469EEF35CA27C271BC792EEE21156E617B922EA8F713C22CF282DC5D6385BB12868EB781278FA"
      "0AB2A8958FCCB5FFE2E5C361FC174420122B0163CA4A46308C8C46C91EA7457C136A7D9FD4A7F529FD4A7F529FD4"
      "A7F529FD4A7F529FD4A7F529FD4A7F529FD4A7F52A";
    Log::debug(r_hex);

    auto sanitized = sanitize_hex_string(r_hex);
    auto r_bytes = hex_to_bytes(sanitized);
    ByteLogger::print("bytes:", r_bytes);

    Log::debug("uint64_t array");
    // TODO: use Bignum4096/256 fo this operation and delete this method
    auto r_bignum = copy_bytes_to_bignum_64(r_bytes, MAX_P_LEN);
    ByteLogger::print("", r_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto r_hacl = ElementModP::fromHex(r_hex, true);
    ByteLogger::print("", r_hacl->get(), MAX_P_LEN);

    // Generator

    Log::debug("\n----- generator G -----\n");
    auto g_hex =
      "1D41E49C477E15EAEEF0C5E4AC08D4A46C268CD3424FC01D13769BDB43673218587BC86C4C1448D006A03699F3AB"
      "AE5FEB19E296F5D143CC5E4A3FC89088C9F4523D166EE3AE9D5FB03C0BDD77ADD5C017F6C55E2EC92C226FEF5C6C"
      "1DF2E7C36D90E7EAADE098241D3409983BCCD2B5379E9391FBC62F9F8D939D1208B160367C134264122189595EC8"
      "5C8CDBE5F9D307F46912C04932F8C16815A76B4682BD6BDC0ED52B00D8D30F59C731D5A7FFAE8165D53CF96649AA"
      "C2B743DA56F14F19DACC5236F29B1AB9F9BEFC69697293D5DEAD8B5BF5DE9BAB6DE67C45719E56344A3CBDF36098"
      "24B1B578E34EAEB6DD3190AB3571D6D671C512282C1DA7BD36B4251D2584FADEA80B9E141423074DD9B5FB83ACBD"
      "EAD4C87A58FFF517F977A83080370A3B0CF98A1BC2978C47AAC29611FD6C40E2F9875C35D50443A9AA3F49611DCD"
      "3A0D6FF3CB3FACF31471BDB61860B92C594D4E46569BB39FEEADFF1FD64C836A6D6DB85C6BA7241766B7AB56BF73"
      "9633B054147F7170921412E948D9E47402D15BB1C257318612C121C36B80EB8433C08E7D0B7149E3AB0A8735A92E"
      "DCE8FF943E28A2DCEACFCC69EC318909CB047BE1C5858844B5AD44F22EEB289E4CC554F7A5E2F3DEA026877FF928"
      "51816071CE028EB868D965CCB2D2295A8C55BD1C070B39B09AE06B37D29343B9D8997DC244C468B980970731736E"
      "E018BBADB987";
    Log::debug(g_hex);

    auto g_bytes = hex_to_bytes(g_hex);
    ByteLogger::print("bytes:", g_bytes);

    Log::debug("uint64_t array");
    auto g_bignum = copy_bytes_to_bignum_64(g_bytes, MAX_P_LEN);
    ByteLogger::print("", g_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto g_hacl = ElementModP::fromHex(g_hex, true);
    ByteLogger::print("", g_hacl->get(), MAX_P_LEN);
}

TEST_CASE("Print Test Constants")
{
    Log::debug("\n-------- Print TEST Constants ---------\n");
    Log::debug("Endianness of your system:");
    if (is_little_endian()) {
        Log::debug("Little");
    } else {
        Log::debug("Big");
    }

    // Small Prime

    Log::debug("\n----- small_prime Q -----\n");
    auto q_hex = "FFF1";
    Log::debug(q_hex);

    auto q_bytes = hex_to_bytes(q_hex);
    ByteLogger::print("bytes:", q_bytes);

    Log::debug("uint64_t array");
    auto q_bignum = copy_bytes_to_bignum_64(q_bytes, MAX_Q_LEN);
    ByteLogger::print("", q_bignum);

    Log::debug("uint64_t array (inverted hacl format)");
    auto q_hacl = ElementModQ::fromHex(q_hex, true);
    ByteLogger::print("", q_hacl->get(), MAX_Q_LEN);

    // 2^256 - Q + 1
    uint64_t q_inverse_offset[MAX_Q_LEN_DOUBLE] = {};
    auto q_carry = Bignum256::sub(const_cast<uint64_t *>(MAX_256), q_hacl->get(), q_inverse_offset);
    q_carry = Bignum256::add(static_cast<uint64_t *>(q_inverse_offset),
                             const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                             static_cast<uint64_t *>(q_inverse_offset));
    ByteLogger::print("hex inverse offset:", q_inverse_offset, MAX_Q_LEN);

    // Large Prime

    Log::debug("\n----- large_prime P -----\n");
    auto p_hex = "FFFFFFFFFFB43EA5";
    Log::debug(p_hex);

    auto p_bytes = hex_to_bytes(p_hex);
    ByteLogger::print("bytes:", p_bytes);

    Log::debug("uint64_t array");
    auto p_bignum = copy_bytes_to_bignum_64(p_bytes, MAX_P_LEN);
    ByteLogger::print("", p_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto p_hacl = ElementModP::fromHex(p_hex, true);
    ByteLogger::print("", p_hacl->get(), MAX_P_LEN);

    uint64_t p_inverse_offset[MAX_P_LEN_DOUBLE] = {};
    auto p_carry =
      Bignum4096::sub(const_cast<uint64_t *>(MAX_4096), p_hacl->get(), p_inverse_offset);
    p_carry = Bignum4096::add(static_cast<uint64_t *>(p_inverse_offset),
                              const_cast<uint64_t *>(ONE_MOD_P_ARRAY),
                              static_cast<uint64_t *>(p_inverse_offset));
    ByteLogger::print("hex inverse offset:", p_inverse_offset, MAX_P_LEN);

    // Cofactor

    Log::debug("\n----- cofactor R -----\n");
    auto r_hex = "01000F00E10CE4";
    Log::debug(r_hex);

    auto r_bytes = hex_to_bytes(r_hex);
    ByteLogger::print("bytes:", r_bytes);

    Log::debug("uint64_t array");
    auto r_bignum = copy_bytes_to_bignum_64(r_bytes, MAX_P_LEN);
    ByteLogger::print("", r_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto r_hacl = ElementModP::fromHex(r_hex, true);
    ByteLogger::print("", r_hacl->get(), MAX_P_LEN);

    // Generator

    Log::debug("\n----- generator G -----\n");
    auto g_hex = "D6982759F3D5107E";
    Log::debug(g_hex);

    auto g_bytes = hex_to_bytes(g_hex);
    ByteLogger::print("bytes:", g_bytes);

    Log::debug("uint64_t array");
    auto g_bignum = copy_bytes_to_bignum_64(g_bytes, MAX_P_LEN);
    ByteLogger::print("", g_bignum);

    Log::debug("uint64_t array (hacl inverted format)");
    auto g_hacl = ElementModP::fromHex(g_hex, true);
    ByteLogger::print("", g_hacl->get(), MAX_P_LEN);
}