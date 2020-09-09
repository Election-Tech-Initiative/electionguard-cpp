//
//  HttpDataService.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 9/4/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import Foundation

struct APIResult<T> {
    var data: T?
}

struct APIResultError: Error {
    let message: String?
}

class HttpDataService {
    func sendRequest<T: Decodable>(url: String, httpMethod: String? = "GET", completion: @escaping ((APIResult<T>?, APIResultError?) -> Void)) {
        guard let url = URL(string: url) else {
            print("Error constructing URL request for endpoint")
            return
        }
        
        let request = URLRequest(url: url)
        let session = URLSession.shared
        let task = session.dataTask(with: request) { data, response, error in
            guard error == nil else {
                completion(nil, error as? APIResultError)
                return
            }
            
            guard let response = response as? HTTPURLResponse else {
                completion(nil, APIResultError(message: "Response is NULL"))
                return
            }
            
            if response.statusCode >= 400 {
                completion(nil, APIResultError(message: "Response returned unsuccessful status code: \(response.statusCode)"))
                return
            }
            
            guard let data = data else {
                completion(nil, APIResultError(message: "Response data is NULL"))
                return
            }
            
            guard let result: T? = JsonHelper.fromJson(data) else {
                completion(nil, APIResultError(message: "Response decoding failed"))
                return
            }
            
            completion(APIResult(data: result), nil)
        }
        
        task.resume()
    }
}
