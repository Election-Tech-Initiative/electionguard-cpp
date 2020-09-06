//
//  FinishedViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 9/4/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class FinishedViewController: UIViewController {
    var isSpoiled: Bool? {
        didSet {
            revoteButton.isHidden = self.isSpoiled != true
            messageLabel.text = self.isSpoiled == true ? "Your vote has been spoiled." : "Your vote has been counted!"
        }
    }
    
    let messageLabel: UILabel = {
        let label = UILabel()
        
        label.font = .boldSystemFont(ofSize: 20)
        label.textAlignment = .center
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()

    let revoteButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Re Vote", for: .normal)
        button.addTarget(self, action: #selector(revote), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    let closeButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Close", for: .normal)
        button.addTarget(self, action: #selector(close), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        title = "Thank You"
        navigationItem.hidesBackButton = true
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            messageLabel,
            revoteButton,
            closeButton
        ])
        
        let constraints = [
            messageLabel.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            messageLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            messageLabel.widthAnchor.constraint(equalToConstant: 250),

            revoteButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            revoteButton.widthAnchor.constraint(equalToConstant: 300),
            revoteButton.heightAnchor.constraint(equalToConstant: 60),
            revoteButton.bottomAnchor.constraint(equalTo: closeButton.topAnchor, constant: -12),
            
            closeButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            closeButton.widthAnchor.constraint(equalToConstant: 300),
            closeButton.heightAnchor.constraint(equalToConstant: 60),
            closeButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -12),
        ]
        
        NSLayoutConstraint.activate(constraints)
    }

    @objc private func revote() {
        guard isSpoiled == true else {
            return
        }
        
        guard let contest = navigationController?.viewControllers.first(where: { $0 is ContestViewController }) else {
            navigationController?.popToRootViewController(animated: true)
            return
        }
        
        navigationController?.popToViewController(contest, animated: true)
    }
    
    @objc private func close() {
        navigationController?.popToRootViewController(animated: true)
    }
}
