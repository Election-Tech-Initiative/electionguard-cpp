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
        
        label.textAlignment = .center
        label.font = .boldSystemFont(ofSize: 16)
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 2
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let contestCard: UIView = {
        let view = UIView()
        
        view.backgroundColor = .systemBackground
        view.layer.cornerRadius = 36
        view.translatesAutoresizingMaskIntoConstraints = false
        
        return view
    }()
    
    let activeContestLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Active Contest"
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let contestNameLabel: UILabel = {
        let label = UILabel()
        
        label.font = .boldSystemFont(ofSize: 24)
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 3
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let activeContestMessageLabel: UILabel = {
        let label = UILabel()
        
        label.textAlignment = .center
        label.lineBreakMode = .byWordWrapping
        label.numberOfLines = 4
        label.isHidden = true
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
    
    private var activeContest: ContestDescription?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        title = "Election Guard Demo"
    }
    
    override func viewWillAppear(_ animated: Bool) {
        loadData()
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            electionNameLabel,
            contestCard,
            activeContestLabel,
            contestNameLabel,
            activeContestMessageLabel,
            beginButton
        ])
        
        let constraints = [
            electionNameLabel.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            electionNameLabel.bottomAnchor.constraint(equalTo: contestCard.topAnchor, constant: -60),

            contestCard.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            contestCard.centerYAnchor.constraint(equalTo: view.centerYAnchor),
            contestCard.widthAnchor.constraint(equalToConstant: 300),
            contestCard.heightAnchor.constraint(equalToConstant: 340),
            
            activeContestLabel.topAnchor.constraint(equalTo: contestCard.topAnchor, constant: 40),
            activeContestLabel.leadingAnchor.constraint(equalTo: contestCard.leadingAnchor, constant: 30),
            activeContestLabel.trailingAnchor.constraint(equalTo: contestCard.trailingAnchor, constant: -30),
            
            contestNameLabel.topAnchor.constraint(equalTo: activeContestLabel.bottomAnchor, constant: 12),
            contestNameLabel.leadingAnchor.constraint(equalTo: activeContestLabel.leadingAnchor),
            contestNameLabel.trailingAnchor.constraint(equalTo: activeContestLabel.trailingAnchor),
            
            activeContestMessageLabel.bottomAnchor.constraint(equalTo: contestCard.bottomAnchor, constant: -40),
            activeContestMessageLabel.leadingAnchor.constraint(equalTo: activeContestLabel.leadingAnchor),
            activeContestMessageLabel.trailingAnchor.constraint(equalTo: activeContestLabel.trailingAnchor),
            
            beginButton.centerXAnchor.constraint(equalTo: contestCard.centerXAnchor),
            beginButton.bottomAnchor.constraint(equalTo: contestCard.bottomAnchor, constant: -20),
            beginButton.widthAnchor.constraint(equalToConstant: 200),
            beginButton.heightAnchor.constraint(equalToConstant: 60)
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    private func loadData() {
        let election = EGDataService.shared.getElectionManifest()
        
        electionNameLabel.text = election?.name?.text?.first?.value
        activeContest = election?.contests?.first

        guard activeContest != nil else {
            contestNameLabel.text = "There is currently not an active contest available"
            beginButton.isHidden = true
            activeContestMessageLabel.isHidden = true
            
            return
        }
        
        contestNameLabel.text = activeContest?.name
        
        let votes = EGDataService.shared.votes
        let canVote = !votes.contains(where: { $0.key == activeContest?.objectId && $0.value == true })
        
        // Adjust controls based on ability to vote for active contest
        beginButton.isHidden = !canVote
        activeContestMessageLabel.isHidden = canVote
        activeContestMessageLabel.text = canVote
            ? ""
            : "Your vote has already been counted for this contest"
    }
    
    @objc private func showContest() {
        guard let activeContest = activeContest else {
            return
        }
        
        let contest = ContestViewController()
        
        contest.contestId = activeContest.objectId
        navigationController?.pushViewController(contest, animated: true)
    }
}

