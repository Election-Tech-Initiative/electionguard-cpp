//
//  ContestViewController.swift
//  ElectionGuardDemo
//
//  Created by Ed Snider on 8/21/20.
//  Copyright © 2020 Microsoft. All rights reserved.
//

import UIKit

class ContestViewController: UIViewController {
    
    let nameLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Contest name"
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let ballotTitleLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Ballot title"
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let ballotSubtitleLabel: UILabel = {
        let label = UILabel()
        
        label.text = "Ballot subtitle"
        label.translatesAutoresizingMaskIntoConstraints = false
        
        return label
    }()
    
    let selectionsCollectionView: UICollectionView = {
        let layout = UICollectionViewFlowLayout()
        
        layout.sectionInset = UIEdgeInsets(top: 20, left: 0, bottom: 0, right: 0)
        
        let collectionView = UICollectionView(frame: .zero, collectionViewLayout: layout)
        
        collectionView.backgroundColor = .clear
        collectionView.translatesAutoresizingMaskIntoConstraints = false
        collectionView.register(SelectionCollectionViewCell.self, forCellWithReuseIdentifier: "selectionCell")
        
        return collectionView
    }()
    
    let submitButton: EGButton = {
        let button = EGButton()
        
        button.setTitle("Submit", for: .normal)
        button.addTarget(self, action: #selector(submit), for: .touchUpInside)
        button.translatesAutoresizingMaskIntoConstraints = false
        
        return button
    }()
    
    private var selections = [String]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        selectionsCollectionView.delegate = self
        selectionsCollectionView.dataSource = self
        
        loadSelections()
    }
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .secondarySystemBackground
        
        view.addSubviews([
            nameLabel,
            ballotTitleLabel,
            ballotSubtitleLabel,
            selectionsCollectionView,
            submitButton
        ])
        
        let constraints = [
            nameLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 20),
            nameLabel.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            
            ballotTitleLabel.topAnchor.constraint(equalTo: nameLabel.bottomAnchor, constant: 12),
            ballotTitleLabel.leadingAnchor.constraint(equalTo: nameLabel.leadingAnchor),
            
            ballotSubtitleLabel.topAnchor.constraint(equalTo: ballotTitleLabel.bottomAnchor, constant: 12),
            ballotSubtitleLabel.leadingAnchor.constraint(equalTo: nameLabel.leadingAnchor),
            
            submitButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            submitButton.widthAnchor.constraint(equalToConstant: 300),
            submitButton.heightAnchor.constraint(equalToConstant: 60),
            submitButton.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -12),
            
            selectionsCollectionView.topAnchor.constraint(equalTo: ballotSubtitleLabel.bottomAnchor, constant: 12),
            selectionsCollectionView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20),
            selectionsCollectionView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20),
            selectionsCollectionView.bottomAnchor.constraint(equalTo: submitButton.topAnchor, constant: -20)
        ]
        
        NSLayoutConstraint.activate(constraints)
    }
    
    private func loadSelections() {
        selections = ["Selection 1", "Selection 2"]
        selectionsCollectionView.reloadData()
    }
    
    @objc private func submit() {
        
    }
}

extension ContestViewController: UICollectionViewDelegateFlowLayout, UICollectionViewDataSource {
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        return .init(width: 300, height: 80)
    }

    func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }

    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return selections.count
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "selectionCell", for: indexPath) as! SelectionCollectionViewCell
        
        cell.backgroundColor = .tertiarySystemBackground
        cell.layer.cornerRadius = 16
        cell.selection = selections[indexPath.row]
        
        return cell
    }

    func collectionView(_ collectionView: UICollectionView, didHighlightItemAt indexPath: IndexPath) {
        let cell = collectionView.cellForItem(at: indexPath)
        
        cell?.backgroundColor = cell?.backgroundColor?.withAlphaComponent(0.5)
    }

    func collectionView(_ collectionView: UICollectionView, didUnhighlightItemAt indexPath: IndexPath) {
        let cell = collectionView.cellForItem(at: indexPath)
        
        cell?.backgroundColor = .tertiarySystemBackground
    }

    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        let cell = collectionView.cellForItem(at: indexPath)
        
        cell?.backgroundColor = UIColor(named: "eg_lightGreen")
    }
    
    func collectionView(_ collectionView: UICollectionView, didDeselectItemAt indexPath: IndexPath) {
        let cell = collectionView.cellForItem(at: indexPath)
        
        cell?.backgroundColor = .tertiarySystemBackground
    }
}
