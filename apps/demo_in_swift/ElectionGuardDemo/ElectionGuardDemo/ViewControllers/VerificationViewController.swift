//
//  VerificationViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/27/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class VerificationViewController: UIViewController {
    
    var code: String? {
        didSet {
            codeLabel.text = self.code
        }
    }
    
    var isSpoiled: Bool? {
        didSet {
            revoteButton.isHidden = self.isSpoiled != true
            messageLabel.text = self.isSpoiled == true ? "Your vote has been spoiled." : "Your vote has been counted!"
        }
    }
    
    let messageLabel: UILabel = {
        let label = UILabel()
        
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let instructionsLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Below is your tracking ID. You can use this to verify your ballot was successfully included in the election."
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let codeLabel: UILabel = {
        let label = UILabel()
        
        label.font = .boldSystemFont(ofSize: 20)
        label.textAlignment = .center
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 6
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let copyButton: UIButton = {
        let button = UIButton(type: .system)

        button.setTitle("Copy", for: .normal)
        button.addTarget(self, action: #selector(copyCode), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false

        return button
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
        
        title = "Verification"
        navigationItem.hidesBackButton = true
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            messageLabel,
            instructionsLabel,
            codeLabel,
            copyButton,
            revoteButton,
            closeButton
        ])
        
        let constraints = [
            messageLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            messageLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            messageLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            instructionsLabel.topAnchor.constraint(equalTo: messageLabel.bottomAnchor, constant: 12),
            instructionsLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            instructionsLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            codeLabel.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            codeLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            codeLabel.widthAnchor.constraint(equalToConstant: 250),
            
            copyButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            copyButton.topAnchor.constraint(equalTo: codeLabel.bottomAnchor, constant: 12),

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
    
    @objc private func copyCode() {
        UIPasteboard.general.string = code
        ProgressHUD.showSucceed("Copied to clipboard", interaction: false)
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
