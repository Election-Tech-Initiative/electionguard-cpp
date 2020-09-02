//
//  EGButton.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class EGButton: UIButton {
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setup()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setup() {
        backgroundColor = UIColor(named: "eg_darkGreen")
        setTitleColor(.white, for: .normal)
        setTitleColor(.gray, for: .highlighted)
        setTitleColor(.lightGray, for: .disabled)
        titleLabel?.font = UIFont.boldSystemFont(ofSize: 18)
        layer.cornerRadius = 30
    }
}
