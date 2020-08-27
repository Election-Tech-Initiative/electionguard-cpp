//
//  UIViewController+alert.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/27/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

extension UIViewController {
    
    func showDialog(title: String, body: String, okText: String = "OK") {
        let alert = UIAlertController(title: title, message: body, preferredStyle: .alert)
        
        alert.addAction(UIAlertAction(title: okText, style: .cancel) { action in })
        present(alert, animated: true, completion: nil)
    }
    
    func showDialog(title: String, body: String, yesText: String = "Yes", noText: String = "No", handler: @escaping (Bool) -> Void) {
        
        let alert = UIAlertController(title: title, message: body, preferredStyle: .alert)
        let yesAction = UIAlertAction(title: yesText, style: .default) { action in handler(true) }
        let noAction = UIAlertAction(title: noText, style: .default) { action in handler(false) }
        
        alert.addAction(yesAction)
        alert.addAction(noAction)
        present(alert, animated: true, completion: nil)
    }
}
