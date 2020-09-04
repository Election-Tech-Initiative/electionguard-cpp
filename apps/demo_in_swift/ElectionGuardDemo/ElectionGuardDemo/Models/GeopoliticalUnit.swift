//
//  GeopoliticalUnit.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

struct GeopoliticalUnit: Codable {
    let objectId: String?
    let name: String?
    let type: ReportingUnitType?
    let contactInfo: ContactInformation?
    
    enum CodingKeys: String, CodingKey {
        case objectId = "object_id"
        case name
        case type
        case contactInfo = "contact_information"
    }
}

enum ReportingUnitType: String, Codable {
    case unknown
    case ballot_batch
    case ballot_style_area
    case borough
    case city
    case city_council
    case combined_precinct
    case congressional
    case country
    case county
    case county_council
    case drop_box
    case judicial
    case municipality
    case polling_place
    case precinct
    case school
    case special
    case split_precinct
    case state
    case state_house
    case state_senate
    case township
    case utility
    case village
    case vote_center
    case ward
    case water
    case other
}
