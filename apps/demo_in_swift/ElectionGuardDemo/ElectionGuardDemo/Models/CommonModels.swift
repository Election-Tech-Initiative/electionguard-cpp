//
//  CommonModels.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct AnnontatedString: Codable {
    let annotation: String?
    let value: String?
}

struct Language: Codable {
    let value: String?
    let language: String?
}

struct InternationalizedText: Codable {
    let text: [Language]?
}

struct ContactInformation: Codable {
    let addressLine: [String]?
    let name: String?
    let email: [AnnontatedString]?
    let phone: [AnnontatedString]?
    
    enum CodingKeys: String, CodingKey {
        case addressLine = "address_line"
        case name
        case email
        case phone
    }
}
