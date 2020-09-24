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
}
