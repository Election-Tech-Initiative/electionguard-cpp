//
//  CiphertextElectionContext.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 9/1/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct CiphertextElectionContext: Codable {
    let numberOfGuardians: Int?
    let quorum: Int?
    let elgamalPublicKey: String?
    let descriptionHash: String?
    let cryptoBaseHash: String?
    let cryptoExtendedBaseHash: String?
    
    enum CodingKeys: String, CodingKey {
        case numberOfGuardians = "number_of_guardians"
        case quorum
        case elgamalPublicKey = "elgamal_public_key"
        case descriptionHash = "description_hash"
        case cryptoBaseHash = "crypto_base_hash"
        case cryptoExtendedBaseHash = "crypto_extended_base_hash"
    }
    
    func elgamalPublicKeyAsElementModP() -> ElementModP {
        // TODO: Implement
        return ElementModP([UInt64](repeating: 0, count: ElementModP.MAX_SIZE))
    }
    
    func descriptionHashAsElementModQ() -> ElementModQ {
        // TODO: Implement
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }
    
    func cryptoBaseHashElementModQ() -> ElementModQ {
        // TODO: Implement
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }
    
    func cryptoExtendedBaseHashAsElementModQ() -> ElementModQ {
        // TODO: Implement
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }
}
