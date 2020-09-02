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

struct SelectionDescription: Codable {
    let objectId: String?
    let candidateId: String?
    let sequenceOrder: UInt64?
    
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

enum VoteVariationType: String, Codable {
    case unknown
    case one_of_m
    case approval
    case borda
    case cumulative
    case majority
    case n_of_m
    case plurality
    case proportional
    case range
    case rcv
    case super_majority
    case other
}
