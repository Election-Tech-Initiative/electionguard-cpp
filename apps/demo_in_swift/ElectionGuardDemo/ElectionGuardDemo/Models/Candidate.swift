//
//  Candidate.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct Candidate: Codable {
    let objectId: String?
    let ballotName: InternationalizedText?
    let partyId: String?
    let imageUri: String?
    let isWriteIn: Bool?
    
    enum CodingKeys: String, CodingKey {
        case objectId = "object_id"
        case ballotName = "ballot_name"
        case partyId = "party_id"
        case imageUri = "image_uri"
        case isWriteIn = "is_write_in"
    }
}
