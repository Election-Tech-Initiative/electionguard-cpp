//
//  ElectionDescription.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct ElectionDescription: Codable {
    let electionScopeId: String?
    let name: InternationalizedText?
    let type: ElectionType?
    let contactInformation: ContactInformation?
    let startDate: Date?
    let endDate: Date?
    let geopoliticalUnits: [GeopoliticalUnit]?
    let candidates: [Candidate]?
    let parties: [Party]?
    let contests: [ContestDescription]?
    let ballotStyles: [BallotStyle]?
    
    enum CodingKeys: String, CodingKey {
        case electionScopeId = "election_scope_id"
        case name
        case type
        case contactInformation = "contact_information"
        case startDate = "start_date"
        case endDate = "end_date"
        case geopoliticalUnits = "geopolitical_units"
        case candidates
        case parties
        case contests
        case ballotStyles = "ballot_styles"
    }
    
    func getContests(ballotStyleId: String) -> [ContestDescription] {
        let style = getBallotStyle(objectId: ballotStyleId)
        
        guard let gpUnitIds = style?.geopoliticalUnitIds, !gpUnitIds.isEmpty else {
            return [ContestDescription]()
        }
        
        return contests?.filter { c in return gpUnitIds.contains(c.electoralDistrictId ?? "") } ?? [ContestDescription]()
    }
    
    func getBallotStyle(objectId: String) -> BallotStyle? {
        return ballotStyles?.first(where: { $0.objectId == objectId })
    }
}

enum ElectionType: Int, Codable {
    case unknown = 0
    case general = 1
    case partisan_primary_closed = 2
    case partisan_primary_open = 3
    case primary = 4
    case runoff = 5
    case special = 6
    case other = 7
}
