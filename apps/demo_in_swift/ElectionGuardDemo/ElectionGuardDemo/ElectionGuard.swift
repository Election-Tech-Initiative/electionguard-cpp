import Foundation

protocol ElectionObjectBase {
    var objectId: String { get }
}
protocol CryptoHash {
    func cryptoHash() -> ElementModQ
}

struct CiphertextBallot: ElectionObjectBase {
    var objectId: String
    let ballotStyle: String
    let descriptionHash: ElementModQ
    let previousTrackingHash: ElementModQ
    let contests: [CiphertextBallotContest]
    let trackingHash: ElementModQ?
    let timestamp: Int
    let cryptoHash: ElementModQ
    let nonce: ElementModQ?
}

struct PlaintextBallot: ElectionObjectBase {
    var objectId: String
    let ballotStyle: String
    let contests: [PlaintextBallotContest]
}

struct CiphertextBallotSelection: ElectionObjectBase {
    var objectId: String

    let descriptionHash: ElementModQ
    let ciphertext: ElgamalCiphertext
    let cryptoHash: ElementModQ
    let isPlaceholderSelection: Bool
    let nonce: ElementModQ
    let proof: DisjunctiveChaumPedersenProof?
}

struct PlaintextBallotSelection: ElectionObjectBase {
    var objectId: String
    let vote: UInt8
}

struct CiphertextBallotContest: ElectionObjectBase {
    var objectId: String
    let descriptionHash: ElementModQ
    let ballotSelections: [CiphertextBallotSelection]
    let cryptoHash: ElementModQ
    let nonce: ElementModQ?
    let proof: ConstantChaumPedersenProof?
}

struct PlaintextBallotContest: ElectionObjectBase {
    var objectId: String
    let ballotSelections: [PlaintextBallotSelection]
}

struct ConstantChaumPedersenProof {
    let pad: ElementModP
    let data: ElementModP
    let challenge: ElementModQ
    let response: ElementModQ
    let constant: Int
    let usage: ProofUsage = .selectionLimit
}

struct DisjunctiveChaumPedersenProof {
    let proofZeroPad: ElementModP
    let proofZeroData: ElementModP
    let proofOnePad: ElementModP
    let proofOneData: ElementModP
    let proofZeroChallenge: ElementModQ
    let proofOneChallenge: ElementModQ
    let challenge: ElementModQ
    let proofZeroResponse: ElementModQ
    let proofOneResponse: ElementModQ
    let usage: ProofUsage = .selectionValue
}

enum ProofUsage: String {
    case unknown = "Unknown"
    case secretValue = "Prove knowledge of secret value"
    case selectionLimit = "Prove value within selection's limit"
    case selectionValue = "Prove selection's value (0 or 1)"
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

struct ElgamalCiphertext: CryptoHash {
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
        let description = SelectionDescription()
        description.objectId = "some-selection-id"
        description.candidateId = candidateId
        description.sequenceOrder = 5
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
        let proof: DisjunctiveChaumPedersenProof? = nil

        let ciphertext_ = eg_encrypt_selection(plaintext_, description_, public_key_, extended_base_hash_, nonce_seed_, isPlaceholder, shouldVerifyProofs)
        
        let ciphertext_object_id = eg_ciphertext_ballot_selection_get_object_id(ciphertext_)!
        let descriptionHash_ = eg_ciphertext_ballot_selection_get_description_hash(ciphertext_)!
        
        let dh = getElementModQ(descriptionHash_)!
        let elgamal = getElgamalCiphertext()
        
        return CiphertextBallotSelection(objectId: String(cString: ciphertext_object_id),
                                         descriptionHash: dh, ciphertext: elgamal, cryptoHash: nonceSeed, isPlaceholderSelection: isPlaceholder, nonce: nonceSeed, proof: proof)

    }
    
    static func encryptContest(
            _ contest: PlaintextBallotContest, _ contestDescription: ContestDescriptionWithPlaceholders, _ elgamalPublicKey: ElementModP,
            _ extendedBaseHash: ElementModQ, _ nonceSeed: ElementModQ, _ shouldVerifyProofs: Bool = true) -> CiphertextBallotContest? {

        guard let ballotSelections = contestDescription.ballotSelections else {
            return nil
        }
        
        var encryptedSelections = [CiphertextBallotSelection]()
        
        for description in ballotSelections {
            var hasSelection = false
            var encryptedSelection: CiphertextBallotSelection?
            
            for selection in contest.ballotSelections {
                if selection.objectId == description.objectId {
                    hasSelection = true
                    encryptedSelection = encryptSelection(selection, description, elgamalPublicKey, extendedBaseHash, nonceSeed, false, shouldVerifyProofs)
                }
            }

            if !hasSelection {
                encryptedSelection = encryptSelection(description.toPlaintextBallotSelection(), description, elgamalPublicKey, extendedBaseHash, nonceSeed, false, shouldVerifyProofs)
            }
            
            guard encryptedSelection != nil else {
                return nil
            }
            
            encryptedSelections.append(encryptedSelection!)
        }
        
        // TODO: Handle placeholder selections

        let contestDescriptionHash = contestDescription.cryptoHash()!
        let cryptoHash = createCiphertextBallotContextCryptoHash(objectId: contest.objectId, ballotSelections: encryptedSelections, seedHash: contestDescriptionHash)
        let proof: ConstantChaumPedersenProof? = nil // TODO: Replace
        
        let encryptedContest = CiphertextBallotContest(objectId: contest.objectId, descriptionHash: contestDescriptionHash, ballotSelections: encryptedSelections, cryptoHash: cryptoHash, nonce: nonceSeed, proof: proof)
        
        if shouldVerifyProofs {
            // TODO: Verify the proof
        }
        
        return encryptedContest
    }
    
    static func encryptBallot(_ ballot: PlaintextBallot, electionManifest: ElectionDescription, context: CiphertextElectionContext, seedHash: ElementModQ, nonce: ElementModQ?, shouldVerifyProofs: Bool = true) -> CiphertextBallot? {
        let randomMasterNonce = nonce // TODO: Randomize
        let nonceSeed = createCiphertextBallotNonceSeed(descriptionHash: context.descriptionHashAsElementModQ(), objectId: ballot.objectId, nonce: randomMasterNonce!)
        
        var encryptedContests = [CiphertextBallotContest]()
        let contests = electionManifest.getContests(ballotStyleId: ballot.ballotStyle)
        
        for description in contests {
            var useContest: PlaintextBallotContest? = nil
            for contest in ballot.contests {
                if contest.objectId == description.objectId {
                    useContest = contest
                    break
                }
            }
            
            if useContest == nil {
                useContest = description.toPlaintextBallotContest()
            }
                        
            guard let encryptedContest = encryptContest(useContest!, description as! ContestDescriptionWithPlaceholders, context.elgamalPublicKeyAsElementModP(), context.cryptoExtendedBaseHashAsElementModQ(), nonceSeed) else {
                return nil
            }
            
            encryptedContests.append(encryptedContest)
        }
        
        let cryptoHash = createCiphertextBallotCryptoHash(objectId: ballot.objectId, descriptionHash: context.descriptionHashAsElementModQ(), contests: encryptedContests)
        let timestamp = Int(Date().timeIntervalSince(Date.distantPast) * 10000000)
        
        let encryptedBallot = CiphertextBallot(objectId: ballot.objectId, ballotStyle: ballot.ballotStyle, descriptionHash: context.descriptionHashAsElementModQ(), previousTrackingHash: seedHash, contests: encryptedContests, trackingHash: nil, timestamp: timestamp, cryptoHash: cryptoHash, nonce: randomMasterNonce)
        
        if shouldVerifyProofs {
            // TODO: Verify the proof
        }
        
        return encryptedBallot
    }
    
    static func hash(_ data: String) -> ElementModQ? {
        guard let pointer = eg_hash_elems_string(data) else {
            return nil
        }
        
        return getElementModQ(pointer);
    }
    
    static func hash(_ data: [String]) -> ElementModQ? {
        // Convert swift [String] to c char **
        var c_data = data.map { UnsafePointer<Int8>(strdup($0)) }
        
        guard let pointer = eg_hash_elems_strings(&c_data, Int32(data.count)) else {
            return nil
        }
        
        return getElementModQ(pointer);
    }
    
    static func hash(_ data: UInt64) -> ElementModQ? {
        guard let pointer = eg_hash_elems_int(data) else {
            return nil
        }
        
        return getElementModQ(pointer);
    }
    
    static func createCiphertextBallotContextCryptoHash(objectId: String, ballotSelections: [CiphertextBallotSelection], seedHash: ElementModQ) -> ElementModQ {
        // TODO: Implement
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }

    static func createCiphertextBallotCryptoHash(objectId: String, descriptionHash: ElementModQ, contests: [CiphertextBallotContest]) -> ElementModQ {
        // TODO: Implement (using hash of all incoming args)
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }
    
    static func createCiphertextBallotNonceSeed(descriptionHash: ElementModQ, objectId: String, nonce: ElementModQ) -> ElementModQ {
        // TODO: Implement (using hash of all incoming args)
        return ElementModQ([UInt64](repeating: 0, count: ElementModQ.MAX_SIZE))
    }

    static func setSelectionDescription(_ description: SelectionDescription) -> OpaquePointer? {
        return eg_selection_description_new(description.objectId, description.candidateId, description.sequenceOrder!)
    }

    static func getSelectionDescription(_ description: OpaquePointer) -> SelectionDescription? {
        guard let object_id = eg_selection_description_get_object_id(description) else { return nil }
        guard let candidate_id = eg_selection_description_get_candidate_id(description) else { return nil }
        let sequence_order = eg_selection_description_get_sequence_order(description)
        let swift_type = SelectionDescription()
        swift_type.objectId = String(cString: object_id)
        swift_type.candidateId = String(cString: candidate_id)
        swift_type.sequenceOrder = UInt64(sequence_order)
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
    
    static func getElgamalCiphertext() -> ElgamalCiphertext {
        let fake_pad = ElementModP([UInt64](repeating: 0, count: ElementModP.MAX_SIZE))
        let fake_data = ElementModP([UInt64](repeating: 0, count: ElementModP.MAX_SIZE))
        
        return ElgamalCiphertext(pad: fake_pad, data: fake_data)
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
