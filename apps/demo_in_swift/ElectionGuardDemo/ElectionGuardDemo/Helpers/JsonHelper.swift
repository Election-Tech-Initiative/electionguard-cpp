//
//  JsonHelper.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

class JsonHelper {
    static func fromJson<T: Decodable>(_ jsonData: Data) -> T? {
        let decoder = JSONDecoder()
        decoder.dateDecodingStrategy = .iso8601

        do {
            return try decoder.decode(T.self, from: jsonData)
        } catch {
            print(error.localizedDescription)
        }
        
        return nil
    }
}
