//
//  ContestDescription.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

class ContestDescription: Codable {
    let type: ContestType?
    let objectId: String?
    let sequenceOrder: Int?
    let ballotSelections: [SelectionDescription]?
    let ballotTitle: InternationalizedText?
    let ballotSubtitle: InternationalizedText?
    let voteVariation: VoteVariationType?
    let electoralDistrictId: String?
    let name: String?
    let primaryPartyIds: [String]?
    let numberElected: Int?
    let votesAllowed: Int?
    
    enum CodingKeys: String, CodingKey {
        case type = "@type"
        case objectId = "object_id"
        case sequenceOrder = "sequence_order"
        case ballotSelections = "ballot_selections"
        case ballotTitle = "ballot_title"
        case ballotSubtitle = "ballot_subtitle"
        case voteVariation = "vote_variation"
        case electoralDistrictId = "electoral_district_id"
        case name
        case primaryPartyIds = "primary_party_ids"
        case numberElected = "number_elected"
        case votesAllowed = "votes_allowed"
    }
}

class ContestDescriptionWithPlaceholders: ContestDescription {
    let placeholderSelections: [SelectionDescription]? = nil
}

class SelectionDescription: Codable {
    var objectId: String? = ""
    var candidateId: String? = ""
    var sequenceOrder: UInt64? = 0
    
    enum CodingKeys : String, CodingKey {
        case objectId = "object_id"
        case candidateId = "candidate_id"
        case sequenceOrder = "sequence_order"
    }
}

enum ContestType: String, Codable {
    case unknown
    case candidate = "CandidateContest"
    case referendum = "ReferendumContest"
}

enum VoteVariationType: Int, Codable {
    case unknown = 0
    case one_of_m = 1
    case approval = 2
    case borda = 3
    case cumulative = 4
    case majority = 5
    case n_of_m = 6
    case plurality = 7
    case proportional = 8
    case range = 9
    case rcv = 10
    case super_majority = 11
    case other = 12
}


extension SelectionDescription {
    func toPlaintextBallotSelection(isPlaceholder: Bool = false, isAffirmative: Bool = false) -> PlaintextBallotSelection {
        return PlaintextBallotSelection(objectId: self.objectId!, vote: isAffirmative ? 1 : 0)
    }
    
    func cryptoHash() -> ElementModQ? {
        var toHash = "|"
        
        toHash += self.objectId ?? "null"
        toHash += "|"
        toHash += self.sequenceOrder != nil ? String(self.sequenceOrder!) : "null"
        toHash += "|"
        toHash += self.candidateId ?? "null"
        toHash += "|"
        
        return ElectionGuard.hash(toHash)
    }
}

extension ContestDescription {
    func toPlaintextBallotContest() -> PlaintextBallotContest {
        return PlaintextBallotContest(
            objectId: self.objectId!,
            ballotSelections: self.ballotSelections?.map { $0.toPlaintextBallotSelection() } ?? [PlaintextBallotSelection]())
    }
    
    func cryptoHash() -> ElementModQ? {
        
        var toHash = "|"
        
        toHash += self.objectId ?? "null"
        toHash += "|"
        toHash += self.sequenceOrder != nil ? String(self.sequenceOrder!) : "null"
        toHash += "|"
        toHash += self.electoralDistrictId ?? "null"
        toHash += "|"
        toHash += self.voteVariation != nil ? String(self.voteVariation!.rawValue) : "null" // TODO: Confirm this is correct
        toHash += "|"
        
        if let titleLanguages = self.ballotTitle?.text {
            for text in titleLanguages {
                toHash += text.value ?? "null"
                toHash += "|"
                // TODO: Include .language?
            }
        } else {
            toHash += "null|"
        }
        
        if let subtitleLanguages = self.ballotSubtitle?.text {
            for text in subtitleLanguages {
                toHash += text.value ?? "null"
                toHash += "|"
                // TODO: Include .language?
            }
        } else {
            toHash += "null|"
        }
        
        toHash += self.name ?? "null"
        toHash += "|"
        toHash += self.numberElected != nil ? String(self.numberElected!) : "null"
        toHash += "|"
        toHash += self.votesAllowed != nil ? String(self.votesAllowed!) : "null"
        toHash += "|"
        
        if let selections = self.ballotSelections {
            for selection in selections {
                // TODO: Replace with actual hash of an ElementModQ instance
                toHash += "ELEMENT_MOD_Q_HASH"
                toHash += "|"
            }
        } else {
            toHash += "null|"
        }
        
        return ElectionGuard.hash(toHash)
    }
}
