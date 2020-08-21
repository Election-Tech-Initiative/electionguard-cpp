//
//  ElectionManifest.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct ElectionManifest: Codable {
    let scopeId: String?
    let name: Name?
    let type: ElectionType?
    let contactInfo: ElectionContactInfo?
    let startDate: Date?
    let endDate: Date?
    let geopoliticalUnits: [GeopoliticalUnit]?
    let candidates: [Candidate]?
    let parties: [Party]?
    let contests: [Contest]?
    let ballotStyles: [BallotStyle]?
    
    enum CodingKeys: String, CodingKey {
        case scopeId = "election_scope_id"
        case name
        case type
        case contactInfo = "contact_information"
        case startDate = "start_date"
        case endDate = "end_date"
        case geopoliticalUnits = "geopolitical_units"
        case candidates
        case parties
        case contests
        case ballotStyles = "ballot_styles"
    }
}

enum ElectionType: String, Codable {
    case unknown
    case primary
}
