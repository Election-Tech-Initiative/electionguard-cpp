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

struct SelectionDescription: ElectionObjectBase {
    var objectId: String
    let candidateId: String
    let sequenceOrder: UInt64
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
        let plaintext = PlaintextBallotSelection(objectId: "some-object-id", vote: 1)
        let description = SelectionDescription(objectId: "some-selection-id", candidateId: "some-candidate-id", sequenceOrder: 5)
        let publicKey = ElementModP([UInt64](repeating: 255, count: ElementModP.MAX_SIZE))
        let extendedHash = ElementModQ([UInt64](repeating: 162, count: ElementModQ.MAX_SIZE))
        let nonceSeed = getSecureRandomElementModQ()

        guard let ciphertext = encryptSelection(plaintext, description, publicKey, extendedHash, nonceSeed, false, true) else {
            print("encryption FAILED!")
            return false
        }

        assert(ciphertext.objectId == plaintext.objectId, "Object Ids do not match")
        
        return true

    }

    static func getSecureRandomElementModQ() -> ElementModQ {
        let allocation = [UInt64](repeating: 31, count: ElementModQ.MAX_SIZE)
        // TODO: generate secure random data
        return ElementModQ(allocation)
    }


    static func encryptSelection(
            _ plaintext: PlaintextBallotSelection, _ description: SelectionDescription, _ elgamalPublicKey: ElementModP,
            _ extendedBaseHash: ElementModQ, _ nonceSeed: ElementModQ, _ isPlaceholder: Bool, _ shouldVerifyProofs: Bool) -> CiphertextBallotSelection? {
        let plaintext_ = getPlaintextBallotSelection(plaintext)
        let description_ = setSelectionDescription(description)

        let public_key_ = getElementModP(elgamalPublicKey)
        let extended_base_hash_ = getElementModQ(extendedBaseHash)
        let nonce_seed_ = getElementModQ(nonceSeed)

        let ciphertext_ = eg_encrypt_selection(plaintext_?.pointer, description_, public_key_, extended_base_hash_, nonce_seed_, isPlaceholder, shouldVerifyProofs)
        
        let ciphertext_object_id = eg_ciphertext_ballot_selection_get_object_id(ciphertext_)
        let descriptionHash_ = eg_ciphertext_ballot_selection_get_description_hash(ciphertext_)
        
        var descriptionHashData = [UInt64](repeating: 0, count: ElementModQ.MAX_SIZE)
        
        
        var hashDataptr = UnsafeMutablePointer<UInt64>.allocate(capacity: ElementModQ.MAX_SIZE)
        hashDataptr.moveInitialize(from: &descriptionHashData, count:ElementModQ.MAX_SIZE)
        let dh_size = eg_element_mod_q_get(descriptionHash_, hashDataptr)
        
        let dh = ElementModQ(
        
        return CiphertextBallotSelection(objectId: StringString(cString: ciphertext_object_id), descriptionHash: <#T##ElementModQ#>, ciphertext: <#T##ElgamalCiphetext#>, cryptoHash: <#T##ElementModQ#>, isPlaceholderSelection: <#T##Bool#>, nonce: <#T##ElementModQ#>)

    }

    static func setSelectionDescription(_ description: SelectionDescription) -> OpaquePointer? {
        //UnsafeMutablePointer<eg_selection_description_t>
        eg_selection_description_new(description.objectId, description.candidateId, description.sequenceOrder)
    }

    static func getSelectionDescription(_ description: OpaquePointer) -> SelectionDescription? {
        // one possible pattern for accessing objects
        //UnsafeMutablePointer<eg_selection_description_t>
        guard let object_id = eg_selection_description_get_object_id(description) else { return nil }
        guard let candidate_id = eg_selection_description_get_candidate_id(description) else { return nil }
        let sequence_order = eg_selection_description_get_sequence_order(description)

        let swift_type = SelectionDescription(objectId: String(cString: object_id), candidateId: String(cString: candidate_id), sequenceOrder: UInt64(sequence_order))
        eg_selection_description_free(description)
        return swift_type
    }

    static func getPlaintextBallotSelection(_ plaintext: PlaintextBallotSelection) -> GCHandle? {
        guard let c_type = eg_plaintext_ballot_selection_new(plaintext.objectId, UnsafeMutablePointer<Int8>(mutating: ("\(plaintext.vote)" as NSString).utf8String)) else {
            return nil
        }
        return GCHandle(pointer: c_type)
    }

    static func getElementModP(_ element: ElementModP) -> OpaquePointer? {
        // another is use opaque pointers to raw memory?
        eg_element_mod_p_new(UnsafeMutablePointer<UInt64>(OpaquePointer(element.data)))
    }

    static func getElementModQ(_ element: ElementModQ) -> OpaquePointer? {
        eg_element_mod_q_new(UnsafeMutablePointer<UInt64>(OpaquePointer(element.data)))
    }
    
    
    struct GCHandle{
        let pointer: OpaquePointer?
    }
}
