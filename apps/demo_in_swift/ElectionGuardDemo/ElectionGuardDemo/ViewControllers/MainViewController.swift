//
//  MainViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/20/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class MainViewController: UIViewController {

    let electionNameLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Election name"
        label.textAlignment = .center
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let beginButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Begin", for: .normal)
        button.addTarget(self, action: #selector(showContest), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        title = "Election Guard Demo"
        
        loadData()
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            electionNameLabel,
            beginButton
        ])
        
        let constraints = [
            electionNameLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            electionNameLabel.centerYAnchor.constraint(equalTo: view.centerYAnchor, constant: -100),
            
            beginButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            beginButton.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            beginButton.widthAnchor.constraint(equalToConstant: 300),
            beginButton.heightAnchor.constraint(equalToConstant: 60)
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    private func loadData() {
        let election = EGDataService.shared.getElectionManifest()
        
        electionNameLabel.text = election?.name?.text?.first?.value
    }
    
    @objc private func showContest() {
        navigationController?.pushViewController(ContestViewController(), animated: true)
    }
}

