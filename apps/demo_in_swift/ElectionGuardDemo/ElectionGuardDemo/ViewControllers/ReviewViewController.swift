//
//  ReviewViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/26/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class ReviewViewController: UIViewController {
    
    var selection: SelectionDescription?
    var contestId: String?
    
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
    
    let submitButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Encrypt & Submit", for: .normal)
        button.addTarget(self, action: #selector(submit), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        title = "Review"
        
        loadCandidate()
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            instructionsLabel,
            selectionLabel,
            submitButton
        ])
        
        let constraints = [
            instructionsLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            instructionsLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            instructionsLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            selectionLabel.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            selectionLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            selectionLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            selectionLabel.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            
            submitButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            submitButton.widthAnchor.constraint(equalToConstant: 300),
            submitButton.heightAnchor.constraint(equalToConstant: 60),
            submitButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -12),
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
    
    @objc private func submit() {
        guard selection != nil else {
            return
        }

        showDialog(title: "Encrypt ballot",
                   body: "You are about to encrypt your ballot. This cannot be undone. Do you wish to continue?",
                   yesText: "Yes",
                   noText: "No",
                   handler: { yes in
            if yes {
                self.encryptBallot { code in
                    self.showVerification(code: code)
                }
            }
        })
    }

    private func encryptBallot(completion: @escaping (String?) -> Void) {
        ProgressHUD.show("Encrypting ballot...", interaction: false)
        
        DispatchQueue.main.asyncAfter(deadline: .now() + 2, execute: {
            // TODO: Encrypt ballot here
            _ = ElectionGuard.mainTest()
            ProgressHUD.dismiss()
            completion("checker ACND2 sorry K2JJD connect 9ANDK four 8C372 unreal G338F blizzard FGG1J super 4RR81 tornado J7BCA")
        })
    }

    private func showVerification(code: String?) {
        let verification = VerificationViewController()
        
        verification.code = code
        verification.contestId = contestId
        self.navigationController?.pushViewController(verification, animated: true)
    }
}
