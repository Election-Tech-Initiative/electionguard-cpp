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
    
    func getElectionContext() -> CiphertextElectionContext? {
        let json = """
    {"crypto_base_hash": "32691573411347233797389113720176532355599649930735751534902623911469109902502", "crypto_extended_base_hash": "104227560160703924253371832430508940762128870347062722643933104744368792001225", "description_hash": "111645506576807617916720741496896823744186328177133991469015506590684696470386", "elgamal_public_key": "272472971784857062318186037869821910944623755103085175766630597987638442742535928983440123504901309849199670533691500535640178207286007838529090139107528203120018045153871239151909280333822947546838501195123901760723022043892872456294186641920959946105078290918454346533777259280207431645562322639207126561858650175610063890281722203452310590459996093173631882929260659136825363052812452559843199065336768379658801726201354577813339905390436281825498951198042472557014838361569692912448626238612883740576389228689749632230077836692136301635333009703853577887742237860769243919096484778102512160277150918652498292917073346746587432096056003573856627561304128995347115348737413647420359440460531114585826172550207948149249041101150059103446644094637363541944941037636050644572959433620607464865927789795780728935054506489189299675612168269268066742428529692350056166520489645319686523746992798384008225738056914483943225768947537282002762371995473077796934892044328349847212891011887347180067207161879154588304908155350687983658766893532581301950082177995923363441836439786091374507485417318244611104091276789254052304886621109350264786803352737338705539003014527189976888464761819820985550247401053647074377051641963166904791235170945", "number_of_guardians": 5, "quorum": 3}
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
