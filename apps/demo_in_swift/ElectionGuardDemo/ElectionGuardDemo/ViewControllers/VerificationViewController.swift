//
//  VerificationViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/27/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class VerificationViewController: UIViewController {
    
    var contestId: String?
    var code: String? {
        didSet {
            codeLabel.text = self.code
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

    let castButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Cast", for: .normal)
        button.addTarget(self, action: #selector(cast), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    let spoilButton: UIButton = {
        let button = UIButton(type: .system)
        
        button.setTitle("Test", for: .normal)
        button.addTarget(self, action: #selector(spoil), for: .touchUpInside)
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
            castButton,
            spoilButton
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

            castButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            castButton.widthAnchor.constraint(equalToConstant: 300),
            castButton.heightAnchor.constraint(equalToConstant: 60),
            castButton.bottomAnchor.constraint(equalTo: spoilButton.topAnchor, constant: -12),
            
            spoilButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            spoilButton.widthAnchor.constraint(equalToConstant: 300),
            spoilButton.heightAnchor.constraint(equalToConstant: 60),
            spoilButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -12),
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    @objc private func copyCode() {
        UIPasteboard.general.string = code
        ProgressHUD.showSucceed("Copied to clipboard", interaction: false)
    }

    @objc private func cast() {
        ProgressHUD.show("Casting ballot...", interaction: false)
        EGDataService.shared.castBallot(contestId: contestId!, completion: {
            ProgressHUD.dismiss()
            self.showFinished()
        })
    }
    
    @objc private func spoil() {
        ProgressHUD.show("Spoiling ballot...", interaction: false)
        EGDataService.shared.spoilBallot(contestId: contestId!, completion: {
            ProgressHUD.dismiss()
            self.showFinished(isSpoiled: true)
        })
    }
    
    private func showFinished(isSpoiled: Bool = false) {
        let finished = FinishedViewController()
        
        finished.isSpoiled = isSpoiled
        self.navigationController?.pushViewController(finished, animated: true)
    }
}
