//
//  Contest.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct Contest: Codable {
    let type: ContestType?
    let id: String?
    let sequenceOrder: Int?
    let ballotSelections: [BallotSelection]?
    let ballotTitle: Name?
    let ballotSubtitle: Name?
    let voteVariation: String? // TODO: enum?
    let electoralDistrictId: String?
    let name: String?
    let primaryPartyIds: [String]?
    let numberElected: Int?
    let votesAllowed: Int?
    
    enum CodingKeys: String, CodingKey {
        case type = "@type"
        case id = "object_id"
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

struct BallotSelection: Codable {
    let id: String?
    let sequenceOrder: Int?
    let candidateId: String?
    
    enum CodingKeys : String, CodingKey {
        case id = "object_id"
        case sequenceOrder = "sequence_order"
        case candidateId = "candidate_id"
    }
}

enum ContestType: String, Codable {
    case unknown
    case candidate = "CandidateContest"
    case referendum = "ReferendumContest"
}
