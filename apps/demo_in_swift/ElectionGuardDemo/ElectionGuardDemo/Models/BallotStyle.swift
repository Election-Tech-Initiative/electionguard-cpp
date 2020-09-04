//
//  BallotStyle.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct BallotStyle: Codable {
    let objectId: String?
    let geopoliticalUnitIds: [String]?
    let partyIds: [String]?
    let imageUri: String?
    
    enum CodingKeys: String, CodingKey {
        case objectId = "object_id"
        case geopoliticalUnitIds = "geopolitical_unit_ids"
        case partyIds = "party_ids"
        case imageUri = "image_uri"
    }
}
