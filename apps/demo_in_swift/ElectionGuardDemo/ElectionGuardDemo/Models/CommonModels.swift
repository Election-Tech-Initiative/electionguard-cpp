//
//  CommonModels.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct Name: Codable {
    let text: [Text]?
}

struct Text: Codable {
    let annotation: String?
    let value: String?
    let language: String?
}

struct ElectionContactInfo: Codable {
    let address: [String]?
    let name: String?
    let email: [Text]?
    let phone: [Text]?
    
    enum CodingKeys: String, CodingKey {
        case address = "address_line"
        case name
        case email
        case phone
    }
}
