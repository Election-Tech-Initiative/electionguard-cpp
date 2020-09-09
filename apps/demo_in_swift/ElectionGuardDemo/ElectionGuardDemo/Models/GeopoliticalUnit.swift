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

enum ReportingUnitType: Int, Codable {
    case unknown = 0
    case ballot_batch = 1
    case ballot_style_area = 2
    case borough = 3
    case city = 4
    case city_council = 5
    case combined_precinct = 6
    case congressional = 7
    case country = 8
    case county = 9
    case county_council = 10
    case drop_box = 11
    case judicial = 12
    case municipality = 13
    case polling_place = 14
    case precinct = 15
    case school = 16
    case special = 17
    case split_precinct = 18
    case state = 19
    case state_house = 20
    case state_senate = 21
    case township = 22
    case utility = 23
    case village = 24
    case vote_center = 25
    case ward = 26
    case water = 27
    case other = 28
}
