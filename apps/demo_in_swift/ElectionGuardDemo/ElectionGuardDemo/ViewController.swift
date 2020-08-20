//
//  ViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/20/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        
        _ = ElectionGuard.encryptSelection(objectId: "some-object-id", vote: "1")
    }
}

