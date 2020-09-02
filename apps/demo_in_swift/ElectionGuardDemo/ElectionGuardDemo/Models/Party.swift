//
//  Party.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct Party: Codable {
    let id: String?
    let abbreviation: String?
    let color: String?
    let logoUri: String?
    let ballotName: InternationalizedText?
    
    enum CodingKeys: String, CodingKey {
        case id = "object_id"
        case abbreviation
        case color
        case logoUri = "logo_uri"
        case ballotName = "name"
    }
}
