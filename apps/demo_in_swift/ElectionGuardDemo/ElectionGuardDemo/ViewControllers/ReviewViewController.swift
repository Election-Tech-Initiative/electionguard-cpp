//
//  ReviewViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/26/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class ReviewViewController: UIViewController {
    
    var selection: BallotSelection?
    
    let instructionsLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Please confirm your selection. If this is not correct, tap the Back button to change your selection."
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let selectionLabel: UILabel = {
        let label = UILabel()
        
        label.font = .boldSystemFont(ofSize: 32)
        label.textAlignment = .center
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let castButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Cast Ballot", for: .normal)
        button.addTarget(self, action: #selector(cast), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        title = "Review"
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Spoil", style: .plain, target: self, action: #selector(spoil))
        
        loadCandidate()
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            instructionsLabel,
            selectionLabel,
            castButton
        ])
        
        let constraints = [
            instructionsLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            instructionsLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            instructionsLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            selectionLabel.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            selectionLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            selectionLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            selectionLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            castButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            castButton.widthAnchor.constraint(equalToConstant: 300),
            castButton.heightAnchor.constraint(equalToConstant: 60),
            castButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -12),
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    private func loadCandidate() {
        guard
            let candidateId = selection?.candidateId,
            let candidateName = EGDataService.shared.getCandidateName(forId: candidateId)?.text?.first(where: { $0.language == "en" })?.value
        else {
            showDialog(title: "Selection Not Found", body: "There was a problem with your selection. Please go back and try again.", okText: "OK")
            return
        }
        
        selectionLabel.text = candidateName
    }
    
    @objc private func cast() {
        guard selection != nil else {
            return
        }
        
        showDialog(title: "Cast ballot",
                   body: "You are about to cast your ballot. This cannot be undone. Do you wish to continue?",
                   yesText: "Yes",
                   noText: "No",
                   handler: { yes in
            if yes {
                self.castBallot { code in
                    self.showVerification(code: code, isSpoiled: false)
                }
            }
        })
    }
    
    @objc private func spoil() {
        guard selection != nil else {
            return
        }
        
        showDialog(title: "Spoil ballot",
                   body: "You are about to spoil your ballot. This cannot be undone. Do you wish to continue?",
                   yesText: "Yes",
                   noText: "No",
                   handler: { yes in
            if yes {
                self.spoilBallot { code in
                    self.showVerification(code: code, isSpoiled: true)
                }
            }
        })
    }
    
    private func castBallot(completion: @escaping (String?) -> Void) {
        ProgressHUD.show("Casting ballot...", interaction: false)
        EGDataService.shared.castBallot(completion: { code in
            ProgressHUD.dismiss()
            completion(code)
        })
    }
    
    private func spoilBallot(completion: @escaping (String?) -> Void) {
        ProgressHUD.show("Spoiling ballot...", interaction: false)
        EGDataService.shared.spoilBallot(completion: { code in
            ProgressHUD.dismiss()
            completion(code)
        })
    }
    
    private func showVerification(code: String?, isSpoiled: Bool) {
        let verification = VerificationViewController()
        
        verification.code = code
        verification.isSpoiled = isSpoiled
        self.navigationController?.pushViewController(verification, animated: true)
    }
}
