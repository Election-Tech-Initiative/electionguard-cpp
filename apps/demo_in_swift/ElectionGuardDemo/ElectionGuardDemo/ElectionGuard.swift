import Foundation

protocol ElectionObjectBase {
    var objectId: String { get }
}
protocol CryptoHash {
    func cryptoHash() -> ElementModQ
}

struct CiphertextBallotSelection: ElectionObjectBase {
    var objectId: String

    let descriptionHash: ElementModQ
    let ciphertext: ElgamalCiphetext
    let cryptoHash: ElementModQ
    let isPlaceholderSelection: Bool
    let nonce: ElementModQ
}

struct PlaintextBallotSelection: ElectionObjectBase {
    var objectId: String
    let vote: UInt8
}

struct ElementModP {
    var data: [UInt64] = [UInt64](repeating: 0, count: ElementModP.MAX_SIZE)
    static let MAX_SIZE = 64

    init(_ data: [UInt64]) {
        // TODO: better safety
        if data.count <= ElementModP.MAX_SIZE {
            self.data = data
        }
    }
}

struct ElementModQ {
    var data: [UInt64] = [UInt64](repeating: 0, count: ElementModQ.MAX_SIZE)
    static let MAX_SIZE = 4

    init(_ data: [UInt64]) {
        // TODO: better safety
        if data.count <= ElementModQ.MAX_SIZE {
            self.data = data
        }
    }
}

struct ElgamalCiphetext: CryptoHash {
    var pad: ElementModP
    var data: ElementModP
    
    func cryptoHash() -> ElementModQ {
        return ElementModQ([UInt64](repeating: 33, count: ElementModQ.MAX_SIZE))
    }
}

class ElectionGuard {

    static func mainTest() -> Bool {
        
        assert(testEncryptSelection() == true, "Encrypting the selection failed")
        assert(testGenerateTrackingCode() == true, "GEnerating the tracking code failed")
        return true;

    }
    
    static func testEncryptSelection() -> Bool
    {
        let candidateId = "some-candidate-id"
        let plaintext = PlaintextBallotSelection(objectId: candidateId, vote: 1)
        let description = SelectionDescription(objectId: "some-selection-id", candidateId: candidateId, sequenceOrder: 5)
        let publicKey = ElementModP([UInt64](repeating: 255, count: ElementModP.MAX_SIZE))
        let extendedHash = ElementModQ([UInt64](repeating: 162, count: ElementModQ.MAX_SIZE))
        let nonceSeed = getSecureRandomElementModQ() // just a stubbed method to simulate a secure value for now

        guard let ciphertext = encryptSelection(plaintext, description, publicKey, extendedHash, nonceSeed, false, true) else {
            print("encryption FAILED!")
            return false
        }

        assert(ciphertext.objectId == plaintext.objectId, "Object Ids do not match")
        assert(ciphertext.objectId == candidateId, "Candidate id does not match")
        // the current test just arbitrarily assigns the vote to the hash
        assert(ciphertext.descriptionHash.data[0] == 1, "the fake hash doesnt match the vote")
        
        print("success!")
        
        return true
        
    }
    
    static func testGenerateTrackingCode() -> Bool {
        
        let deviceHash = getHashForDevice(1234, "some-location-string")!
        let firstBallotHash = ElementModQ([UInt64](repeating: 255, count: ElementModQ.MAX_SIZE))
        let secondBallotHash = ElementModQ([UInt64](repeating: 162, count: ElementModQ.MAX_SIZE))
        
        let rotatingHash1 = getRotatingTrackerHash(deviceHash, 1000, firstBallotHash)!
        let rotatingHash2 = getRotatingTrackerHash(rotatingHash1, 1001, secondBallotHash)!
        
        assert(rotatingHash1.data != deviceHash.data)
        assert(rotatingHash2.data != deviceHash.data)
        assert(rotatingHash1.data != rotatingHash2.data)
        
        return true
    }

    static func getSecureRandomElementModQ() -> ElementModQ {
        let allocation = [UInt64](repeating: 31, count: ElementModQ.MAX_SIZE)
        // TODO: generate secure random data
        return ElementModQ(allocation)
    }

    /// Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
    ///
    /// - parameter plaintext: the selection in the valid input form
    /// - parameter description: the `SelectionDescription` from the `ContestDescription`
    ///                          which defines this selection's structure
    /// - parameter elgamalPublicKey: the public key (K) used to encrypt the ballot
    /// - parameter extendedBaseHash: the extended base hash of the election
    /// - parameter nonceSeed: an `ElementModQ` used as a header to seed the `Nonce` generated
    ///                        for this selection. this value can be (or derived from) the
    ///                        BallotContest nonce, but no relationship is required
    /// - parameter isPlaceholder: specifies if this is a placeholder selection
    /// - parameter shouldVerifyProofs: specify if the proofs should be verified prior to returning (default True)
    /// - returns: A `CiphertextBallotSelection`
    static func encryptSelection(
            _ plaintext: PlaintextBallotSelection, _ description: SelectionDescription, _ elgamalPublicKey: ElementModP,
            _ extendedBaseHash: ElementModQ, _ nonceSeed: ElementModQ, _ isPlaceholder: Bool, _ shouldVerifyProofs: Bool) -> CiphertextBallotSelection? {
        let plaintext_ = setPlaintextBallotSelection(plaintext)
        let description_ = setSelectionDescription(description)

        let public_key_ = setElementModP(elgamalPublicKey)
        let extended_base_hash_ = setElementModQ(extendedBaseHash)
        let nonce_seed_ = setElementModQ(nonceSeed)

        let ciphertext_ = eg_encrypt_selection(plaintext_, description_, public_key_, extended_base_hash_, nonce_seed_, isPlaceholder, shouldVerifyProofs)
        
        let ciphertext_object_id = eg_ciphertext_ballot_selection_get_object_id(ciphertext_)!
        let descriptionHash_ = eg_ciphertext_ballot_selection_get_description_hash(ciphertext_)!
        
        let dh = getElementModQ(descriptionHash_)!
        let elgamal = getElgamalCiphertext()
        
        return CiphertextBallotSelection(objectId: String(cString: ciphertext_object_id),
            descriptionHash: dh, ciphertext: elgamal, cryptoHash: nonceSeed, isPlaceholderSelection: isPlaceholder, nonce: nonceSeed)

    }

    static func setSelectionDescription(_ description: SelectionDescription) -> OpaquePointer? {
        return eg_selection_description_new(description.objectId, description.candidateId, description.sequenceOrder!)
    }

    static func getSelectionDescription(_ description: OpaquePointer) -> SelectionDescription? {
        guard let object_id = eg_selection_description_get_object_id(description) else { return nil }
        guard let candidate_id = eg_selection_description_get_candidate_id(description) else { return nil }
        let sequence_order = eg_selection_description_get_sequence_order(description)

        let swift_type = SelectionDescription(objectId: String(cString: object_id), candidateId: String(cString: candidate_id), sequenceOrder: UInt64(sequence_order))
        eg_selection_description_free(description)
        return swift_type
    }

    static func setPlaintextBallotSelection(_ plaintext: PlaintextBallotSelection) -> OpaquePointer? {
        let as_str = ("\(plaintext.vote)" as NSString).utf8String
        return eg_plaintext_ballot_selection_new(plaintext.objectId, as_str)
    }

    static func setElementModP(_ element: ElementModP) -> OpaquePointer? {
        let ptr = UnsafeMutablePointer<UInt64>.allocate(capacity: ElementModP.MAX_SIZE)
        ptr.initialize(from: element.data, count: ElementModP.MAX_SIZE)
        return eg_element_mod_p_new(ptr)
    }

    static func setElementModQ(_ element: ElementModQ) -> OpaquePointer? {
        let ptr = UnsafeMutablePointer<UInt64>.allocate(capacity: ElementModQ.MAX_SIZE)
        ptr.initialize(from: element.data, count: ElementModQ.MAX_SIZE)
        return eg_element_mod_q_new(ptr)
    }
    
    static func getElementModP(_ opaquePointer: OpaquePointer) -> ElementModP? {
        var element: UnsafeMutablePointer<UInt64>?
        let dh_size = eg_element_mod_q_get(opaquePointer, &element)
        
        if dh_size != ElementModP.MAX_SIZE {
            print("wrong size")
        }
        
        var elementData = [UInt64](repeating: 0, count: ElementModP.MAX_SIZE)
        
        for i in 0..<ElementModP.MAX_SIZE {
            guard let value = element?[i] else {
                print("value was wrong")
                return nil
            }
            elementData[i] = value
        }
        
        return ElementModP(elementData)
        
    }
    
    static func getElementModQ(_ opaquePointer: OpaquePointer) -> ElementModQ? {
        var element: UnsafeMutablePointer<UInt64>?
        let dh_size = eg_element_mod_q_get(opaquePointer, &element)
        
        if dh_size != ElementModQ.MAX_SIZE {
            print("wrong size")
        }
        
        var elementData = [UInt64](repeating: 0, count: ElementModQ.MAX_SIZE)
        
        for i in 0..<ElementModQ.MAX_SIZE {
            guard let value = element?[i] else {
                print("value was wrong")
                return nil
            }
            elementData[i] = value
        }
        
        return ElementModQ(elementData)
        
    }
    
    static func getElgamalCiphertext() -> ElgamalCiphetext {
        let fake_pad = ElementModP([UInt64](repeating: 0, count: ElementModP.MAX_SIZE))
        let fake_data = ElementModP([UInt64](repeating: 0, count: ElementModP.MAX_SIZE))
        
        return ElgamalCiphetext(pad: fake_pad, data: fake_data)
    }

    static func getHashForDevice(_ uuid: UInt64, _ location: String) -> ElementModQ? {
        let ptr = eg_get_hash_for_device(uuid, location)!
        return getElementModQ(ptr)
    }

    static func getRotatingTrackerHash(_ previous: ElementModQ, _ timestamp: UInt64, _ ballotHash: ElementModQ) -> ElementModQ? {
        let previous_ptr = setElementModQ(previous)
        let ballot_ptr = setElementModQ(ballotHash)

        let rotating_hash_ptr = eg_get_rotating_tracker_hash(previous_ptr, timestamp, ballot_ptr)!
        eg_element_mod_q_free(previous_ptr)
        eg_element_mod_q_free(ballot_ptr)
        return getElementModQ(rotating_hash_ptr)
    }

    static func hashToWords(_ trackerHash: ElementModQ) -> String {
        let tracker_hash_ptr = setElementModQ(trackerHash)!
        let words = eg_hash_to_words_with_default_separator(tracker_hash_ptr)!
        eg_element_mod_q_free(tracker_hash_ptr)
        return String(cString: words)
    }

}
