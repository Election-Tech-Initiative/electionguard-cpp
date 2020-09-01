//
//  EGDataService.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

class EGDataService {
    static let shared = EGDataService()

    // Dictionary for keeping track of cast ballots in memory (demo purposes only)
    var votes = [String: Bool]() // Key: contestId; Value: True (if cast), False (if spoiled)

    private init() { }
    
    func getElectionManifest() -> ElectionManifest? {
        let json = """
    {"ballot_styles": [{"geopolitical_unit_ids": ["hamilton-county"], "object_id": "congress-district-7-hamilton-county"}], "candidates": [{"ballot_name": {"text": [{"language": "en", "value": "Joseph Barchi"}]}, "object_id": "barchi"}, {"ballot_name": {"text": [{"language": "en", "value": "Adam Cramer"}]}, "object_id": "cramer"}, {"ballot_name": {"text": [{"language": "en", "value": "Daniel Court"}]}, "object_id": "court"}, {"ballot_name": {"text": [{"language": "en", "value": "Alvin Boone"}]}, "object_id": "boone"}, {"ballot_name": {"text": [{"language": "en", "value": "Ashley Hildebrand-McDougall"}]}, "object_id": "hildebrand"}, {"ballot_name": {"text": [{"language": "en", "value": "Martin Patterson"}]}, "object_id": "patterson"}, {"ballot_name": {"text": [{"language": "en", "value": "Charlene Franz"}]}, "object_id": "franz"}, {"ballot_name": {"text": [{"language": "en", "value": "Gerald Harris"}]}, "object_id": "harris"}, {"ballot_name": {"text": [{"language": "en", "value": "Linda Bargmann"}]}, "object_id": "bargmann"}, {"ballot_name": {"text": [{"language": "en", "value": "Barbara Abcock"}]}, "object_id": "abcock"}], "contests": [{"@type": "CandidateContest", "ballot_selections": [{"candidate_id": "barchi", "object_id": "barchi-selection", "sequence_order": 0}, {"candidate_id": "cramer", "object_id": "cramer-selection", "sequence_order": 1}, {"candidate_id": "court", "object_id": "court-selection", "sequence_order": 2}, {"candidate_id": "boone", "object_id": "boone-selection", "sequence_order": 3}, {"candidate_id": "hildebrand", "object_id": "hildebrand-selection", "sequence_order": 4}, {"candidate_id": "patterson", "object_id": "patterson-selection", "sequence_order": 5}, {"candidate_id": "franz", "object_id": "franz-selection", "sequence_order": 6}, {"candidate_id": "harris", "object_id": "harris-selection", "sequence_order": 7}, {"candidate_id": "bargmann", "object_id": "bargmann-selection", "sequence_order": 8}, {"candidate_id": "abcock", "object_id": "abcock-selection", "sequence_order": 9}], "ballot_subtitle": {"text": [{"language": "en", "value": "Vote for one"}, {"language": "es", "value": "Votar por uno"}]}, "ballot_title": {"text": [{"language": "en", "value": "Hamilton County School Board"}, {"language": "es", "value": "Junta Escolar del Condado de Hamilton"}]}, "electoral_district_id": "hamilton-county", "name": "Hamilton County School Board", "number_elected": 1, "object_id": "school-board-contest", "sequence_order": 0, "vote_variation": "one_of_m", "votes_allowed": 1}], "election_scope_id": "hamilton-county-general-election", "end_date": "2020-03-01T20:00:00-05:00", "geopolitical_units": [{"contact_information": {"address_line": ["1234 Samuel Adams Way", "Hamilton, Ozark 99999"], "email": [{"annotation": "inquiries", "value": "inquiries@hamiltoncounty.gov"}], "name": "Hamilton County Clerk", "phone": [{"annotation": "domestic", "value": "123-456-7890"}]}, "name": "Hamilton County", "object_id": "hamilton-county", "type": "county"}], "name": {"text": [{"language": "en", "value": "Hamiltion County General Election"}, {"language": "es", "value": "Elección general del condado de Hamilton"}]}, "parties": [], "start_date": "2020-03-01T08:00:00-05:00", "type": "general"}
    """
            
        return JsonHelper.fromJson(json.data(using: .utf8)!)
    }
    
    func getCandidateName(forId id: String) -> Name? {
        let manifest = getElectionManifest()
        let candidate = manifest?.candidates?.first(where: { $0.id == id })
        
        return candidate?.name
    }
    
    func castBallot(contestId: String, completion: @escaping (String?) -> Void) {
        // TODO: Replace hardcoded delay with actual API call
        votes[contestId] = true
        DispatchQueue.main.asyncAfter(deadline: .now() + 2, execute: {
            completion("checker ACND2 sorry K2JJD connect 9ANDK four 8C372 unreal G338F blizzard FGG1J super 4RR81 tornado J7BCA")
        })
    }
    
    func spoilBallot(contestId: String, completion: @escaping (String?) -> Void) {
        // TODO: Replace hardcoded delay with actual API call
        votes[contestId] = false
        DispatchQueue.main.asyncAfter(deadline: .now() + 2, execute: {
            completion("chess ACND2 sorry K2JJD connect 9ANDK four 8C372 unreal G338F blizzard FGG1J super 4RR81 tornado J7BCA")
        })
    }
}
