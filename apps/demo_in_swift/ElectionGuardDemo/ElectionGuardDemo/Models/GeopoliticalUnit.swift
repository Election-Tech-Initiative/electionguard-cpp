//
//  GeopoliticalUnit.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct GeopoliticalUnit: Codable {
    let id: String?
    let name: String?
    let type: GeopoliticalUnitType?
    let contactInfo: ElectionContactInfo?
    
    enum CodingKeys: String, CodingKey {
        case id = "object_id"
        case name
        case type
        case contactInfo = "contact_information"
    }
}

enum GeopoliticalUnitType: String, Codable {
    case unknown
    case county
    case township
    case school
}
