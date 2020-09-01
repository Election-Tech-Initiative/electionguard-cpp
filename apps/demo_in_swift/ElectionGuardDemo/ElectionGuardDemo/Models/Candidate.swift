//
//  Candidate.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct Candidate: Codable {
    let id: String?
    let name: Name?
    let partyId: String?
    let isWriteIn: Bool?
    
    enum CodingKeys: String, CodingKey {
        case id = "object_id"
        case name = "ballot_name"
        case partyId = "party_id"
        case isWriteIn = "is_write_in"
    }
}
