//
//  SelectionCollectionViewCell.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class SelectionCollectionViewCell: UICollectionViewCell {
    
    var selection: BallotSelection? {
        didSet {
            setName()
        }
    }
    
    private let nameLabel: UILabel = {
        let label = UILabel()
        
        label.translatesAutoresizingMaskIntoConstraints = false
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 2
        
        return label
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setup()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setup() {
        addSubviews([nameLabel])
        
        let constraints = [
            nameLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 14),
            nameLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -14),
            nameLabel.centerYAnchor.constraint(equalTo: centerYAnchor)
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    private func setName() {
        guard let candidateId = selection?.candidateId else {
            return
        }
        
        let candidateName = EGDataService.shared.getCandidateName(forId: candidateId)?
                                .text?.first(where: { $0.language == "en" })?.value
        
        nameLabel.text = candidateName ?? ""
    }
}
