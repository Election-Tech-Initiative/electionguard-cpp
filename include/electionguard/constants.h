#ifndef __ELECTIONGUARD_CORE_CONSTANTS_H_INCLUDED__
#define __ELECTIONGUARD_CORE_CONSTANTS_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>

enum MAX_IDENIFIER_LENGTH { MAX_IDENIFIER_LENGTH = 256 };

const uint8_t MAX_P_LEN = 64;
const uint8_t MAX_P_LEN_DOUBLE = 128;
const uint8_t MAX_Q_LEN = 4;
const uint8_t MAX_Q_LEN_DOUBLE = 8;

const uint32_t MAX_P_SIZE = MAX_P_LEN * sizeof(uint64_t);
const uint32_t MAX_Q_SIZE = MAX_Q_LEN * sizeof(uint64_t);

// Max P value in Hacl_Bignum4096 format
const uint64_t P_ARRAY[MAX_P_LEN] = {
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
  0xceddd45334dbc6b5, 0xd2f9ca02d03199ba, 0xfb82a75c47c03d43, 0x34aa85973f79a93f,
  0xf17903f4e234ea60, 0x98bb22aa7ec1b6d0, 0x9ad030aaba5f3a57, 0x1b32e5e8d8036214,
  0x81ccae83512b3051, 0x81b5753ea8427633, 0x542e9f722bce15a3, 0x92bf52225a2b38e6,
  0x81c1a48e439cd03a, 0x1baf84dbb6612b78, 0xe9cc3d1737fb2817, 0x818bd3075ad1f5c7,
  0x4068bbcafea32a67, 0xb7a661e5c26e5421, 0x7f18b93ef5389130, 0xd0d6234217fcc003,
  0x9f9c7ccc69ddf172, 0x6223b2f09215f371, 0xe528fdf1b05e5b25, 0xaea1c4bb6fe9554e,
  0xaf2890487031f9c0, 0xc5d238d0744db679, 0xe11df2424f1b66c2, 0x7c555302d2238d8c,
  0x4c4d8c4f13edf604, 0x8e704b7c5c40bf30, 0x2741e53865f004cc, 0x7db2db49ee52e018,
  0x32fab2c7448f2a05, 0xc083f22ad3ee591c, 0x6fcaabccc9b31959, 0x0365594964b83989,
  0x5a7bd46931ced822, 0x7cfde60ee741e5d8, 0x8fc28658e80567a4, 0xb54fe70e63e6d09f,
  0x961542a3ce3bea5d, 0x5e6ac2f0bd61c746, 0x3ec7c27182797722, 0xd2a1ea1de62ff864,
  0x0c09d4c8b6b7b86f, 0x8a96d1567899aaae, 0xdbe7bf38154b36cf, 0x58deb878cc86d733,
  0xe43b4673d74bafea, 0x1056ae9132135a08, 0xd0649ccb621057d1, 0x8e4b59fa03a9f0ee,
  0x0c03df34709affbd, 0xa1cecc3af65cc019, 0xd1be3f810152cb56, 0x93c467e37db0c7a4,
  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
};

// Max Q value in Hacl_Bignum256 format
const uint64_t Q_ARRAY[MAX_Q_LEN] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                     0xFFFFFFFFFFFFFFFF};

// G in Hacl_Bignum4096 format
const uint64_t G_ARRAY[MAX_P_LEN] = {
  0xb0c73abf03e8e0aa, 0x94b38ac417cddf40, 0x40d9992197d80a6e, 0x82dc54775ab83538,
  0x10c136f8691101ad, 0x8e61369ba0ddbadb, 0x00e8a317ae1e1d82, 0x288a89c931cf564f,
  0xdba8a12c5ca82757, 0xd4809ef34abeb83f, 0x76ebd2d95a3dee96, 0x8f299bcb29005200,
  0xe9f61c11a07bf421, 0xc3b1cbdb50f759d0, 0xbf7cd02d9661385a, 0x801328b2c6a60526,
  0xf6bc80662ba06207, 0xcaec94c95e96d628, 0x76c94b2008926b38, 0x93ad97b8fe09274e,
  0x09af5e71114a005f, 0xc0cdbdb76b6ad45f, 0x5a5d1cedeee01e4b, 0x0c7e71fcb2255254,
  0xbaad96aac96b0dbe, 0xc9b11ab6c59bce62, 0x72ceb313fdab069c, 0xeb276fb3685f3716,
  0xdf23f1db8355bde1, 0x501fa2342b728263, 0xfb1918b2a2a30e69, 0xf7f72da39a2284fd,
  0x8090c35da9355e6c, 0x322b37c50af18215, 0x86d24900742062c1, 0xf6932825bf45de83,
  0x7aaa0e9fd8785702, 0xd8e41422b64d2cba, 0x1e221c41f55bba31, 0xa6750e7c8750b514,
  0xf09d40ac20c2ab74, 0x872e62b5d1b13a98, 0xb3c39aa08b829b28, 0xcc3eda17dbfc47b8,
  0xfdebeef58570669a, 0x7f3dabcbad8eb9d8, 0xfa7b6c87f4554cb5, 0x26e050af50a03c65,
  0xd7f736fcba6c032e, 0x6b0e2a05be6af78f, 0xdd4b795f78de07d8, 0x4e65633480b396e1,
  0x25ef356fe5bb5931, 0xd3b308da86d1c3a5, 0x4a62dcbb93b1ddd8, 0xb706fa55d5038ffb,
  0x6387b98ee0e3def1, 0xcfb0cfb65363b256, 0x840ec7d4ec804794, 0xa142cdf33f6ef853,
  0x750d6798f5196cf2, 0xd45ec4cc64cfd15e, 0x8d2a3141825b33d5, 0x037de384f98f6e03,
};

const uint64_t ZERO_MOD_P_ARRAY[MAX_P_LEN] = {0UL};
const uint64_t ONE_MOD_P_ARRAY[MAX_P_LEN] = {1UL};
const uint64_t TWO_MOD_P_ARRAY[MAX_P_LEN] = {2UL};

const uint64_t ZERO_MOD_Q_ARRAY[MAX_Q_LEN] = {0UL, 0UL, 0UL, 0UL};
const uint64_t ONE_MOD_Q_ARRAY[MAX_Q_LEN] = {1UL, 0UL, 0UL, 0UL};
const uint64_t TWO_MOD_Q_ARRAY[MAX_Q_LEN] = {2UL, 0UL, 0UL, 0UL};

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_CONSTANTS_H_INCLUDED__ */