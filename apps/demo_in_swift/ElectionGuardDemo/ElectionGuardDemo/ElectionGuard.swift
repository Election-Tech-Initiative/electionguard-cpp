//
//  ElectionGuard.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/20/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct ElectionGuard {
    static func encryptSelection(objectId: String, vote: String) -> Bool {
        // instantiate a selection on a ballot
        let plaintextSelection = eg_plaintext_ballot_selection_new(
            objectId, UnsafeMutablePointer<Int8>(mutating: (vote as NSString).utf8String)
        )
        
        var ciphertextSelection: OpaquePointer?
        let encryptResult = eg_encrypt_selection(plaintextSelection, &ciphertextSelection)
        
        // arbitrarily returns one for success
        assert(encryptResult == 1)
        
        // check the object id by accessing the property getter
        let plaintextObjectIdPtr = eg_plaintext_ballot_selection_get_object_id(plaintextSelection)
        let ciphertextObjectIdPtr = eg_ciphertext_ballot_selection_get_object_id(ciphertextSelection)
        
        assert(String(cString: ciphertextObjectIdPtr!) == String(cString: plaintextObjectIdPtr!), "Ciphertext object ID not equal to original `objectId` value")
        print(String(cString: ciphertextObjectIdPtr!))
        
        // get out one of the ElementModQ hashes
        let descriptionHash = eg_ciphertext_ballot_selection_get_description_hash(ciphertextSelection)
        var descriptionHashData: UnsafeMutablePointer<UInt64>?
        let size = eg_element_mod_q_get(descriptionHash, &descriptionHashData)
        
        assert(size == 4, "Size not 4")
        // the current test just arbitrarily assigns the vote to the hash
        assert(descriptionHashData?[0] == UInt64(vote), "Description hash not equal to original `vote` value")
        print(descriptionHashData?[0] ?? "")
        
        eg_plaintext_ballot_selection_free(plaintextSelection)
        eg_ciphertext_ballot_selection_free(ciphertextSelection)
        
        return true
    }
}
