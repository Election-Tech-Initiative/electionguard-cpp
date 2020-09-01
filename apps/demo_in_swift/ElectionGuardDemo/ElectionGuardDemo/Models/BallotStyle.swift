//
//  BallotStyle.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct BallotStyle: Codable {
    let id: String?
    let geopoliticalUnitIds: [String]?
    
    enum CodingKeys: String, CodingKey {
        case id = "object_id"
        case geopoliticalUnitIds = "geopolitical_unit_ids"
    }
}
